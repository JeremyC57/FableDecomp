#!/usr/bin/env python3
# rdis.py addr : disassemble the retail oracle bytes
import sys,csv
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
a=sys.argv[1].lower().replace('0x','')
for r in csv.DictReader(open('/home/user/FableDecomp/rebuild/oracles/auto-re-candidates.tsv',encoding='utf-8-sig'),delimiter='\t'):
    if r['address'].lower().replace('0x','')==a:
        b=bytes.fromhex(r['bytes']); break
md=Cs(CS_ARCH_X86,CS_MODE_32)
for i in md.disasm(b,int(a,16)): print(f"{i.address:08x}: {i.mnemonic} {i.op_str}")
