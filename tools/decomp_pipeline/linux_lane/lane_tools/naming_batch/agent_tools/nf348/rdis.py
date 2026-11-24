# rdis.py <addr> [len]: disassemble Fable.exe at a retail address (catalog length by default). Prints to stdout only (no byte dumps kept).
import sys, struct, os
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import nf, capstone
a = sys.argv[1].lower().replace('0x', ''); va = int(a, 16)
n = int(sys.argv[2]) if len(sys.argv) > 2 else int(nf.catalog()[a]['length'])
for nm, base, d, ch in nf.exe():
    if base <= va < base + len(d):
        code = d[va - base: va - base + n]; break
md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
for i in md.disasm(code, va): print('%08x  %-8s %s' % (i.address, i.mnemonic, i.op_str))
