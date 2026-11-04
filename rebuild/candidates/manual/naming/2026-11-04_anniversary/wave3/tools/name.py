#!/usr/bin/env python3
"""Wave 3 naming: registration (string, Alloc) -> class vtable -> PDB-checked nested class name -> slot rows."""
import sys, os, re, csv, json, struct, collections
R = '/home/user/FableDecomp'
sys.path.insert(0, R + '/tools/decomp_pipeline/linux_lane/lane_tools/bounds')
from extent import Exe
E = Exe()
W = os.path.dirname(os.path.abspath(__file__)) + '/..'
J = json.load(open(W + '/scan.json'))

cat = {}
for l in open(R + '/rebuild/oracles/auto-re-candidates.tsv', encoding='utf-8'):
    c = l.rstrip('\n').split('\t')
    if len(c) > 1: cat[c[0]] = c[1]
man = {}
for r in csv.DictReader(open(R + '/rebuild/manifest/functions.tsv', encoding='utf-8-sig'), delimiter='\t'):
    m = r['module']; n = r['name']
    man[r['address']] = (n if m in ('_global', '_gapscan', '', '_unattributed') or n.startswith(m + '::') else m + '::' + n, m, n)
landed = set(re.findall(r"Address = '([0-9a-f]{8})'", open(R + '/rebuild/build_candidates.ps1', encoding='utf-8').read()))

pdb = set()
for f in ('ghidra_out/fablewin_pdb_names.tsv', 'ghidra_out/egor_pdb_names.tsv'):
    for l in open(R + '/' + f, errors='replace'):
        c = l.rstrip('\n').split('\t')
        if len(c) > 1: pdb.add(c[1])
ALLOC_RE = re.compile(r'^\?Alloc@([^@]+)@([^@]+)@NScript@@SI\?AV\?\$CCountedPointer@VCEntityScriptBase@@')
pdb_alloc = collections.defaultdict(set)      # outer -> inners
pdb_alloc_by_inner = collections.defaultdict(set)
for s in pdb:
    m = ALLOC_RE.match(s)
    if m:
        pdb_alloc[m.group(2)].add(m.group(1)); pdb_alloc_by_inner[m.group(1)].add(m.group(2))
pfx = collections.defaultdict(set)  # "Inner@Outer" -> set of member tags
for s in pdb:
    m = re.match(r'^\?(\w+)@([^@]+)@([^@]+)@NScript@@', s)
    if m: pfx[m.group(2) + '@' + m.group(3)].add(m.group(1))
    m = re.match(r'^\?\?(0|1|_G|_E)([^@]+)@([^@]+)@NScript@@', s)
    if m: pfx[m.group(2) + '@' + m.group(3)].add('??' + m.group(1))

PH = re.compile(r"sub_|CVtable_|CUnproven|vfunc_|^\?|_gapscan|^None$|Alloc_CVtable|^_unattributed::|_[0-9a-f]{8}$|^FUN_|^thunk_FUN")
def curname(a):
    return cat.get(a) or (man[a][0] if a in man else None) or 'None'
def is_ph(n): return bool(PH.search(n))

ann = {}
for r in csv.DictReader(open(R + '/rebuild/candidates/manual/naming/2026-11-04_anniversary/candidate_names.tsv'), delimiter='\t'):
    m = re.search(r'vtable 0x([0-9A-Fa-f]+)', r['source_evidence'])
    if m: ann[int(m.group(1), 16)] = (r['name'], r['confidence'])
for r in csv.DictReader(open(R + '/rebuild/candidates/manual/naming/2026-11-04_anniversary/vt_confirmations.tsv'), delimiter='\t'):
    ann.setdefault(int(r['tlc_vtable'], 16), (r['anniversary_rtti_class'], 'confirmed'))

raw = E.d
def refs(va): return raw.count(struct.pack('<I', va))

pfx_outer = collections.defaultdict(set)
for s_ in pdb:
    m_ = re.match(r'^\?(\w+)@([^@]+)@NScript@@[^@]', s_)
    if m_: pfx_outer[m_.group(2)].add(m_.group(1))
    m_ = re.match(r'^\?\?(0|1|_G|_E)([^@]+)@NScript@@[^@]', s_)
    if m_: pfx_outer[m_.group(2)].add('??' + m_.group(1))
LAY = {0: 'sdd', 1: 'Main', 2: 'Init', 3: 'GetParentScript', 4: 'OnPersist', 5: 'OnPredicateFail', 6: 'OnInterrupted'}
def cls_has(chain, meth):
    tags = pfx.get(chain, set())
    if meth == 'sdd': return '??_G' in tags or '??1' in tags
    return meth in tags

