#!/usr/bin/env python3
"""frame7.py <obj...>: print the __chkstk frame size of RunCutsceneMacro in each object."""
import sys
sys.path.insert(0, "/home/user/FableDecomp/tools/decomp_pipeline")
import verify_and_land as V
from capstone import Cs, CS_ARCH_X86, CS_MODE_32
md = Cs(CS_ARCH_X86, CS_MODE_32)
for o in sys.argv[1:]:
    b, _, _ = V.obj_text(o, '?RunCutsceneMacro@@')
    fr = next((i.op_str for i in md.disasm(b[:64], 0) if i.mnemonic == 'mov' and i.op_str.startswith('eax, 0x')), '?')
    print(o, fr)
