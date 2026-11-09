#!/usr/bin/env python3
"""dumpr.py <obj> <addr> <leaf> <lo> <hi>  -- retail [lo,hi) and ours [lo-addr, hi-addr) disassembly, two columns-free listing"""
import sys, csv, glob, os
csv.field_size_limit(10**8)
sys.path.insert(0, "/home/user/FableDecomp/tools/decomp_pipeline")
import verify_and_land as V
from capstone import Cs, CS_ARCH_X86, CS_MODE_32
S = os.environ['S']
obj, addr, leaf, lo, hi = sys.argv[1], sys.argv[2].lower(), sys.argv[3], int(sys.argv[4], 16), int(sys.argv[5], 16)
for f in glob.glob(f'{S}/w22/chunk88*.tsv'):
    for r in csv.DictReader(open(f, encoding='utf-8-sig'), delimiter='\t'):
        if r['address'].lower() == addr: retail = bytes.fromhex(r['bytes'])
built = V.obj_text(obj, leaf)[0]
A = int(addr, 16)
md = Cs(CS_ARCH_X86, CS_MODE_32)
which = sys.argv[6] if len(sys.argv) > 6 else 'both'
if which in ('both', 'r'):
    print('--- retail')
    for i in md.disasm(retail, A):
        if lo <= i.address < hi: print('%08x %s %s' % (i.address, i.mnemonic, i.op_str))
if which in ('both', 'o'):
    print('--- ours (offsets + %x)' % A)
    for i in md.disasm(built, A):
        if lo <= i.address < hi: print('%08x %s %s' % (i.address, i.mnemonic, i.op_str))
