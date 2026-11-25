#!/usr/bin/env python3
"""rdis.py <hexaddr> [nbytes]: retail Fable.exe disassembly (FABLE_EXE) from a VA."""
import sys, os, struct
from capstone import Cs, CS_ARCH_X86, CS_MODE_32
E = os.environ.get('FABLE_EXE'); d = open(E, 'rb').read()
pe = struct.unpack_from('<I', d, 0x3c)[0]; ns = struct.unpack_from('<H', d, pe + 6)[0]; op = struct.unpack_from('<H', d, pe + 20)[0]
ib = struct.unpack_from('<I', d, pe + 24 + 28)[0]
va = int(sys.argv[1], 16); n = int(sys.argv[2]) if len(sys.argv) > 2 else 64
for i in range(ns):
    vs, vaddr, rs, rp = struct.unpack_from('<IIII', d, pe + 24 + op + 40 * i + 8)
    if ib + vaddr <= va < ib + vaddr + max(vs, rs):
        off = rp + va - ib - vaddr; b = d[off:off + n]; break
for x in Cs(CS_ARCH_X86, CS_MODE_32).disasm(b, va):
    print('%08x %-16s %s %s' % (x.address, x.bytes.hex(), x.mnemonic, x.op_str))
    if x.mnemonic in ('ret', 'jmp') and x.address - va > 4 and len(sys.argv) <= 3: break
