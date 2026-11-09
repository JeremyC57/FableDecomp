#!/usr/bin/env python3
"""odis.py <obj> [from_ins] [count]: RunCutsceneMacro instructions of an object (capstone), side by side with retail when
aligned by index."""
import sys, os, csv
csv.field_size_limit(10**8)
sys.path.insert(0, "/home/user/FableDecomp/tools/decomp_pipeline")
import verify_and_land as V
from capstone import Cs, CS_ARCH_X86, CS_MODE_32
S = os.environ['S']
obj = sys.argv[1]; a = int(sys.argv[2]) if len(sys.argv) > 2 else 0; n = int(sys.argv[3]) if len(sys.argv) > 3 else 30
built, sec, sym = V.obj_text(obj, '?RunCutsceneMacro@@')
row = next(r for r in csv.DictReader(open(f'{S}/w22/chunk876.tsv', encoding='utf-8-sig'), delimiter='\t'))
retail = bytes.fromhex(row['bytes'])
md = Cs(CS_ARCH_X86, CS_MODE_32)
O = [f'{i.mnemonic} {i.op_str}' for i in md.disasm(built, 0)]
R = [f'{i.mnemonic} {i.op_str}' for i in md.disasm(retail, 0xCBFB7D)]
for k in range(a, min(a + n, len(O))):
    print(f'{k:5d} {"  " if O[k] == R[k] else "**"} {O[k]:40s} | {R[k]}')
