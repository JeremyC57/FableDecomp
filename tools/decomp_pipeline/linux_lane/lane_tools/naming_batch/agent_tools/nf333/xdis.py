# xdis.py <va> [n]: capstone listing of retail Fable.exe at a VA (screen only; never write its output into a hand-back file), with RTTI-port labels for call targets
import sys, os, struct
from capstone import Cs, CS_ARCH_X86, CS_MODE_32
R = '/home/user/FableDecomp'; L = {}
for f in ('labels_rtti_port.tsv', 'labels_rtti_port_dtors.tsv'):
    for l in open(R + '/ghidra_out/' + f):
        p = l.rstrip('\n').split('\t')
        if len(p) > 1: L.setdefault(int(p[0], 16), p[1])
for f in ('egor_pdb_names.tsv',):
    pass
d = open(os.environ['FABLE_EXE'], 'rb').read()
pe = struct.unpack_from('<I', d, 0x3C)[0]; ns = struct.unpack_from('<H', d, pe + 6)[0]; osz = struct.unpack_from('<H', d, pe + 20)[0]
base = struct.unpack_from('<I', d, pe + 52)[0]; so = pe + 24 + osz
va = int(sys.argv[1], 16); n = int(sys.argv[2]) if len(sys.argv) > 2 else 64
for i in range(ns):
    vsz, vaddr = struct.unpack_from('<II', d, so + i * 40 + 8); raw = struct.unpack_from('<I', d, so + i * 40 + 20)[0]
    if base + vaddr <= va < base + vaddr + vsz: code = d[raw + va - base - vaddr: raw + va - base - vaddr + n]
md = Cs(CS_ARCH_X86, CS_MODE_32)
for ins in md.disasm(code, va):
    t = ''
    try:
        x = int(ins.op_str, 16); t = L.get(x, '')
    except ValueError: pass
    print('%08X %-6s %-30s %s' % (ins.address, ins.mnemonic, ins.op_str, t))
    if ins.mnemonic in ('ret',) : break