RD_LO, RD_HI = [(x[1], x[1] + x[2]) for x in E.secs if x[0] == '.rdata'][0]
def rdata_refs(va):
    p = struct.pack('<I', va); out = []; o = E.off(RD_LO); end = o + (RD_HI - RD_LO); i = o
    while True:
        i = raw.find(p, i, end)
        if i < 0: break
        out.append(RD_LO + i - o); i += 1
    return out

def strip_ns(n):
    return n[len('NScript::'):] if n.startswith('NScript::') else n

def encl_outer(func):
    """(outer, method, source) from the catalog / manifest name of the enclosing function."""
    a = '%08x' % func
    for src, n in (('catalog', cat.get(a)), ('manifest', man[a][0] if a in man else None)):
        if n and not is_ph(n):
            m = re.match(r'^(\w+)::(\w+)$', strip_ns(n))
            if m: return m.group(1), m.group(2), '%s %s' % (src, n)
    return None, None, 'enclosing %s unnamed (catalog %s)' % (a, cat.get(a))

def script_vt_context(func):
    """script vtable(s) holding the enclosing function, with the slot index and the catalog names of the other slots' classes."""
    out = []
    for r in rdata_refs(func):
        for slot in (2, 3, 1):
            vt = r - 4 * slot
            names = []
            for k in range(0, 5):
                f = E.dword(vt + 4 * k)
                if f is None or not (E.tlo <= f < E.thi): break
                n = curname('%08x' % f)
                if not is_ph(n) and k != slot and refs(f) == 1: names.append((k, strip_ns(n)))
            outs = {re.match(r'^(\w+)::', n).group(1) for k, n in names if re.match(r'^(\w+)::', n) and k != slot}
            if outs:
                out.append((vt, slot, sorted(outs))); break
    return out

AR_IN = pdb_alloc_by_inner
groups = collections.defaultdict(list)
for s in J['sites']: groups[s['func']].append(s)

def tsvq(x): return str(x).replace('\t', ' ').replace('\n', ' ')

review = []; labels = {}; class_rows = []; disagreements = []
vt_named = {}
for func, ss in sorted(groups.items()):
    eo, em, esrc = encl_outer(func)
    inter = None
    for s in ss:
        o = AR_IN.get('C' + s['name'])
        if o: inter = set(o) if inter is None else inter & o
    inter = inter or set()
    vctx = script_vt_context(func)
    vouts = set(o for _, _, outs in vctx for o in outs)
    outer = None; osrc = []
    if eo and eo in inter:
        outer = eo; osrc = [esrc, 'PDB Alloc set of %s holds the registered names' % eo]
    elif eo and not inter:
        # names don't match any PDB Alloc: keep the enclosing name, inner resolution decides
        outer = eo; osrc = [esrc]
    elif eo and eo not in inter:
        disagreements.append(('%08x' % func, 'enclosing name %s but PDB outers of the registered names %s; script vtable context %s' % (eo, sorted(inter), vctx)))
        if len(inter) == 1 and vouts == inter:
            outer = next(iter(inter)); osrc = ['PDB: the only NScript outer with Alloc@C<string> for all %d registered names (enclosing name %s is wrong)' % (len(ss), esrc)]
    elif len(inter) == 1:
        outer = next(iter(inter)); osrc = ['%s; PDB: the only NScript outer with Alloc@C<string> for all %d registered names' % (esrc, len(ss))]
    if outer and vouts:
        if vouts == {outer}: osrc.append('script vtable %08x (enclosing = slot %d) other slots named %s' % (vctx[0][0], vctx[0][1], outer))
        else:
            disagreements.append(('%08x' % func, 'script vtable context %s vs outer %s' % (vctx, outer)))
    ok_outer = outer is not None and len(osrc) >= 2
    pinners = pdb_alloc.get(outer, set()) if outer else set()
    # direct matches, then elimination for Allocs whose strings have no PDB Alloc (shared Allocs registered under several names)
    direct = {}
    for s in ss:
        cand = 'C' + s['name']
        if cand in pinners: direct[s['alloc']] = (cand, 'OK')
        else:
            lc = [i for i in pinners if i.lower() == cand.lower()]
            if len(lc) == 1: direct[s['alloc']] = (lc[0], 'OK_CASE')
    unm_allocs = sorted({s['alloc'] for s in ss} - set(direct))
    unm_inners = set(pinners) - {v[0] for v in direct.values()}
    elim = {}; elim_note = {}
    for al in list(unm_allocs):
        strs = [s['name'] for s in ss if s['alloc'] == al]
        c = [u for u in unm_inners if all(x.endswith(u[1:]) for x in strs)]
        if len(c) == 1:
            elim[al] = c[0]; unm_inners.discard(c[0]); unm_allocs.remove(al)
            elim_note[al] = 'strings %s have no PDB Alloc; they share Alloc %08x; every string ends with "%s", the only unmatched PDB entity Alloc of %s with that suffix' % (strs, al, c[0][1:], outer)
    if len(unm_allocs) == 1 and len(unm_inners) == 1:
        al = unm_allocs[0]; elim[al] = next(iter(unm_inners))
        strs = [s['name'] for s in ss if s['alloc'] == al]
        elim_note[al] = 'strings %s have no PDB Alloc; they share Alloc %08x; after the %d direct matches %s has exactly one PDB entity Alloc left (%s)' % (strs, al, len(direct), outer, elim[al])
    for s in ss:
        a = '%08x' % s['alloc']; A = J['allocs'][a]; vt = A['vt']
        inner = None; verdict = None; note = []
        if not outer: verdict = 'NO_OUTER'
        elif s['alloc'] in direct: inner, verdict = direct[s['alloc']]
        elif s['alloc'] in elim:
            inner = elim[s['alloc']]; verdict = 'STRING_MISMATCH_ELIM'; note.append(elim_note[s['alloc']])
        else:
            verdict = 'PDB_MISMATCH'; note.append('unmatched PDB inners of %s: %s' % (outer, sorted(unm_inners)))
        if inner and not ok_outer:
            verdict = 'OUTER_ONE_SOURCE'
        rv = dict(site='%08x' % s['site'], func='%08x' % func, string=s['name'], alloc=a, vt='%08x' % vt if vt else '', ctor='%08x' % A['ctor'] if A['ctor'] else '',
                  size=A['size'], outer=outer or '', inner=inner or '', verdict=verdict, outer_src='; '.join(osrc), note='; '.join(note))
        review.append(rv)

