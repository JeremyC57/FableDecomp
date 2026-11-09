#!/usr/bin/env python3
"""rdis.py <va-hex> <len-hex> : disassemble retail bytes."""
import os, struct, sys
from capstone import Cs, CS_ARCH_X86, CS_MODE_32
exe = open(os.environ["FABLE_EXE"], "rb").read()
pe = struct.unpack_from("<I", exe, 0x3c)[0]; ns = struct.unpack_from("<H", exe, pe+6)[0]; opt = struct.unpack_from("<H", exe, pe+20)[0]
va = int(sys.argv[1], 16); n = int(sys.argv[2], 16)
for i in range(ns):
    o = pe+24+opt+40*i; vs, sva, rs, ro = struct.unpack_from("<IIII", exe, o+8)
    if sva <= va-0x400000 < sva+rs:
        off = va-0x400000-sva+ro
        for ins in Cs(CS_ARCH_X86, CS_MODE_32).disasm(exe[off:off+n], va): print(hex(ins.address), ins.mnemonic, ins.op_str)
