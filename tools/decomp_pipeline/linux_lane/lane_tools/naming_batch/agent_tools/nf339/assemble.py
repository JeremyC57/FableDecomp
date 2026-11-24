# assemble.py <bN> <v_tag> <meta.tsv> [table.tsv]: land_scripted run dir batches/<bN>/ from a ver.py proof.
#   meta.tsv: addr <TAB> unit <TAB> target(s) (comma list; '-' for a self row) <TAB> note (written into the RESULTS reason)
#   table.tsv: TABLE_CHANGES lines (action symbol target witness note), header added
# Copies rows/<a>.cpp (+ rows/<a>_test.cpp -> tests/), orig/<a>.cpp (+ orig/tests/), head/payload.json (+ HEAD_FROM_REPO.txt for rows
# whose landed source carries a retail-bytes comment), RESULTS.tsv (ACCEPT rows only; binder rows carry '; target <addr>').
import os, sys, json, shutil, re
A = '/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF339'
b, tag, metaf = sys.argv[1], sys.argv[2], sys.argv[3]
B = f'{A}/batches/{b}'; V = f'{A}/work/v_{tag}'
for d in ('rows', 'orig', 'orig/tests', 'tests', 'head'): os.makedirs(f'{B}/{d}', exist_ok=True)
meta = {}
for l in open(metaf).read().splitlines():
    if l.strip() and not l.startswith('#'):
        f = l.split('\t'); meta[f[0]] = f
res = [l.split('\t') for l in open(f'{V}/RESULTS.tsv').read().splitlines()[1:] if l]
acc = [r for r in res if r[1] == 'ACCEPT' and r[0] in meta]
out, units, hfr = [], [], []
hp = json.load(open(f'{V}/head/payload.json'))
keep = [x for x in hp['result']['authored'] if x['address'] in {r[0] for r in acc}]
for x in keep:
    if re.search(r'Retail bytes?:|retail bytes', x['source_cpp']): hfr.append(x['address'])
json.dump({'result': {'authored': keep}}, open(f'{B}/head/payload.json', 'w'))
open(f'{B}/head/HEAD_FROM_REPO.txt', 'w').write(''.join(a + '\n' for a in hfr))
for r in acc:
    a = r[0]; m = meta[a]
    shutil.copy(f'{A}/rows/{a}.cpp', f'{B}/rows/{a}.cpp'); shutil.copy(f'{A}/orig/{a}.cpp', f'{B}/orig/{a}.cpp')
    if os.path.exists(f'{A}/rows/{a}_test.cpp'):
        shutil.copy(f'{A}/rows/{a}_test.cpp', f'{B}/tests/{a}_test.cpp'); shutil.copy(f'{A}/orig/tests/{a}_test.cpp', f'{B}/orig/tests/{a}_test.cpp')
    reason = r[4].replace('; symdelta []', '') + (f'; target {m[2]}' if m[2] != '-' else '') + (f' [{m[3]}]' if len(m) > 3 and m[3] else '')
    out.append([a, 'ACCEPT', r[2], r[3], reason]); units.append([a, m[1]])
open(f'{B}/RESULTS.tsv', 'w').write('address\tverdict\told\tnew\treason\n' + ''.join('\t'.join(x) + '\n' for x in out))
open(f'{B}/UNITS.tsv', 'w').write('address\tunit\n' + ''.join('\t'.join(x) + '\n' for x in units))
open(f'{B}/rows.txt', 'w').write(''.join(x[0] + '\n' for x in out))
if len(sys.argv) > 4:
    open(f'{B}/TABLE_CHANGES.tsv', 'w').write('action\tsymbol\ttarget\twitness\tnote\n' + open(sys.argv[4]).read())
print(b, 'rows', len(out), 'head_from_repo', hfr, 'skipped', [r[0] for r in res if r[0] not in {x[0] for x in out}])
