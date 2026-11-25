#!/usr/bin/env python3
"""select_rows.py: pick the next Codex hand-off rows from UNMATCHED_ROWS.tsv (pass 27, 2026-11-22; pass 28 adds --order longest; pass 29 skips a record with retail length 0 and takes --include rows first).

Rule (the pass-26 rule, replayed against pass 26's own inputs to calibrate it):
  - UNMATCHED_ROWS.tsv status exactly `parked`;
  - not a funclet (0x012xxxxx Unwind rows);
  - the best grade on record carries a DIFFER(ours v retail); the row's length is the retail side;
  - closest_version is an existing .cpp draft;
  - no Codex pass has had it: the address is not the first column of any *.tsv in a pending/*codex* folder (NAME_FIXES*.tsv excepted:
    their first column is a callee whose spelling was fixed, not a row), not an address-named
    file / folder there, its draft is not in a *codex* folder, and its status / grade text does not mention Codex;
  - not landed (build_candidates.ps1 / auto-re-candidates.tsv);
  - not named in COORDINATION_CLAIMS.tsv where the newest line naming it is anything but released / done;
  - not held by a lock in $S/w22/nflocks/<addr>/owner (anything but `released`).
Order: smallest |ours - retail| first, then retail length ascending, then address.
Writes rows.tsv in pass 26's 8-column format: address, retail length, best DIFFER, draft path, Ego_d name, Ego_d object, summary, extras.

usage: select_rows.py --unmatched U --ps1 P --auto A --claims C --locks DIR --exclude-folder NAME... --n 100 --out rows.tsv
       [--skips skips.tsv]
"""
import argparse, collections, csv, glob, os, re, sys
csv.field_size_limit(10**9)
R = '/home/user/FableDecomp'
P = f'{R}/rebuild/candidates/manual'
ap = argparse.ArgumentParser()
ap.add_argument('--unmatched', default=f'{P}/UNMATCHED_ROWS.tsv')
ap.add_argument('--ps1', default=f'{R}/rebuild/build_candidates.ps1')
ap.add_argument('--auto', default=f'{R}/rebuild/oracles/auto-re-candidates.tsv')
ap.add_argument('--claims', default=f'{P}/COORDINATION_CLAIMS.tsv')
ap.add_argument('--locks', default=None)
ap.add_argument('--exclude-folder', action='append', default=[], help='pending/*codex* folders to ignore (replay only)')
ap.add_argument('--n', type=int, default=100)
ap.add_argument('--out', required=True)
ap.add_argument('--skips', default=None)
ap.add_argument('--include', default=None, help='pass 29: rows given to this pass whatever the rules say (address<TAB>draft override<TAB>grade override<TAB>note); they go first')
ap.add_argument('--order', choices=['closest','longest'], default='closest', help='pass 28: longest = retail length descending')
a = ap.parse_args()
# the package folder this script lives in is itself a *codex* folder: never let it exclude its own rows
a.exclude_folder.append(os.path.basename(os.path.dirname(os.path.dirname(os.path.abspath(__file__)))))
HEX = re.compile(r'\b(?:0x)?([0-9A-Fa-f]{8})\b')

# --- Codex has had it
codex = collections.defaultdict(set)
for d in sorted(glob.glob(f'{P}/pending/*codex*')):
    if os.path.basename(d) in a.exclude_folder: continue
    for root, dirs, files in os.walk(d):
        for n in dirs + files:
            m = re.match(r'([0-9A-Fa-f]{8})(?:[._-]|$)', n)
            if m: codex[m.group(1).lower()].add(os.path.basename(d) + ':name')
        for f in files:
            if not f.endswith('.tsv') or f.startswith('NAME_FIXES'): continue   # NAME_FIXES first columns are callees, not rows
            for l in open(os.path.join(root, f), encoding='utf-8', errors='replace'):
                c = l.split('\t', 1)[0].strip().lower().replace('0x', '')
                if re.fullmatch(r'[0-9a-f]{8}', c): codex[c].add(os.path.basename(d) + ':' + f)

# --- landed
t = open(a.ps1, encoding='utf-8-sig').read()
landed = set(x.lower() for x in re.findall(r"Address\s*=\s*'([0-9A-Fa-f]{8})'", t))
landed |= set(r['address'].lower() for r in csv.DictReader(open(a.auto, encoding='utf-8-sig'), delimiter='\t'))

