# vtdump.py <vftable-symbol> <obj>...: per object, the data COMDAT defining the vftable: size, selection and slot symbols (NF355)
import sys, os, struct
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import nf
v = sys.argv[1]
for o in sys.argv[2:]:
    secs, sl = nf.coff(o); hit = [(val, sec) for i, n, val, sec, t, scl in sl if n == v and sec > 0]
    if not hit: print(os.path.basename(o), 'NONE'); continue
    val, sec = hit[0]; s = secs[sec - 1]
    rel = sorted((off, sym) for off, sym, t in s['rel'] if off >= val)
    print(os.path.basename(o), 'size', len(s['data']) - val, 'slots', len(rel), [sym for off, sym in rel][:3], '...' if len(rel) > 3 else '')
