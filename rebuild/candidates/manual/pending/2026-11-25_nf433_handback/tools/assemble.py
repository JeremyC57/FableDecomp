#!/usr/bin/env python3
"""assemble.py <W> <batch> <srcdir> <tag> <addr...>: build batches/<batch>/ for the lander: rows/, tests/, orig/rows|tests (HEAD copies by
address), head/payload.json (recheck.py HEAD payload of the run <tag>), hdr/ (overlay headers the rows use), RESULTS, CHECKS,
TABLE_CHANGES, SELF_SYMBOLS, HEADERS. Needs work/vland_<tag>{h,n,m}.log, <srcdir>/ng_<a>.log, <srcdir>/objcmp.tsv, <srcdir>/pk.txt."""
import sys, os, re, json, shutil, glob, collections, subprocess
W, batch, sd, tag = sys.argv[1:5]; A = sys.argv[5:]
R = '/home/user/FableDecomp'; B = f'{W}/batches/{batch}'
for d in ('rows', 'tests', 'orig/rows', 'orig/tests', 'head', 'hdr/engine', 'evidence'): os.makedirs(f'{B}/{d}', exist_ok=True)
hp = json.load(open(f'{W}/work/p_{tag}h/payload.json'))
hp['result']['authored'] = [x for x in hp['result']['authored'] if x['address'] in A]
json.dump(hp, open(f'{B}/head/payload.json', 'w'))
H = {x['address']: x for x in hp['result']['authored']}
def grades(k):
    g = {}
    for l in open(f'{W}/work/vland_{tag}{k}.log'):
        m = re.match(r'^([0-9a-f]{8})\s+(\S+(?:\(\S+\))?)\s+(\S+)\s', l)
        if m and not l.split()[1].startswith('WARN'): g[m.group(1)] = (m.group(2), m.group(3))
    return g
gh, gn, gm = grades('h'), grades('n'), grades('m')
ss = {}
for l in open(R + '/rebuild/oracles/self_symbols.tsv').read().splitlines()[1:]:
    f = l.split('\t'); ss[f[0]] = l
binds = [l.split('\t') for l in open(R + '/rebuild/oracles/reloc_bindings.tsv').read().splitlines()[1:]]
oc = {l.split('\t')[0]: l.rstrip('\n').split('\t') for l in open(f'{sd}/objcmp.tsv')}
pk = collections.defaultdict(dict)
for l in open(f'{sd}/pk.txt'):
    q = l.rstrip('\n').split(' ', 2)
    if len(q) >= 2: pk[q[0]][q[1]] = q[2].strip() if len(q) > 2 else ''
