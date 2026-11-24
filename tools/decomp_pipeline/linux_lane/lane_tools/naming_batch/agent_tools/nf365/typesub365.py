# typesub365.py <objX> <symX> <objY> <symY> <typeX> <typeY>: compare two COMDAT text sections (bytes, relocation list) after replacing
# the mangled class name typeX with typeY in X's relocation symbols (a structural twin check for a template instance with no landed
# same-named reference). Prints EQUAL_MODULO_TYPE / DIFF and the relocation symbols (no bytes).
import sys, os
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import nf
def sec(o, s):
    secs, sl = nf.coff(o)
    for i, n, v, sc, t, scl in sl:
        if n == s and sc > 0 and scl == 2: return secs[sc - 1]
ox, sx, oy, sy, tx, ty = sys.argv[1:7]
x, y = sec(ox, sx), sec(oy, sy)
rx = [(v, s.replace(tx, ty), t) for v, s, t in x['rel']]
print('EQUAL_MODULO_TYPE' if (x['data'], rx) == (y['data'], y['rel']) else 'DIFF bytes=%s relocs=%s' % (x['data'] == y['data'], rx == y['rel']), 'len=%d/%d' % (len(x['data']), len(y['data'])))
for r in x['rel']: print('  X reloc', r[0], r[1], r[2])
for r in y['rel']: print('  Y reloc', r[0], r[1], r[2])
