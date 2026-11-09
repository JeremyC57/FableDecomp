#!/usr/bin/env python3
"""ncalls.py <obj>: call count, frame size (sub esp), entry registers (first 12 insns: this/zero), and the empty-boasts continue
layout (retail: `jne body` right after `cmp [eax],eax` with clear expanded inline = 'inline'; ours d1: `je far` = 'far')."""
import sys, re
sys.path.insert(0, "/home/user/FableDecomp/tools/decomp_pipeline")
import verify_and_land as V
from capstone import Cs, CS_ARCH_X86, CS_MODE_32
built, sec, sym = V.obj_text(sys.argv[1], '?Activate@CTCBoastUI@@')
I = list(Cs(CS_ARCH_X86, CS_MODE_32).disasm(built, 0))
calls = sum(1 for i in I if i.mnemonic == 'call')
fr = next((i.op_str.split(',')[1].strip() for i in I[:6] if i.mnemonic == 'sub' and i.op_str.startswith('esp')), '?')
ent = ' '.join(f'{i.mnemonic} {i.op_str}' for i in I[7:12] if i.mnemonic in ('mov', 'xor') and 'byte' not in i.op_str)
lay = []
for k, i in enumerate(I):
    if i.mnemonic == 'cmp' and i.op_str == 'dword ptr [eax], eax' and k + 1 < len(I):
        lay.append(I[k + 1].mnemonic)
print(f'calls {calls} frame {fr} ent[{ent}] empty-jcc {",".join(lay)}')
