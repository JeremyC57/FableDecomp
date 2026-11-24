# odis.py <obj> <symbol>: disassemble the COMDAT section defining <symbol> (relocations shown by symbol)
import sys, os
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import nf, capstone
obj, sym = sys.argv[1], sys.argv[2]
secs, sl = nf.coff(obj)
for i, n, v, sec, t, scl in sl:
    if n == sym and sec > 0 and scl == 2: s = secs[sec - 1]; break
else: sys.exit('NODEF')
rel = {o: nm for o, nm, ty in s['rel']}
md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
for i in md.disasm(s['data'], 0):
    r = [rel[o][:60] for o in range(i.address, i.address + i.size) if o in rel]
    print('%04x  %-8s %s %s' % (i.address, i.mnemonic, i.op_str, ('; ' + r[0]) if r else ''))
