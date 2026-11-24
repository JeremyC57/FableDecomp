# pkreach.py <pkdir> <addr...>: coordinator ruling 2026-11-24: an already-NEQ copy whose leaf reach (leafref or reach column of pk_all)
# flips n -> y between HEAD and new is a newly leaf-reached NEQ copy and fails the gate. Compares pk_all HEAD v new per (symbol, address);
# prints REACH_FLIP lines (NEQ in new, reached in new, not reached in HEAD) and a per-row verdict; PKREACH_REVERSE=1 swaps sides. Agent NF353.
import sys, os
D = sys.argv[1].rstrip('/') + '/'
def pk(p):
    d = {}
    for l in open(p):
        if l.startswith('#') or not l.strip(): continue
        f = l.rstrip('\n').split('\t')
        if len(f) < 10: continue
        d[(f[9], f[2])] = (f[0], f[7] == 'y' or f[8] == 'y')
    return d
bad = 0
for a in sys.argv[2:]:
    h, n = pk(D + 'H_%s.pkall.tsv' % a), pk(D + 'N_%s.pkall.tsv' % a)
    if os.environ.get('PKREACH_REVERSE'): h, n = n, h
    flips = []
    for k, (v, r) in n.items():
        if not v.startswith('NEQ') or not r: continue
        hv = h.get(k)
        if hv is None or not hv[1]: flips.append((k, 'absent' if hv is None else hv[0]))
    for k, hv in flips: print('%s\tREACH_FLIP\t%s\tHEAD=%s\t%s' % (a, k[1], hv, k[0][:200]))
    print('ROW %s reach=%s flips=%d' % (a, 'F' if flips else 'P', len(flips))); bad += bool(flips)
print('REACH', 'PASS' if not bad else 'FAIL (%d rows)' % bad)
