# mkpayload.py <out_prefix> <overlay_root|-> <rows.txt>: recheck payload + oracle for the listed landed rows, sources and
# tests read from the overlay when present there, else from the repo (same fields as coordinator/mk_recheck_payload.py)
import sys, os, json, re
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from rows import rows, ROOT
out, ov, rf = sys.argv[1:4]
R = rows()
A = sorted(set(l.strip() for l in open(rf) if l.strip()))
cat = {}
for l in open(ROOT + '/rebuild/oracles/auto-re-candidates.tsv'):
    s = l.rstrip('\n').split('\t')
    if len(s) >= 4: cat[s[0].lower()] = l if l.endswith('\n') else l + '\n'
base = {'/O2', '/Oy', '/W3'}
def rd(rel):
    p = os.path.join(ov, rel) if ov != '-' else None
    if not p or not os.path.exists(p): p = os.path.join(ROOT, rel)
    return open(p, encoding='utf-8', errors='surrogateescape').read()
rs = []; miss = []
for a in A:
    d = R.get(a)
    if not d or a not in cat: miss.append(a); continue
    s = rd('rebuild/src/compiled/' + d['Source']); t = rd('rebuild/tests/' + d['TestSource'])
    leaf = [l[9:] for l in s.split('\n') if l.startswith('// Leaf: ')]
    r = {"address": a, "module": d.get('Module', 'm'), "name": cat[a].split('\t')[1], "source_cpp": s, "test_cpp": t,
         "pass_pattern": d.get('PassPattern', a + '_TEST PASS'), "leaf": leaf[0].strip() if leaf else ""}
    fl = [x for x in d.get('CompilerFlags', '').split() if x not in base]
    if fl: r["flags"] = fl
    rs.append(r)
json.dump({"result": {"authored": rs}}, open(out + '_payload.json', 'w'))
open(out + '_oracle.tsv', 'w').write('address\tname\tlength\tbytes\n' + ''.join(cat[r['address']] for r in rs))
print('rows', len(rs), 'missing', miss)
