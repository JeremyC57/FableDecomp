#!/usr/bin/env python3
"""dd.py <addr> <obj> [--leaf L] [--ctx N] [--hunks N] [--loose] [--full]
Diff localisation for agent890 rows (esp-framed functions): retail bytes from chunk890.tsv vs the leaf's bytes in <obj>.
Relocations (ours: objdump -r; retail: absolute addresses >= 0x400000) are masked, branch targets compared by instruction
index. Prints sizes, the first differing instruction (strict: registers and frame offsets kept), difflib shares
(strict / loose = registers + [esp+N] masked) and the differing hunks in address order."""
import sys, os, re, csv, subprocess, difflib
csv.field_size_limit(10**8)
sys.path.insert(0, "/home/user/FableDecomp/tools/decomp_pipeline")
import verify_and_land as V
from capstone import Cs, CS_ARCH_X86, CS_MODE_32
S = os.environ.get('S', '/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad')
A = f'{S}/w22/agent890'
args = sys.argv[1:]
def opt(n, d=None): return args[args.index(n) + 1] if n in args else d
addr = args[0].lower(); obj = args[1]
RET = int(addr, 16)
rows = {r['address'].lower(): r for r in csv.DictReader(open(f'{S}/w22/chunk890.tsv', encoding='utf-8-sig'), delimiter='\t')}
LEAF0 = {'00551ea0': '?ConstructSpritesToDraw@CTable@NUISystem@@', '0056ad40': '?UpdateUI_QuestMenu@CQuestManager@@',
        '00498490': '?DrawRetailDisplay@CProgressDisplay@@'}
leaf = opt('--leaf', '?Activate@CTCBoastUI@@')
retail = bytes.fromhex(rows[addr]['bytes'])
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
Ridx = {a: k for k, (a, *_x) in enumerate(R)}; Oidx = {a: k for k, (a, *_x) in enumerate(O)}
JMP = re.compile(r'^(j\w+|call|loop\w*)$')
def key(ins, base, idx, ours, loose=False):
    off, size, mn, op = ins
    o = op
    hasrel = ours and any(r in rel for r in range(off, off + size))
    if JMP.match(mn) and re.match(r'^0x[0-9a-f]+$', o):
        t = int(o, 16) - base
        if mn == 'call': o = 'CALL'
        else: o = 'T%+d' % (idx.get(t, 10**6) - idx.get(off, 0))
    elif hasrel:
        o = re.sub(r'0x[0-9a-f]+|\b0\b', 'R', o)
    else:
        o = re.sub(r'0x[0-9a-f]{6,8}\b', lambda m: 'R' if int(m.group(0), 16) >= 0x400000 else m.group(0), o)
    if loose:
        o = re.sub(r'\[esp \+ 0x[0-9a-f]+\]|\[esp\]', '[M]', o)
        o = re.sub(r'\b(eax|ecx|edx|esi|edi|ebx|ebp)\b', 'R', o); o = re.sub(r'\b(al|cl|dl|bl|ah|bh|ax|cx|dx)\b', 'r', o)
    return mn + ' ' + o
rk = [key(x, RET, Ridx, False) for x in R]; ok = [key(x, 0, Oidx, True) for x in O]
rl = [key(x, RET, Ridx, False, True) for x in R]; ol = [key(x, 0, Oidx, True, True) for x in O]
FM = lambda k: re.sub(r'\[esp \+ 0x[0-9a-f]+\]|\[esp\]', '[M]', k)
_sf = difflib.SequenceMatcher(None, [FM(k) for k in rk], [FM(k) for k in ok], autojunk=False)
MF = sum(b.size for b in _sf.get_matching_blocks())
first = next((k for k in range(min(len(R), len(O))) if rk[k] != ok[k]), min(len(R), len(O)))
sm = difflib.SequenceMatcher(None, rk, ok, autojunk=False); ms = sum(b.size for b in sm.get_matching_blocks())
sl = difflib.SequenceMatcher(None, rl, ol, autojunk=False); ml = sum(b.size for b in sl.get_matching_blocks())
print(f'{addr}: retail {len(retail)} B / {len(R)} ins; ours {len(built)} B / {len(O)} ins; strict {ms}/{len(R)} = {ms/len(R):.1%}; loose {ml}/{len(R)} = {ml/len(R):.1%}; noframe {MF}/{len(R)} = {MF/len(R):.1%}')
print(f'first diff at ins {first}: retail 0x{(R[first][0]+RET) if first < len(R) else 0:08x}, ours +0x{O[first][0] if first < len(O) else 0:x}')
ctx = int(opt('--ctx', '6'))
for k in range(max(0, first - ctx), first + ctx):
    r = R[k] if k < len(R) else None; o = O[k] if k < len(O) else None
    rs = f'{r[0]+RET:08x} {r[2]} {r[3]}' if r else ''
    os_ = f'+{o[0]:04x} {o[2]} {o[3]}' + (' [R]' if o and any(x in rel for x in range(o[0], o[0]+o[1])) else '') if o else ''
    print(f"{'>>' if k == first else '  '}{rs[:62]:62s} | {os_[:62]}")
H = int(opt('--hunks', '0'))
if H:
    use = sl if '--loose' in args else (_sf if '--nf' in args else sm)
    n = 0
    for tag, i1, i2, j1, j2 in use.get_opcodes():
        if tag == 'equal': continue
        n += 1
        if n > H: break
        ra = R[i1][0] + RET if i1 < len(R) else 0
        print(f'@@ {tag} retail[{i1}:{i2}] 0x{ra:08x} | ours[{j1}:{j2}] +0x{O[j1][0] if j1 < len(O) else 0:x}')
        lim = 1000 if '--full' in args else 8
        for k in range(i1, min(i2, i1 + lim)): print(f'  - {R[k][0]+RET:08x} {R[k][2]} {R[k][3]}')
        if i2 - i1 > lim: print(f'  - ... {i2-i1} retail ins')
        for k in range(j1, min(j2, j1 + lim)): print(f'  + +{O[k][0]:04x} {O[k][2]} {O[k][3]}')
        if j2 - j1 > lim: print(f'  + ... {j2-j1} ours ins')
SB = opt('--sbs')
if SB:
    lo, hi = [int(x, 16) for x in SB.split(':')]
    for tag, i1, i2, j1, j2 in sl.get_opcodes():
        n = max(i2 - i1, j2 - j1)
        for t in range(n):
            i = i1 + t; j = j1 + t
            r = R[i] if i < i2 else None; o = O[j] if j < j2 else None
            ra = (r[0] + RET) if r else None
            if ra is not None and not (lo <= ra < hi): continue
            if ra is None and not (i1 < len(R) and lo <= R[min(i1, len(R)-1)][0] + RET < hi): continue
            rs = f'{ra:08x} {r[2]} {r[3]}' if r else ''
            os_ = f'+{o[0]:04x} {o[2]} {o[3]}' if o else ''
            mark = '  ' if tag == 'equal' and (rk[i] == ok[j]) else ('~ ' if tag == 'equal' else '! ')
            print(f'{mark}{rs[:58]:58s} | {os_[:58]}')