if __name__ == '__main__' and os.environ.get('STAGE') == '1':
    print(collections.Counter(r['verdict'] for r in review))
    for r in review:
        if r['verdict'] not in ('OK',): print(r)
    for d in disagreements: print('DIS', d)

# ---------------- stage 2: labels
MAIN_LIT = 0x012C2F94  # "Main" literal pushed by every RegisterMain
def script_vtable(func):
    """(vt, slot) of the 5-slot level-script vtable holding the enclosing function; slot 1 must be a RegisterMain
    (its body pushes the "Main" literal 0x012C2F94)."""
    for r in rdata_refs(func):
        for slot in (2, 3):
            vt = r - 4 * slot; rm = E.dword(vt + 4)
            if rm and E.tlo <= rm < E.thi and struct.pack('<I', MAIN_LIT) in E.read(rm, 0x90):
                return vt, slot
    return None, None

name_holders = collections.defaultdict(set)
for a, n in cat.items():
    if a != 'address': name_holders[n].add(a)
def full_cur(a):
    n = curname(a)
    return n

def pdb_has_full(full, meth):
    """does the PDB name set list `meth` for the class spelled by catalog name `full` (NScript::Outer[::Inner])?"""
    parts = strip_ns(full).split('::')
    if len(parts) < 2: return False
    cls = parts[:-1]
    chain = '@'.join(reversed(cls))
    return any(s.startswith('?%s@%s@NScript@@' % (meth, chain)) for s in pdb_by_meth.get(meth, ()))
pdb_by_meth = collections.defaultdict(list)
for s in pdb:
    m = re.match(r'^\?(\w+)@', s)
    if m: pdb_by_meth[m.group(1)].append(s)

VIRT_ACC = set('EFMNUV')   # MSVC access codes of virtual members (private/protected/public virtual)
def pdb_kind(cur):
    """for a catalog name X::...::meth return (found, virtual) from the PDB name sets (any nesting ending in X)."""
    parts = strip_ns(cur).split('::')
    if len(parts) < 2: return None, None
    meth = parts[-1]; chain = '@'.join(reversed(parts[:-1]))
    found = False; virt = False
    for s_ in pdb_by_meth.get(meth, ()):
        if s_.startswith('?%s@%s@' % (meth, chain)):
            found = True
            m_ = re.match(r'^\?\w+@(?:[^@]+@)+@([A-Z])', s_)
            if m_ and m_.group(1) in VIRT_ACC: virt = True
    return found, virt

