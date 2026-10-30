#!/usr/bin/env python3
"""dis.py <va> [nbytes]: capstone disassembly of retail bytes with labels on call/jmp targets."""
import sys, os
sys.argv, a = [sys.argv[0]], sys.argv[1:]
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import vt
from capstone import Cs, CS_ARCH_X86, CS_MODE_32
va = int(a[0], 16); n = int(a[1], 0) if len(a) > 1 else 0x60
md = Cs(CS_ARCH_X86, CS_MODE_32)
for i in md.disasm(vt.rd(va, n), va):
    extra = ""
    for tok in i.op_str.replace("[", " ").replace("]", " ").replace(",", " ").split():
        if tok.startswith("0x") and len(tok) >= 8:
            v = int(tok, 16); l = vt.lab.get("%08x" % v)
            if l: extra += "  ; " + l[:110]
    print(f"{i.address:08x}  {i.bytes.hex():24} {i.mnemonic} {i.op_str}{extra}")
    if i.mnemonic == "ret" and len(a) < 3: break