# --- claims (newest line per address wins)
claim = {}
for r in csv.DictReader(open(a.claims, encoding='utf-8-sig'), delimiter='\t'):
    for x in HEX.findall(r.get('item') or ''):
        claim[x.lower()] = (r.get('status') or '').strip()
def locked(addr):
    if not a.locks: return None
    p = f'{a.locks}/{addr}/owner'
    if not os.path.exists(p): return None
    o = open(p).read().strip()
    return None if o == 'released' else o

# --- Ego_d names
egod = collections.defaultdict(list)       # qualified name -> [object]
bytail = collections.defaultdict(list)     # 'Class::Method' / 'Method' tail -> [(qualified, object)]
for l in open(f'{R}/ghidra_out/egod_carved_functions.tsv', encoding='utf-8', errors='replace'):
    if l.startswith('#'): continue
    f = l.rstrip('\n').split('\t', 4)
    if len(f) < 5 or f[0] == 'object': continue
    q, ob = f[4], f[0]
    if ob not in egod[q]: egod[q].append(ob)
    parts = q.split('<')[0].split('::')
    for k in range(1, min(3, len(parts)) + 1):
        tail = '::'.join(q.split('::')[-k:]) if '<' not in q else q
        bytail[tail].append((q, ob))
PLACEHOLDER = re.compile(r'(^vfunc|^sub_|^FUN_|Unproven|_[0-9a-f]{8}\b|^_v\d|_v\d$|^DIFFER)', re.I)
def demangle_leaf(leaf):
    """?Method@Class@NS@@... -> NS::Class::Method (operators / special names skipped)."""
    m = re.match(r'\?([A-Za-z_]\w*)@((?:[A-Za-z_]\w*@)*)@', leaf or '')
    if not m: return None
    scopes = [s for s in m.group(2).split('@') if s]
    return '::'.join(list(reversed(scopes)) + [m.group(1)])
def leaf_of(path):
    try:
        for x in open(path, encoding='utf-8', errors='replace'):
            if x.startswith('// Leaf:'):
                s = x.split(); return s[2] if len(s) > 2 else None
    except OSError: pass
    return None
def egod_lookup(cands):
    for q in cands:
        if not q or PLACEHOLDER.search(q.split('::')[-1]): continue
        if q in egod: return q, egod[q][0]
        hits = sorted(set(bytail.get(q, [])))
        if len(set(h[0] for h in hits)) == 1: return hits[0]
    return '-', '-'

rows, skips = [], collections.Counter()
INC = {}
if a.include:
    for l in open(a.include, encoding='utf-8'):
        if not l.strip() or l.startswith('#'): continue
        f = (l.rstrip('\n').split('\t') + ['', '', ''])[:4]
        INC[f[0].strip().lower()] = f[1:]
