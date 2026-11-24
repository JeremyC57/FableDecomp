# copycensus360.py <symscan.out> <symbol> <owner.obj>: for every scratch object that DEFINES <symbol>, group by row address (the 8-hex
# object basename), keep the newest object per address, and compare its COMDAT (bytes + relocation list) with the owner's. Prints
# per address: sel, EQUAL / DIFF, size, object path; last line: totals. A census of the scratch corpus (stale objects included), not HEAD. (NF360)
import sys, os, re
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import nf
scan, sym, owner = sys.argv[1:4]
def sec(obj):
    secs, sl = nf.coff(obj)
    for i, n, v, s, t, scl in sl:
        if n == sym and s > 0 and scl == 2:
            x = secs[s - 1]; return x['data'], [(o, y, ty) for o, y, ty in x['rel']]
    return None
ref = sec(owner); best = {}
for l in open(scan):
    f = l.rstrip('\n').split('\t')
    if len(f) < 4 or f[0] != 'DEF' or f[2] != sym: continue
    m = re.match(r'(?:H_|D_)?([0-9a-f]{8})', os.path.basename(f[3]))
    if not m or '.t.obj' in f[3]: continue
    a = m.group(1); mt = os.path.getmtime(f[3])
    if a not in best or mt > best[a][0]: best[a] = (mt, f[3], f[1])
eq = df = 0
for a in sorted(best):
    mt, p, sl = best[a]; x = sec(p)
    v = 'EQUAL' if x == ref else 'DIFF'; eq += v == 'EQUAL'; df += v == 'DIFF'
    print('%s\tsel=%s\t%s\tsize=%d\t%s' % (a, sl, v, len(x[0]) if x else -1, p.split('/scratchpad/')[-1]))
print('# addresses %d EQUAL %d DIFF %d' % (len(best), eq, df))
