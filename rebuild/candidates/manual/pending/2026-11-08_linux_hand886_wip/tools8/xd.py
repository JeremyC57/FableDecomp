"""xd.py <va hex> [n bytes hex]: disassemble retail Fable.exe at a VA."""
import sys, struct
from capstone import Cs, CS_ARCH_X86, CS_MODE_32
p = "/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/dl/game/Fable The Lost Chapters/Fable.exe"
d = open(p, 'rb').read()
pe = struct.unpack_from('<I', d, 0x3c)[0]; nsec = struct.unpack_from('<H', d, pe + 6)[0]; opt = struct.unpack_from('<H', d, pe + 20)[0]
base = struct.unpack_from('<I', d, pe + 24 + 28)[0]
secs = []
for i in range(nsec):
    s = pe + 24 + opt + 40 * i; vs, va, rs, ra = struct.unpack_from('<IIII', d, s + 8); secs.append((base + va, rs, ra))
def rd(va, n):
    for sva, rs, ra in secs:
        if sva <= va < sva + rs: return d[ra + va - sva: ra + va - sva + n]
va = int(sys.argv[1], 16); n = int(sys.argv[2], 16) if len(sys.argv) > 2 else 0x60
md = Cs(CS_ARCH_X86, CS_MODE_32)
for i in md.disasm(rd(va, n), va):
    print(f'{i.address:08x} {i.mnemonic} {i.op_str}')
    if i.mnemonic == 'ret' and len(sys.argv) <= 3: break
