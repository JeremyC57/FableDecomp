# leafcmp.py <head.obj> <head-leaf> <new.obj> <new-leaf> [old=new ...]: one function COMDAT of each object, bytes and relocation list equal
# under the given symbol renames; also the COMDAT selection of each (NF355)
import sys, os, struct
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import nf
def fn(o, name):
    secs, sl = nf.coff(o)
    for i, n, v, sec, t, scl in sl:
        if n == name and sec > 0: return secs[sec - 1]
h, hl, n, nl = sys.argv[1:5]; mp = dict(x.split('=', 1) for x in sys.argv[5:])
a, b = fn(h, hl), fn(n, nl)
ra = [(o, mp.get(s, s), t) for o, s, t in a['rel']]; rb = list(b['rel'])
print('bytes', 'EQUAL' if a['data'] == b['data'] else 'DIFF', len(a['data']), len(b['data']))
print('relocs', 'EQUAL' if ra == rb else 'DIFF'); print(' head', ra); print(' new ', rb)
