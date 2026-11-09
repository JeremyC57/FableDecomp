#!/usr/bin/env python3
"""first.py <obj> [--noframe] [--n N]: (--noframe: [ebp +- disp] normalised, so the first REGISTER/shape difference; --n: list N)
 index of the first instruction of RunCutsceneMacro that differs from retail (relocations / branch
targets / call targets normalised), and the count of instructions identical in the longest common prefix."""
import sys, os, csv, re, subprocess
csv.field_size_limit(10**8)
sys.path.insert(0, "/home/user/FableDecomp/tools/decomp_pipeline")
import verify_and_land as V
from capstone import Cs, CS_ARCH_X86, CS_MODE_32
S = os.environ['S']
obj = sys.argv[1]
NOFRAME = '--noframe' in sys.argv
NN = int(sys.argv[sys.argv.index('--n')+1]) if '--n' in sys.argv else 1
built, sec, sym = V.obj_text(obj, '?RunCutsceneMacro@@')
row = next(r for r in csv.DictReader(open(f'{S}/w22/chunk876.tsv', encoding='utf-8-sig'), delimiter='\t'))
retail = bytes.fromhex(row['bytes'])
md = Cs(CS_ARCH_X86, CS_MODE_32)
O = list(md.disasm(built, 0)); R = list(md.disasm(retail, 0xCBFB7D))
JMP = re.compile(r'^(j\w+|call|loop\w*)$')
def k(i, base):
    o = i.op_str
    if JMP.match(i.mnemonic) and re.match(r'^0x[0-9a-f]+$', o): o = 'T'
    o = re.sub(r'0x[0-9a-f]{6,8}\b', lambda m: 'R' if base == 0 or int(m.group(0), 16) >= 0x400000 else m.group(0), o)
    o = re.sub(r'\[0\]', '[R]', o)
    if NOFRAME: o = re.sub(r'\[ebp [-+] 0x[0-9a-f]+\]|\[ebp\]', '[F]', o)
    return i.mnemonic + ' ' + o
for n, (a, b) in enumerate(zip(O, R)):
    ka, kb = k(a, 0), k(b, 1)
    if ka != kb and re.sub(r'(?<![\w\[])0(?![\w\]])', 'R', ka) != kb:
        print(n, f'ours {a.mnemonic} {a.op_str} | retail {b.mnemonic} {b.op_str} @0x{b.address:08x}')
        NN -= 1
        if NN <= 0: break
