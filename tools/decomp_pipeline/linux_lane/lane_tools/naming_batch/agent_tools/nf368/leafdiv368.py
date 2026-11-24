# leafdiv368.py <objA> <objB> <leaf symbol>: where two builds of one leaf first diverge (rebuild objects only, never retail bytes).
# Masks every relocation field (4 bytes) on both sides, prints both sizes, the first differing offset, the objdump instruction lines of
# A and B around it, and the relocation symbol lists' first difference (order of call targets).
import sys, subprocess, os
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import nf
W = nf.W
def leaf(p, sym):
    secs, sl = nf.coff(p)
    for i, nm, v, sec, t, scl in sl:
        if nm == sym and sec > 0:
            x = secs[sec - 1]; d = bytearray(x['data'])
            for o, s, ty in x['rel']: d[o:o + 4] = b'\0\0\0\0'
            return bytes(d), [s for o, s, ty in x['rel'] if not s.startswith('$')]
    raise SystemExit('no leaf in ' + p)
def dis(b, tag):
    f = W / ('leafdiv_%s.bin' % tag); f.write_bytes(b)
    r = subprocess.run(['objdump', '-D', '-b', 'binary', '-m', 'i386', '-M', 'intel', str(f)], capture_output=True, text=True).stdout
    out = []
    for l in r.splitlines():
        p = l.split(':', 1)
        if len(p) == 2 and p[0].strip() and all(c in '0123456789abcdef' for c in p[0].strip()):
            cols = p[1].split('\t'); out.append((int(p[0].strip(), 16), cols[-1].strip() if len(cols) > 2 else ''))
    return out
a, b, sym = sys.argv[1:4]
da, ra = leaf(a, sym); db, rb = leaf(b, sym)
print('size A %d  B %d' % (len(da), len(db)))
i = next((k for k in range(min(len(da), len(db))) if da[k] != db[k]), None)
print('first differing byte offset:', hex(i) if i is not None else 'none (prefix equal)')
ia, ib = dis(da, 'A'), dis(db, 'B')
if i is not None:
    sa = [x for x in ia if i - 0x18 <= x[0] <= i + 0x30]; sb = [x for x in ib if i - 0x18 <= x[0] <= i + 0x30]
    print('A around it:'); [print('   %5x  %s' % x) for x in sa]
    print('B around it:'); [print('   %5x  %s' % x) for x in sb]
nd = sum(1 for x, y in zip(ia, ib) if x != y)
print('aligned instruction pairs differing: %d of %d / %d' % (nd, len(ia), len(ib)))
j = next((k for k in range(min(len(ra), len(rb))) if ra[k] != rb[k]), None)
print('external relocation targets: A %d  B %d; first differing index: %s' % (len(ra), len(rb), j))
if j is not None: print('   A:', ra[j][:110]); print('   B:', rb[j][:110])
