#!/usr/bin/env python3
"""strins.py <obj>: string instructions (cmps/movs/stos/scas/lods) in ours v retail with surrounding esi/edi loads."""
import sys, os, csv
csv.field_size_limit(10**8)
sys.path.insert(0, "/home/user/FableDecomp/tools/decomp_pipeline")
import verify_and_land as V
from capstone import Cs, CS_ARCH_X86, CS_MODE_32
S = os.environ['S']
built, sec, sym = V.obj_text(sys.argv[1], '?RunCutsceneMacro@@')
row = next(r for r in csv.DictReader(open(f'{S}/w22/chunk876.tsv', encoding='utf-8-sig'), delimiter='\t'))
retail = bytes.fromhex(row['bytes'])
md = Cs(CS_ARCH_X86, CS_MODE_32)
for nm, b, base in (('ours', built, 0), ('retail', retail, 0xCBFB7D)):
    L = list(md.disasm(b, base))
    hits = [i for i, x in enumerate(L) if x.mnemonic.split()[-1][:4] in ('cmps', 'movs', 'stos', 'scas', 'lods')]
    print(nm, len(hits), [f'{i}:{L[i].address:x}:{L[i].mnemonic}' for i in hits])
