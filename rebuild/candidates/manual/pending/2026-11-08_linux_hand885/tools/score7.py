#!/usr/bin/env python3
"""score7.py <obj>: one-line fitness: bytes, first register-level difference index (frame offsets masked), and the difflib
count of retail instructions matched with frame offsets masked but registers exact ("fx"), plus fully exact ("ex")."""
import sys, os, re, csv, difflib
csv.field_size_limit(10**8)
sys.path.insert(0, "/home/user/FableDecomp/tools/decomp_pipeline")
import verify_and_land as V
from capstone import Cs, CS_ARCH_X86, CS_MODE_32
S = os.environ['S']
row = next(r for r in csv.DictReader(open(f'{S}/w22/chunk876.tsv', encoding='utf-8-sig'), delimiter='\t'))
retail = bytes.fromhex(row['bytes'])
built, _, _ = V.obj_text(sys.argv[1], '?RunCutsceneMacro@@')
md = Cs(CS_ARCH_X86, CS_MODE_32)
O = list(md.disasm(built, 0)); R = list(md.disasm(retail, 0xCBFB7D))
JMP = re.compile(r'^(j\w+|call|loop\w*)$')
def k(i, base, frame):
    o = i.op_str
    if JMP.match(i.mnemonic) and re.match(r'^0x[0-9a-f]+$', o): o = 'T'
    o = re.sub(r'0x[0-9a-f]{6,8}\b', lambda m: 'R' if base == 0 or int(m.group(0), 16) >= 0x400000 else m.group(0), o)
    o = re.sub(r'\[0\]', '[R]', o)
    if base == 0: o = re.sub(r'(?<![\w\[+-])0(?![\w\]])', 'R', o) if i.mnemonic == 'push' else o
    if frame: o = re.sub(r'\[ebp [-+] 0x[0-9a-f]+\]|\[ebp\]', '[F]', o)
    return i.mnemonic + ' ' + o
KO = [k(i, 0, True) for i in O]; KR = [k(i, 1, True) for i in R]
first = next((n for n, (a, b) in enumerate(zip(KO, KR)) if a != b and n > 1), -1)
fx = sum(b.size for b in difflib.SequenceMatcher(None, KR, KO, autojunk=False).get_matching_blocks())
print(f'{len(built)} first {first} fx {fx}')
