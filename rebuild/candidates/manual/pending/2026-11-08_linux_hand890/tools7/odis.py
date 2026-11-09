#!/usr/bin/env python3
"""odis.py <obj> <lo> <hi> [leaf]: disassemble our Activate between offsets lo..hi (hex), relocs shown."""
import sys, re, subprocess
sys.path.insert(0, "/home/user/FableDecomp/tools/decomp_pipeline")
import verify_and_land as V
from capstone import Cs, CS_ARCH_X86, CS_MODE_32
obj = sys.argv[1]; lo = int(sys.argv[2], 16); hi = int(sys.argv[3], 16)
leaf = sys.argv[4] if len(sys.argv) > 4 else '?Activate@CTCBoastUI@@'
built, sec, sym = V.obj_text(obj, leaf)
md = Cs(CS_ARCH_X86, CS_MODE_32)
for i in md.disasm(built, 0):
    if lo <= i.address < hi: print(f'+{i.address:04x} {i.bytes.hex():20s} {i.mnemonic} {i.op_str}')
