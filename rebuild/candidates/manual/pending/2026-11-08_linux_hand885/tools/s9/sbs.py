#!/usr/bin/env python3
"""sbs.py <obj> <ours_lo_index> <retail_lo_hex> [n=30]: n instructions of ours from index lo, n of retail from address lo, side by side."""
import sys, os, csv
csv.field_size_limit(10**8)
sys.path.insert(0, "/home/user/FableDecomp/tools/decomp_pipeline")
import verify_and_land as V
from capstone import Cs, CS_ARCH_X86, CS_MODE_32
S = os.environ['S']
obj, oi, ra = sys.argv[1], int(sys.argv[2]), int(sys.argv[3], 16); n = int(sys.argv[4]) if len(sys.argv) > 4 else 30
built, _, _ = V.obj_text(obj, '?RunCutsceneMacro@@')
row = next(r for r in csv.DictReader(open(f'{S}/w22/chunk876.tsv', encoding='utf-8-sig'), delimiter='\t'))
retail = bytes.fromhex(row['bytes'])
md = Cs(CS_ARCH_X86, CS_MODE_32)
O = [f'{i.mnemonic} {i.op_str}' for i in md.disasm(built, 0)][oi:oi + n]
R = [(i.address, f'{i.mnemonic} {i.op_str}') for i in md.disasm(retail, 0xCBFB7D) if i.address >= ra][:n]
for k in range(n):
    o = O[k] if k < len(O) else ''; a, r = R[k] if k < len(R) else (0, '')
    print(f'{oi + k:6d} {o:42s} | {a:08x} {r}')
