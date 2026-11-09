#!/usr/bin/env python3
"""xv.py <base.cpp> <first line> <last line> <variant file> <tag> <retail lo> <retail hi>: replace source lines
[first..last] of base with the variant text, compile with /FAs (mcfa.py), and compare the instructions of the replaced lines
with retail [lo, hi] (relocations normalised, [ebp+x] masked, registers exact). Prints OK/DIFF and the side-by-side."""
import sys, os, re, subprocess, csv, difflib
csv.field_size_limit(10**8)
base, l1, l2, var, tag, rlo, rhi = sys.argv[1], int(sys.argv[2]), int(sys.argv[3]), sys.argv[4], sys.argv[5], int(sys.argv[6], 16), int(sys.argv[7], 16)
A = os.environ['A']; S = os.environ['S']
src = open(base).read().split('\n'); vt = open(var).read().rstrip('\n').split('\n')
new = src[:l1 - 1] + vt + src[l2:]
d = f'{A}/s7/x87/{tag}'; os.makedirs(d, exist_ok=True)
open(f'{d}/v.cpp', 'w').write('\n'.join(new))
r = subprocess.run(['timeout', '600', 'python3', f'{A}/tools/pk/mcfa.py', f'{d}/v.cpp', d], cwd='/home/user/FableDecomp', capture_output=True, text=True)
if 'ok' not in r.stdout: print(tag, 'FAIL', r.stdout[-800:]); sys.exit(1)
sys.path.insert(0, "/home/user/FableDecomp/tools/decomp_pipeline")
import verify_and_land as V
from capstone import Cs, CS_ARCH_X86, CS_MODE_32
built, _, _ = V.obj_text(f'{d}/m.obj', '?RunCutsceneMacro@@')
md = Cs(CS_ARCH_X86, CS_MODE_32); O = list(md.disasm(built, 0))
lines = []; cur = 0; inside = False
for l in open(f'{d}/m.asm', encoding='latin1'):
    if not inside:
        if l.startswith('?RunCutsceneMacro@@YIX') and ' PROC' in l: inside = True
        continue
    if ' ENDP' in l and l.startswith('?RunCutsceneMacro'): break
    m = re.match(r'^; (\d+)\s+:', l)
    if m: cur = int(m.group(1)); continue
    if re.match(r'^\t[a-z]', l): lines.append(cur)
row = next(r for r in csv.DictReader(open(f'{S}/w22/chunk876.tsv', encoding='utf-8-sig'), delimiter='\t'))
retail = bytes.fromhex(row['bytes'])
R = [i for i in md.disasm(retail, 0xCBFB7D) if rlo <= i.address <= rhi]
lo, hi = int(os.environ.get('XLO', l1)), int(os.environ.get('XHI', l1 + len(vt) - 1))
OS = [O[k] for k in range(len(O)) if lo <= lines[k] <= hi]
def key(i):
    o = i.op_str
    if re.match(r'^(j\w+|call)$', i.mnemonic) and re.match(r'^0x[0-9a-f]+$', o): o = 'T'
    o = re.sub(r"dword ptr \[0\]|dword ptr 0x[0-9a-f]{6,8}\b", "dword ptr R", o); o = re.sub(r"0x[0-9a-f]{6,8}\b", "R", o)
    o = re.sub(r'\[ebp [-+] 0x[0-9a-f]+\]|\[ebp\]', '[F]', o)
    return i.mnemonic + ' ' + o
KO = [key(i) for i in OS]; KR = [key(i) for i in R]
sm = difflib.SequenceMatcher(None, KR, KO, autojunk=False)
m = sum(b.size for b in sm.get_matching_blocks())
print(f'{tag}: {"OK" if KO == KR else "DIFF"} match {m}/{len(KR)} (ours {len(KO)})')
if '-v' in sys.argv:
    for op, a1, a2, b1, b2 in sm.get_opcodes():
        for j in range(max(a2 - a1, b2 - b1)):
            ra = KR[a1 + j] if a1 + j < a2 else ''; ob = KO[b1 + j] if b1 + j < b2 else ''
            print(f'  {"=" if op == "equal" else "*"} {ra:36s} | {ob}')