proposals = []
def emit(addr, newname, meth, ev, kind):
    proposals.append((kind, addr, newname, meth, ev))

def judge(addr, newname, meth, owner):
    """-> (action, note). owner: (stripped class::meth) -> address, for every slot this wave identified."""
    cur = curname(addr)
    if cur == newname: return 'SAME', ''
    if is_ph(cur): return 'RENAME', 'was %s' % cur
    curn = strip_ns(cur); newn = strip_ns(newname)
    if curn == newn: return 'RENAME', 'was %s (namespace spelling)' % cur
    if newn.endswith('::' + curn): return 'RENAME_QUAL', 'was under-qualified %s' % cur
    why = []
    others = name_holders.get(cur, set()) - {addr}
    if others: why.append('current name also held by %s' % ','.join(sorted(others)[:3]))
    oa = owner.get(curn) or owner_short.get(curn)
    if oa and oa != addr: why.append('%s is at %s (its own vtable slot)' % (curn, oa))
    found, virt = pdb_kind(cur)
    if meth not in ('ctor',):
        if found is False: why.append('PDB has no such member')
        elif found and not virt: why.append('PDB member is non-virtual, cannot be a vtable slot')
    if why: return 'RENAME_WRONG', 'replaces %s: %s' % (cur, '; '.join(why))
    return 'DISAGREE', 'current %s kept (not provably wrong)' % cur

LAYS = {0: 'sdd', 1: 'RegisterMain', 2: 'Main', 3: 'Init', 4: 'OnPersist'}
done_vt = {}
named_vts = {}
slotlog = []
for rv in review:
    if rv['verdict'] not in ('OK', 'OK_CASE', 'STRING_MISMATCH_ELIM'): continue
    outer, inner, vt = rv['outer'], rv['inner'], int(rv['vt'], 16)
    full = 'NScript::%s::%s' % (outer, inner); chain = '%s@%s' % (inner, outer)
    if vt in done_vt:
        if done_vt[vt] != full: disagreements.append((rv['vt'], 'vtable named twice %s / %s' % (done_vt[vt], full)))
        continue
    done_vt[vt] = full; named_vts[vt] = full
    an = ann.get(vt)
    anote = ''
    if an:
        if an[0] == full: anote = 'Anniversary RTTI agrees (%s)' % an[1]
        else:
            anote = 'Anniversary RTTI says %s (%s)' % (an[0], an[1]); disagreements.append((rv['vt'], 'Anniversary %s (%s) vs registration %s' % (an[0], an[1], full)))
    rv['anniv'] = anote
    base = 'reg %s "%s" in %s; Alloc %s; vtable %s' % (rv['site'], rv['string'], rv['func'], rv['alloc'], rv['vt'])
    if rv['verdict'] == 'STRING_MISMATCH_SOLE': base += ' (string differs: sole PDB entity Alloc of %s)' % outer
    tail = (('; ' + anote) if anote else '')
    # Alloc
    alloc_sym = '?Alloc@%s@NScript@@SI' % chain
    n_ref = refs(int(rv['alloc'], 16))
    newstart = '' if rv['alloc'] in man else '; NEW FUNCTION START (absent from manifest; inside %s)' % ('%08x' % max(x for x in E.sstarts if x < int(rv['alloc'], 16)))
    emit(rv['alloc'], full + '::Alloc', 'Alloc', '%s; PDB %s...; refs=%d (registration sites)%s%s' % (base, alloc_sym, n_ref, newstart, tail), 'alloc')
    if rv['ctor'] and '??0' in pfx.get(chain, set()):
        emit(rv['ctor'], full + '::' + inner, 'ctor', '%s; called by the Alloc after operator new; PDB ??0%s@NScript@@; refs(call)=Alloc%s' % (base, chain, tail), 'ctor')
    for slot, meth in LAY.items():
        f = E.dword(vt + 4 * slot); fa = '%08x' % f; n = refs(f)
        has = cls_has(chain, meth)
        newname = full + '::' + ("`scalar deleting destructor'" if meth == 'sdd' else meth)
        if n != 1 or not has:
            slotlog.append(('slot', fa, newname, 'SKIP', 'refs=%d pdb=%s' % (n, has))); continue
        sym = ('??_G/??1%s@NScript@@' % chain) if meth == 'sdd' else '?%s@%s@NScript@@' % (meth, chain)
        emit(fa, newname, meth, '%s slot %d; PDB %s; refs=1%s' % (base, slot, sym, tail), 'slot')

