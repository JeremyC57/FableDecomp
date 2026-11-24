# gradecmp.py <batch dir> <name>: vland --recheck-landed HEAD v new (work/vland_<name>_{H,N}.log): per row parity + behaviour, SAME / CHANGED;
# copies both logs to <batch>/logs/recheck_{head,new}.log and writes <batch>/evidence/grade_head_v_new.txt.
import sys, os, re, shutil
A = os.path.dirname(os.path.dirname(os.path.abspath(__file__))); B, nm = sys.argv[1], sys.argv[2]
os.makedirs(B + '/logs', exist_ok=True)
def st(p):
    d = {}
    for l in open(p, errors='replace'):
        m = re.match(r'([0-9a-f]{8})\s+(\S+)\s+(\S+)\s', l)
        if m and m.group(2) != 'WARN': d.setdefault(m.group(1), (m.group(2), m.group(3)))
    return d
h, n = A + '/work/vland_%s_H.log' % nm, A + '/work/vland_%s_N.log' % nm
shutil.copy(h, B + '/logs/recheck_head.log'); shutil.copy(n, B + '/logs/recheck_new.log')
H, N = st(h), st(n); rows = [l.strip() for l in open(B + '/rows.txt') if l.strip()]
out = ['# vland --recheck-landed, HEAD payload (head/payload.json) v new payload (rows/): parity behaviour per row']; bad = 0
for a in rows:
    x, y = H.get(a, ('NOT_GRADED', '-')), N.get(a, ('NOT_GRADED', '-'))
    ok = x == y and x[0] in ('MATCH', 'RELOCATION_MATCH') and x[1] == 'PASS'
    bad += not ok; out.append('%s\t%s %s\t%s %s\t%s' % (a, x[0], x[1], y[0], y[1], 'SAME' if ok else 'CHANGED_OR_FAIL'))
out.append('# rows %d, grade unchanged and passing %d, other %d' % (len(rows), len(rows) - bad, bad))
open(B + '/evidence/grade_head_v_new.txt', 'w').write('\n'.join(out) + '\n'); print(out[-1]); [print(l) for l in out if 'CHANGED' in l]
