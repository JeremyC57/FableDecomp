# rdis.py <addr> [n]: disassemble the retail bytes of a landed row from the catalog (work-local view only; never copied into hand-back files)
import sys; sys.path.insert(0, __file__.rsplit('/', 1)[0]); import nf, capstone
a = sys.argv[1].lower().replace('0x', ''); n = int(sys.argv[2]) if len(sys.argv) > 2 else 60
b = bytes.fromhex(nf.catalog()[a]['bytes']); md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
for k, i in enumerate(md.disasm(b, int(a, 16))):
    if k >= n: break
    print('%08x  %s %s' % (i.address, i.mnemonic, i.op_str))
