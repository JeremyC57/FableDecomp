#!/usr/bin/env python3
"""m3.py <obj> [leaf]: three difflib shares of retail 0x00CBFB7D instructions matched by ours:
exact (relocs/branch targets normalised), regs masked (eax..ebx -> R), regs+frame masked ([ebp+X] -> M); plus frame (sub esp)."""
import sys, os, re, csv, difflib, subprocess
csv.field_size_limit(10**8)
sys.path.insert(0, "/home/user/FableDecomp/tools/decomp_pipeline")
import verify_and_land as V
from capstone import Cs, CS_ARCH_X86, CS_MODE_32
S = os.environ['S']
obj = sys.argv[1]
leaf = sys.argv[2] if len(sys.argv) > 2 else '?RunCutsceneMacro@@'
RET = 0xCBFB7D
row = next(r for r in csv.DictReader(open(f'{S}/w22/chunk876.tsv', encoding='utf-8-sig'), delimiter='\t'))
retail = bytes.fromhex(row['bytes'])
while retail and retail[-1] in (0xcc, 0x90): retail = retail[:-1]
built, sec, sym = V.obj_text(obj, leaf)
blocks = []; cur = None
for line in subprocess.run(['objdump', '-r', obj], capture_output=True, text=True).stdout.splitlines():
    m = re.match(r'RELOCATION RECORDS FOR \[(.*)\]:', line)
    if m: cur = {'name': m.group(1), 'r': {}}; blocks.append(cur); continue
    m = re.match(r'^([0-9a-f]{8}) (\S+)\s+(.*)$', line)
    if m and cur is not None: cur['r'][int(m.group(1), 16)] = m.group(3).strip()
best = None
for b in blocks:
    if b['name'].startswith('.text') and b['r'] and max(b['r']) < len(built) + 4:
        if best is None or len(b['r']) > len(best['r']): best = b
rel = best['r'] if best else {}
md = Cs(CS_ARCH_X86, CS_MODE_32)
def dis(code, base): return [(i.address - base, i.size, i.mnemonic, i.op_str) for i in md.disasm(code, base)]
R = dis(retail, RET); O = dis(built, 0)
def cut(L):
    for k, i in enumerate(L):
        if i[2] == 'ret' and i[3] == '0x10': return L[:k+1]
    return L
R = cut(R); O = cut(O)
JMP = re.compile(r'^(j\w+|call|loop\w*)$')
def key(ins, base, mode):
    off, size, mn, op = ins
    o = op
    if JMP.match(mn) and re.match(r'^0x[0-9a-f]+$', o): o = 'T'
    if base == 0:
        if any(r in rel for r in range(off, off + size)): o = re.sub(r'0x[0-9a-f]+|\b0\b', 'R', o)
    else:
        o = re.sub(r'0x[0-9a-f]{6,8}\b', lambda m: 'R' if int(m.group(0), 16) >= 0x400000 else m.group(0), o)
    if mode >= 1:
        o = re.sub(r'\b(eax|ecx|edx|esi|edi|ebx)\b', 'R', o); o = re.sub(r'\b(al|cl|dl|bl|ah|bh|si|di|bx)\b', 'r', o)
    if mode >= 2:
        o = re.sub(r'\[ebp(?: [+-] (?:0x[0-9a-f]+|\d+))?\]', 'M', o)
    return mn + ' ' + o
fr = lambda L: next((i[3] for i in L[:6] if i[2] == 'sub' and i[3].startswith('esp')), '?')
out = []
for mode, name in ((0, 'exact'), (1, 'regs'), (2, 'regs+frame')):
    sm = difflib.SequenceMatcher(None, [key(x, RET, mode) for x in R], [key(x, 0, mode) for x in O], autojunk=False)
    m = sum(b.size for b in sm.get_matching_blocks())
    out.append(f'{name} {m}')
print(f'{len(built)}v{len(retail)} ins {len(O)}v{len(R)} frame {fr(O)} v {fr(R)} | ' + ' | '.join(out))
