#!/usr/bin/env python3
"""dis.py <va_start> <va_end>: disassemble c2.dll (13.10.3077, base 0x10700000) range."""
import sys, os
from capstone import Cs, CS_ARCH_X86, CS_MODE_32
c2 = os.path.join(os.environ.get('FABLE_VC71', '/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/wine/vc71rtm'), 'bin', 'c2.dll')
data = open(c2, 'rb').read()
import struct
pe = struct.unpack_from('<I', data, 0x3c)[0]
nsec = struct.unpack_from('<H', data, pe + 6)[0]; opt = struct.unpack_from('<H', data, pe + 20)[0]
secs = []
for i in range(nsec):
    o = pe + 24 + opt + 40 * i
    name = data[o:o+8].rstrip(b'\0'); vs, va, rs, ra = struct.unpack_from('<IIII', data, o + 8)
    secs.append((va, vs, ra, rs))
def off(va):
    rva = va - 0x10700000
    for sva, vs, ra, rs in secs:
        if sva <= rva < sva + max(vs, rs): return rva - sva + ra
a = int(sys.argv[1], 16); b = int(sys.argv[2], 16)
md = Cs(CS_ARCH_X86, CS_MODE_32)
o = off(a)
for i in md.disasm(data[o:o + (b - a)], a):
    print(f'{i.address:08x} {i.mnemonic} {i.op_str}')
