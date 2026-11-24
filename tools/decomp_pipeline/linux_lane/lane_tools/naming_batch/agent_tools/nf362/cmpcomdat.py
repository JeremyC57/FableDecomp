# cmpcomdat.py <symbol> <obj1> <obj2>...: compare the COMDAT text section that defines <symbol> in each object with the first one:
# bytes and relocation list (offset, symbol, type); prints EQUAL / DIFF per object (pick-any copy census)
import sys
sys.path.insert(0, '/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF362/tools'); import nf
def sec_of(obj, sym):
    secs, sl = nf.coff(obj)
    for i, n, v, sec, t, scl in sl:
        if n == sym and sec > 0 and scl == 2: s = secs[sec - 1]; return s['data'], s['rel'], s['name']
    return None
sym = sys.argv[1]; ref = sec_of(sys.argv[2], sym)
print('REF', sys.argv[2], 'len=%d relocs=%d' % (len(ref[0]), len(ref[1])) if ref else 'NODEF')
for o in sys.argv[3:]:
    x = sec_of(o, sym)
    if not x or not ref: print('NODEF', o); continue
    print('EQUAL' if (x[0], x[1]) == (ref[0], ref[1]) else 'DIFF bytes=%s relocs=%s' % (x[0] == ref[0], x[1] == ref[1]), o, 'len=%d' % len(x[0]))