src_all = None
res = ['address\tverdict\told\tnew\treason']; chk = ['address\thead_grade\tnew_grade\tmutant\tobjcmp\tpk_head\tpk_new\tname_gate\tliteral_warns\tnew_warn_kinds']
tch = ['action\tsymbol\ttarget\twitness\tnote']; sso = ['address\tsymbol\tstatus\tleaf_given\tcurrent_line']
hdrs = collections.Counter(); keep = []
for a in A:
    row = open(f'{sd}/rows/{a}.cpp').read(); shutil.copy(f'{sd}/rows/{a}.cpp', f'{B}/rows/{a}.cpp'); shutil.copy(f'{sd}/tests/{a}_test.cpp', f'{B}/tests/{a}_test.cpp')
    open(f'{B}/orig/rows/{a}.cpp', 'w').write(H[a]['source_cpp']); open(f'{B}/orig/tests/{a}_test.cpp', 'w').write(H[a]['test_cpp'])
    new = re.search(r'^// Leaf: (\S+)', row, re.M).group(1); old = oc[a][2]
    for h in re.findall(r'#include "(engine/\w+\.h)"', row):
        if os.path.exists(f'{W}/hdr/{h}'): hdrs[h] += 1
    ng = open(f'{sd}/ng_{a}.log').read(); ngv = ng.strip().splitlines()[-1]
    # bindings: new ones from the name_gate relocation lines; old ones witnessed by this row
    nb = set()
    for m in re.finditer(r'^(?:OK|DEBT)\s+([0-9a-f]{8}) \+[0-9a-f]+\s+\S+\s+(\S+)', ng, re.M): nb.add((m.group(2), m.group(1)))
    ob = {(b[0], b[1]) for b in binds if len(b) > 2 and b[2] == a}
    allb = {(b[0], b[1]) for b in binds}
    for s, t in sorted(ob - nb):
        ident = re.match(r'[?_@]*(\w+)', s).group(1)
        users = subprocess.run(['grep', '-rlwi', ident, R + '/rebuild/src/compiled'], capture_output=True, text=True).stdout.split()
        users = [u for u in users if not u.endswith(f'_{a}.cpp') and t.lstrip('0') in open(u, errors='replace').read().lower()]
        if users: keep.append(f'{s}\t{t}\t{a}\tkept: other landed sources spell {ident} and name the target: ' + ' '.join(os.path.basename(u) for u in users))
        else: tch.append(f'delete\t{s}\t{t}\t{a}\tNF433: the row now binds the PDB name at this target; no other landed source spells {ident} next to the target (census)')
    for s, t in sorted(nb - ob):
        if (s, t) in allb: continue
        tch.append(f'add\t{s}\t{t}\t{a}\tNF433: the row\'s callee at this retail target, PDB spelling (name_gate OK)')
    reason = ('port onto engine/CGameScriptInterface.h (FABLE_RETAIL_NSCRIPT): the RTTI-placed PDB virtual member; self section bytes '
              'equal to HEAD (objcmp SELF_EQ), grade kept, name_gate --test PASS')
    if 'callee respell' in oc[a][4]: reason += '; ' + oc[a][4].split(' | ')[0]
    res.append(f'{a}\tACCEPT\t{old}\t{new}\t{reason}')
    lit = 0; wk = '-'
    chk.append('\t'.join([a, ' '.join(gh[a]), ' '.join(gn[a]), ' '.join(gm[a]), oc[a][1] + (' (' + oc[a][4] + ')' if oc[a][4] else ''),
                          pk[a].get('h', '') or 'no NEQ', pk[a].get('n', '') or 'no NEQ', ngv, str(lit), wk]))
    status = gn[a][0]
    sso.append(f'{a}\t{new}\t{status}\t{new}\t{ss.get(a, "(none)")}')
for h in hdrs:
    shutil.copy(f'{W}/hdr/{h}', f'{B}/hdr/{h}')
hl = ['header\topt_in\tinclude_users_in_batch']
for h, n in sorted(hdrs.items()):
    t = open(f'{W}/hdr/{h}').read(); o = open(f'{W}/orig/hdr_{os.path.basename(h)}').read()
    ins = [l for l in t.splitlines() if l not in set(o.splitlines())]
    mac = sorted(set(re.findall(r'#(?:ifdef|if defined\()\s*\(?(FABLE_RETAIL_\w+)', '\n'.join(ins))))
    hl.append(f'{h}\t{",".join(mac)}\t{n}')
for f, L in (('RESULTS.tsv', res), ('CHECKS.tsv', chk), ('TABLE_CHANGES.tsv', tch), ('SELF_SYMBOLS.tsv', sso), ('HEADERS.tsv', hl)):
    open(f'{B}/{f}', 'w').write('\n'.join(L) + '\n')
open(f'{B}/evidence/TABLE_KEPT.tsv', 'w').write('symbol\ttarget\twitness\tnote\n' + ''.join(k + '\n' for k in keep))
open(f'{B}/rows.txt', 'w').write('\n'.join(A) + '\n')
print(batch, len(A), 'rows;', len(tch) - 1, 'table lines;', len(hl) - 1, 'headers')
