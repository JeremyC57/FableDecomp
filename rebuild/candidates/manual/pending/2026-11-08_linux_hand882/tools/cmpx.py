#!/usr/bin/env python3
"""cmpx.py <obj> <addr> <leaf> [--first N] [--mask regs|frame|both] [--segs] [--side from n]
Generalised from hand885 m3.py/first.py: retail bytes from $S/w22/chunk<NNN>.tsv (any chunk tsv holding the address).
 - default: m3 shares (exact / regs masked / regs+frame masked) + frame size
 - --first N: first N differing instructions (by index alignment through difflib opcodes) with the chosen mask
 - --segs: call-segment diff: split both at calls, list segments that differ (masked by --mask level)
"""
import sys, os, re, csv, glob, difflib, subprocess
csv.field_size_limit(10**8)
sys.path.insert(0, "/home/user/FableDecomp/tools/decomp_pipeline")
import verify_and_land as V
from capstone import Cs, CS_ARCH_X86, CS_MODE_32
S = os.environ.get('S', '/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad')
obj, addr, leaf = sys.argv[1], sys.argv[2].lower(), sys.argv[3]
args = sys.argv[4:]
def opt(name, default=None):
    return args[args.index(name) + 1] if name in args else default
retail = None
for f in glob.glob(f'{S}/w22/chunk88*.tsv'):
    for r in csv.DictReader(open(f, encoding='utf-8-sig'), delimiter='\t'):
        if r['address'].lower() == addr: retail = bytes.fromhex(r['bytes']); break
    if retail: break
while retail and retail[-1] in (0xcc, 0x90): retail = retail[:-1]
RET = int(addr, 16)
built, sec, sym = V.obj_text(obj, leaf)
# relocations of our function's section
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
JMP = re.compile(r'^(j\w+|call|loop\w*)$')
def key(ins, base, mode):
    off, size, mn, op = ins
    o = op
    if JMP.match(mn) and re.match(r'^0x[0-9a-f]+$', o): o = 'T'
    if mode in (2, 3):
        o = re.sub(r'\[esp(?: [+-] (?:0x[0-9a-f]+|\d+))?\]', 'M', o)
        o = re.sub(r'\[ebp(?: [+-] (?:0x[0-9a-f]+|\d+))?\]', 'M', o)
    if base == 0:
        if any(r in rel for r in range(off, off + size)): o = re.sub(r'0x[0-9a-f]+|\b0\b', 'R', o)
    else:
        o = re.sub(r'0x[0-9a-f]{6,8}\b', lambda m: 'R' if 0x400000 <= int(m.group(0), 16) < 0x1800000 else m.group(0), o)
    if mode in (1, 3):
        o = re.sub(r'\b(eax|ecx|edx|esi|edi|ebx|ebp)\b', 'R', o); o = re.sub(r'\b(al|cl|dl|bl|ah|bh|si|di|bx)\b', 'r', o)
    if mode in (2, 3):
        o = re.sub(r'\[esp(?: [+-] (?:0x[0-9a-f]+|\d+))?\]', 'M', o)
        o = re.sub(r'\[ebp(?: [+-] (?:0x[0-9a-f]+|\d+))?\]', 'M', o)
    return mn + ' ' + o
MODES = {'none': 0, 'regs': 1, 'frame': 2, 'both': 3}
fr = lambda L: next((i[3] for i in L[:8] if i[2] == 'sub' and i[3].startswith('esp')), '?')
if '--first' in args or '--side' in args or '--segs' in args:
    mode = MODES[opt('--mask', 'none')]
    KR = [key(x, RET, mode) for x in R]; KO = [key(x, 0, mode) for x in O]
    if '--first' in args:
        N = int(opt('--first'))
        sm = difflib.SequenceMatcher(None, KR, KO, autojunk=False)
        n = 0
        for tag, i1, i2, j1, j2 in sm.get_opcodes():
            if tag == 'equal': continue
            print(f'{tag} retail[{i1}:{i2}] @0x{RET+R[i1][0] if i1 < len(R) else 0:08x} ours[{j1}:{j2}]')
            for k in range(i1, min(i2, i1 + 6)): print(f'   - {R[k][2]} {R[k][3]}')
            for k in range(j1, min(j2, j1 + 6)): print(f'   + {O[k][2]} {O[k][3]}')
            n += 1
            if n >= N: break
    if '--segs' in args:
        def segs(L, K):
            out = []; cur = []
            for x, k in zip(L, K):
                cur.append((x, k))
                if x[2] == 'call': out.append(cur); cur = []
            out.append(cur); return out
        SR = segs(R, KR); SO = segs(O, KO)
        print(f'segments retail {len(SR)} ours {len(SO)}')
        nd = 0
        for idx, (a, b) in enumerate(zip(SR, SO)):
            if [k for _, k in a] != [k for _, k in b]:
                nd += 1
                if '-v' in args:
                    print(f'--- seg {idx} @0x{RET + a[0][0][0]:08x} retail {len(a)} ours {len(b)}')
                    for l in difflib.unified_diff([k for _, k in a], [k for _, k in b], lineterm='', n=1):
                        if not l.startswith(('---', '+++')): print('   ', l)
                else:
                    print(f'seg {idx} @0x{RET + a[0][0][0]:08x} retail {len(a)} ours {len(b)}')
        print('differing segments', nd)
else:
    out = []
    for mode, name in ((0, 'exact'), (1, 'regs'), (3, 'regs+frame')):
        sm = difflib.SequenceMatcher(None, [key(x, RET, mode) for x in R], [key(x, 0, mode) for x in O], autojunk=False)
        m = sum(b.size for b in sm.get_matching_blocks())
        out.append(f'{name} {m/len(R):.2%}')
    print(f'{len(built)}v{len(retail)} ins {len(O)}v{len(R)} frame {fr(O)} v {fr(R)} | ' + ' | '.join(out))
if '--range' in args:
    lo_, hi_ = int(opt('--range').split(':')[0], 16), int(opt('--range').split(':')[1], 16)
    mode = MODES[opt('--mask', 'none')]
    KR = [key(x, RET, mode) for x in R]; KO = [key(x, 0, mode) for x in O]
    sm = difflib.SequenceMatcher(None, KR, KO, autojunk=False)
    for tag, i1, i2, j1, j2 in sm.get_opcodes():
        for k in range(max(i1, i2 - 0) if False else i1, i2 if tag != 'insert' else i1):
            pass
        n = max(i2 - i1, j2 - j1)
        for t in range(n):
            ri = i1 + t if i1 + t < i2 else None; oi = j1 + t if j1 + t < j2 else None
            va = RET + R[ri][0] if ri is not None else (RET + R[i1][0] if i1 < len(R) else 0)
            if not (lo_ <= va < hi_): continue
            rs = f'{R[ri][2]} {R[ri][3]}' if ri is not None else ''
            os_ = f'{O[oi][2]} {O[oi][3]}' if oi is not None else ''
            print(f'{va:08x} {"  " if tag == "equal" else "**"} {rs:44.44s} | {os_}')
