#!/usr/bin/env python3
"""rdis.py <lo> <hi>: retail RunCutsceneMacro disassembly between two addresses."""
import sys, os, csv
csv.field_size_limit(10**8)
from capstone import Cs, CS_ARCH_X86, CS_MODE_32
S = os.environ['S']
row = next(r for r in csv.DictReader(open(f'{S}/w22/chunk876.tsv', encoding='utf-8-sig'), delimiter='\t'))
retail = bytes.fromhex(row['bytes'])
lo, hi = int(sys.argv[1], 16), int(sys.argv[2], 16)
for i in Cs(CS_ARCH_X86, CS_MODE_32).disasm(retail, 0xCBFB7D):
    if lo <= i.address <= hi: print(f'{i.address:08x} {i.mnemonic} {i.op_str}')
