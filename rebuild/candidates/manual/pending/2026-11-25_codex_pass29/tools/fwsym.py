"""fwsym.py <FableWin.exe> <FableWin.publics.txt> <va>...: name the callees of a FableWin.exe (editor debug build) function.
Each <va> is a call target read off `fwdis.py` (usually an incremental-link `jmp` thunk); the thunk is followed and the target is looked
up in `llvm-pdbutil dump -publics FableWin.4k.pdb` output (see tools/repack4k.py). Prints `<va> <mangled public name>`. Read-only.
(AUTH26 2026-11-25: FableWin's /Od body gave row 0x006C0B20's statement order and every callee's PDB name.)"""
import struct, sys, re
from capstone import Cs, CS_ARCH_X86, CS_MODE_32
exe, pubs = sys.argv[1], sys.argv[2]
d = open(exe, 'rb').read()
pe = struct.unpack_from('<I', d, 0x3c)[0]; ns = struct.unpack_from('<H', d, pe + 6)[0]; op = struct.unpack_from('<H', d, pe + 20)[0]
ib = struct.unpack_from('<I', d, pe + 24 + 28)[0]
secs = [struct.unpack_from('<IIII', d, pe + 24 + op + 40 * i + 8) for i in range(ns)]   # (vsize, rva, rawsize, rawoff)
def foff(rva):
    for vs, sva, rs, ro in secs:
        if sva <= rva < sva + max(vs, rs): return ro + rva - sva
def segoff(rva):
    for k, (vs, sva, rs, ro) in enumerate(secs):
        if sva <= rva < sva + max(vs, rs): return '%04d:%d' % (k + 1, rva - sva)
md = Cs(CS_ARCH_X86, CS_MODE_32)
want = {}
for a in sys.argv[3:]:
    va = int(a, 16); i = next(md.disasm(d[foff(va - ib):foff(va - ib) + 8], va))
    t = int(i.op_str, 16) if i.mnemonic == 'jmp' else va
    want[segoff(t - ib)] = a
prev = None
for line in open(pubs, encoding='utf-8', errors='replace'):
    m = re.search(r'addr = (\d+:\d+)', line)
    if m and m.group(1) in want and prev: print(want[m.group(1)], prev.strip().split('`')[1])
    if 'S_PUB32' in line: prev = line
