#!/usr/bin/env python3
"""objcheck.py <obj> <leaf> [listing.log]: size of the function in an object lk produced, and (optionally) whether its
disassembly equals the 'built' side of a q.py -v log (address + full operand text, unmasked; both sides show
unrelocated operands as 0)."""
import sys, os, re
sys.dont_write_bytecode = True
sys.path.insert(0, "/home/user/FableDecomp/tools/decomp_pipeline")
import verify_and_land as V
from capstone import Cs, CS_ARCH_X86, CS_MODE_32
obj, leaf = sys.argv[1], sys.argv[2]
code = V.obj_text(obj, leaf)[0]
print("size", len(code))
if len(sys.argv) > 3:
    base = 0x00B4D370 if len(sys.argv) < 5 else int(sys.argv[4], 16)
    md = Cs(CS_ARCH_X86, CS_MODE_32)
    mine = [f"{i.address:08x} {i.mnemonic} {i.op_str}" for i in md.disasm(code, base)]
    L = open(sys.argv[3]).read().split("--- built", 1)[1].splitlines()[1:]
    ref = []
    for l in L:
        p = l.split(None, 2)
        if len(p) == 3:
            ref.append(p[0] + " " + p[2])
    diff = [(a, b) for a, b in zip(mine, ref) if a != b]
    same = len(mine) == len(ref) and not diff
    print("instructions", len(mine), "listing", len(ref), "SAME (unmasked text)" if same else "DIFFERENT", diff[:5])