# outer-script vtable slots (level script, 5 slots) for the enclosing functions
outer_done = set(); SVT = {}
for rv in review:
    if rv['verdict'] not in ('OK', 'OK_CASE', 'STRING_MISMATCH_ELIM') or rv['func'] in outer_done: continue
    outer_done.add(rv['func'])
    outer = rv['outer']; svt, eslot = script_vtable(int(rv['func'], 16))
    if svt: SVT[outer] = (svt, eslot, rv['func'])
    if svt is None:
        disagreements.append((rv['func'], 'no RegisterMain-anchored script vtable for the enclosing function')); continue
    n_regs = sum(1 for x in review if x['func'] == rv['func'])
    for slot, meth in LAYS.items():
        f = E.dword(svt + 4 * slot); fa = '%08x' % f; n = refs(f)
        newname = 'NScript::%s::%s' % (outer, "`scalar deleting destructor'" if meth == 'sdd' else meth)
        has = cls_has_outer(outer, meth) if False else None
        tags = pfx_outer.get(outer, set())
        has = ('??_G' in tags or '??1' in tags) if meth == 'sdd' else meth in tags
        if n != 1 or not has:
            slotlog.append(('script', fa, newname, 'SKIP', 'refs=%d pdb=%s' % (n, has))); continue
        ev = ('outer script vtable %08x slot %d (slot 1 RegisterMain pushes "Main" 0x012C2F94); enclosing %s (slot %d) registers %d entity scripts whose PDB Alloc outer is %s; PDB %s; refs=1'
              % (svt, slot, rv['func'], eslot, n_regs, outer, ('??_G/??1%s@NScript@@' % outer) if meth == 'sdd' else '?%s@%s@NScript@@' % (meth, outer)))
        emit(fa, newname, meth, ev, 'script')

# ---------------- judge
owner = {}; owner_short_c = collections.Counter(); owner_short = {}
for kind, addr, newname, meth, ev in proposals:
    if kind in ('slot', 'script', 'alloc'):
        k = strip_ns(newname); owner[k] = addr
        sh = '::'.join(k.split('::')[-2:]); owner_short_c[sh] += 1; owner_short[sh] = addr
for k, c in owner_short_c.items():
    if c > 1: owner_short.pop(k, None)
rows = []; rowkeys = set(); slotlog = list(slotlog)
for kind, addr, newname, meth, ev in proposals:
    act, note = judge(addr, newname, meth, owner)
    slotlog.append((kind, addr, newname, act, note))
    if act.startswith('RENAME') and addr not in rowkeys:
        rowkeys.add(addr); rows.append((addr, newname, ev + ('; ' + note if note else ''), act))

# ---------------- outputs
with open(W + '/labels_entity.tsv', 'w') as f:
    f.write('address\tname\tevidence\n')
    for a, n, ev, act in sorted(rows):
        f.write('%s\t%s\t%s\n' % (a, n, tsvq(ev)))
with open(W + '/review.tsv', 'w') as f:
    cols = ['site', 'func', 'string', 'alloc', 'vt', 'ctor', 'size', 'outer', 'inner', 'verdict', 'outer_src', 'note', 'anniv']
    f.write('\t'.join(cols) + '\n')
    for r in review: f.write('\t'.join(tsvq(r.get(c, '')) for c in cols) + '\n')
with open(W + '/slots.tsv', 'w') as f:
    f.write('kind\taddress\tname\taction\tnote\n')
    for x in slotlog: f.write('\t'.join(tsvq(y) for y in x) + '\n')
with open(W + '/disagreements.tsv', 'w') as f:
    f.write('where\tdetail\n')
    for x in disagreements: f.write('%s\t%s\n' % (x[0], tsvq(x[1])))
json.dump({k: ['%08x' % v[0], v[1], v[2]] for k, v in SVT.items()}, open(W + '/script_vts.json', 'w'), indent=0)
json.dump({'%08x' % k: v for k, v in named_vts.items()}, open(W + '/named_vts.json', 'w'), indent=0)
print(collections.Counter(r['verdict'] for r in review))
print('rows', len(rows), collections.Counter(a for *_, a in rows))
print('slotlog', collections.Counter((k, a) for k, _, _, a, _ in slotlog))
print('named vtables', len(named_vts))
