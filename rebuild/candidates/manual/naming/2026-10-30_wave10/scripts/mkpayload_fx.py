# mkpayload_fx.py: as mkpayload.py, but an unwind-funclet stub's `#include "../src/compiled/<parent>"` is pointed at the
# overlay's edited parent (absolute Z: path) so the funclet is graded against the wave-10 parent; the stub itself is not edited.
# mkpayload_fx.py <out_prefix> <overlay_root|-> <rows.txt>: recheck payload + oracle for the listed landed rows, sources and
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
    def fx(m):
        p = os.path.join(os.path.abspath(ov), 'rebuild/src/compiled', m.group(1))
        return '#include "Z:%s"' % p if os.path.exists(p) else m.group(0)
    s = re.sub(r'#include "\.\./src/compiled/([^"]+)"', fx, s)
    leaf = [l[9:] for l in s.split('\n') if l.startswith('// Leaf: ')]
    r = {"address": a, "module": d.get('Module', 'm'), "name": cat[a].split('\t')[1], "source_cpp": s, "test_cpp": t,
         "pass_pattern": d.get('PassPattern', a + '_TEST PASS'), "leaf": leaf[0].strip() if leaf else ""}
    fo = re.search(r"funclet block offset (0x[0-9a-fA-F]+|\d+)", s)
    if fo and 'Unwind@' in r['name']: r['funclet_offset'] = int(fo.group(1), 0)   # as coordinator recheck.py
    fl = [x for x in d.get('CompilerFlags', '').split() if x not in base]
    if fl: r["flags"] = fl
    rs.append(r)
json.dump({"result": {"authored": rs}}, open(out + '_payload.json', 'w'))
open(out + '_oracle.tsv', 'w').write('address\tname\tlength\tbytes\n' + ''.join(cat[r['address']] for r in rs))
print('rows', len(rs), 'missing', miss)
