#!/usr/bin/env python3
"""cfgdis.py <va> [maxinsn]: CFG walk of a c2.dll function from va (follows jumps, not calls), prints basic blocks in
address order with their successors -- hot/cold split functions read as one listing."""
import sys, os, struct
from capstone import Cs, CS_ARCH_X86, CS_MODE_32
c2 = os.path.join(os.environ.get('FABLE_VC71'), 'bin', 'c2.dll'); data = open(c2, 'rb').read()
pe = struct.unpack_from('<I', data, 0x3c)[0]; nsec = struct.unpack_from('<H', data, pe + 6)[0]; opt = struct.unpack_from('<H', data, pe + 20)[0]
secs = []
for i in range(nsec):
    o = pe + 24 + opt + 40 * i; vs, va, rs, ra = struct.unpack_from('<IIII', data, o + 8); secs.append((va, vs, ra, rs))
def off(va):
    rva = va - 0x10700000
    for sva, vs, ra, rs in secs:
        if sva <= rva < sva + max(vs, rs): return rva - sva + ra
md = Cs(CS_ARCH_X86, CS_MODE_32)
start = int(sys.argv[1], 16); MAX = int(sys.argv[2]) if len(sys.argv) > 2 else 3000
todo = [start]; seen = {}; n = 0
while todo and n < MAX:
    a = todo.pop()
    if a in seen: continue
    blk = []; o = off(a)
    for i in md.disasm(data[o:o + 4096], a):
        if i.address in seen and blk: break
        blk.append(i); n += 1
        m = i.mnemonic
        if m.startswith('j') or m.startswith('loop'):
            try: todo.append(int(i.op_str, 16))
            except ValueError: pass
            if m == 'jmp': break
        if m.startswith('ret'): break
    for i in blk: seen[i.address] = i
for a in sorted(seen):
    i = seen[a]; print(f'{a:08x} {i.mnemonic} {i.op_str}')