skipped_detail = []
for r in csv.DictReader(open(a.unmatched, encoding='utf-8-sig'), delimiter='\t'):
    ad = r['address'].strip().lower()
    st = r['status']
    if st != 'parked' and not (ad in INC and st.startswith('parked')): continue
    def skip(why): skips[why] += 1; skipped_detail.append((ad, why))
    if ad.startswith('012'): skip('funclet'); continue
    grade_txt = r['best_grade_on_record'] or ''
    inc = INC.get(ad)
    if inc and inc[1]: grade_txt = inc[1] + ' (' + grade_txt + ')'
    dm = re.search(r'DIFFER[( ](\d+)v(\d+)\)?' if inc else r'DIFFER\((\d+)v(\d+)\)', grade_txt)
    if not dm: skip('no DIFFER grade on record'); continue
    path = (inc[0] if inc and inc[0] else '') or r['closest_version'] or ''
    if path.endswith('.test.cpp') and os.path.exists(f'{R}/{path[:-9]}.cpp'): path = path[:-9] + '.cpp'   # pass 28: some records name the test
    if not path.endswith('.cpp') or not os.path.exists(f'{R}/{path}'): skip('no usable draft path'); continue
    if inc: pass
    elif ad in codex: skip('a Codex pass had it'); skipped_detail[-1] = (ad, 'a Codex pass had it: ' + ','.join(sorted(codex[ad]))); continue
    if inc: pass
    elif 'codex' in path.lower(): skip('draft sits in a Codex folder'); continue
    if inc: pass
    elif re.search(r'codex', st + ' ' + grade_txt, re.I): skip('status/grade text mentions Codex'); continue
    if ad in landed: skip('landed'); continue
    if ad in claim and claim[ad].lower() not in ('released', 'done'): skip(f'claimed ({claim[ad]})'); continue
    lk = None if inc else locked(ad)
    if lk: skip(f'locked ({lk})'); continue
    ours, ret = int(dm.group(1)), int(dm.group(2))
    if ret == 0: skip('retail length 0 on record (broken record)'); continue   # pass 29
    leaf = leaf_of(f'{R}/{path}')
    mod, name = r['module'], r['name']
    cands = []
    dl = demangle_leaf(leaf)
    if dl: cands.append(dl)
    if name.startswith('?'): cands.append(demangle_leaf(name))
    elif '::' in name: cands.append(name)
    elif mod not in ('_global', '_gapscan', '?', ''): cands.append(f'{mod}::{name}')
    else: cands.append(name)
    en, ob = egod_lookup(cands)
    summ = f"Best on record DIFFER({ours}v{ret}) ({r['grades_on_record'] or 0} grades). Record: {grade_txt}"
    # NAME CONFLICT: the draft's Leaf and the manifest label name different functions
    if leaf and leaf.startswith('?'):
        if name.startswith('?'): conflict = leaf != name
        else:
            sm = re.match(r'\?\?([0-9A-Z_])((?:[A-Za-z_]\w*@)+)@', leaf)
            if sm:   # special names: ??0 ctor, ??1 dtor, ??R operator()
                sc = [x for x in sm.group(2).split('@') if x]
                lm = {'0': sc[0], '1': '~' + sc[0], 'R': 'operator()'}.get(sm.group(1))
                lq = '::'.join(reversed(sc)) if sm.group(1) in '01' else '::'.join(reversed(sc))
                lq = (lq + '::' + lm) if lm and sm.group(1) in '01R' else None
            elif dl:
                lq = dl; lm = dl.split('::')[-1]
            else:   # a truncated Leaf (`?Allocate@CReverbDelay`, `?OnAppearanceDraw@`): method token, then the class token if any
                tk = [x for x in re.split(r'@', leaf.lstrip('?')) if x]
                lm = tk[0] if tk and re.fullmatch(r'[A-Za-z_]\w*', tk[0]) else None
                lq = (tk[1] + '::' + tk[0]) if lm and len(tk) > 1 and re.fullmatch(r'[A-Za-z_]\w*', tk[1]) else lm
            q = name if '::' in name else (f'{mod}::{name}' if mod not in ('_global', '_gapscan', '?', '') else name)
            conflict = lm is not None and (lm != q.split('::')[-1] or ('::' in q and lq and '::' in lq and lq.split('::')[-2] != q.split('::')[-2]))
        if conflict:
            summ += (f" NAME CONFLICT: the draft Leaf `{leaf}` and the manifest label `{mod}::{name}` name different functions; "
                     "settle the name from the PDB lists before matching.")
    if en == '-': summ += ' No Ego_d procedure record was found for this name.'
    if inc and inc[2]: summ = 'GIVEN TO THIS PASS: ' + inc[2] + ' ' + summ
    rows.append((-1 if inc else abs(ours - ret), ret, ad, f'DIFFER({ours}v{ret})', path, en, ob, summ))
rows.sort(key=(lambda x: (x[0] >= 0, -x[1], x[2])) if a.order == 'longest' else (lambda x: (x[0], x[1], x[2])))
sel = rows[:a.n]
with open(a.out, 'w') as o:
    for g, ret, ad, gr, path, en, ob, summ in sel:
        if g < 0: g = 0
        o.write('\t'.join([ad, str(ret), gr, path, en, ob, summ.replace('\t', ' '), '']) + '\n')
print(f'eligible {len(rows)}; selected {len(sel)}; gap range {sel[0][0]}-{sel[-1][0]}; bytes {sum(x[1] for x in sel)}; '
      f'sizes {min(x[1] for x in sel)}-{max(x[1] for x in sel)}; with Ego_d {sum(1 for x in sel if x[5] != "-")}; '
      f'name conflicts {sum(1 for x in sel if "NAME CONFLICT" in x[7])}', file=sys.stderr)
for k, v in skips.most_common(): print(f'  skipped {v}: {k}', file=sys.stderr)
if a.skips:
    with open(a.skips, 'w') as o:
        for ad, why in skipped_detail: o.write(f'{ad}\t{why}\n')
