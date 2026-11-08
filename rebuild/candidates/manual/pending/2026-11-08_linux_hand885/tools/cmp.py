#!/usr/bin/env python3
"""Structural comparator for the 74 KB row: cmp.py <obj> [--leaf L] [--ctx N] [--loose] [--from IDX]
Normalises ebp frame displacements (checks a consistent ours->retail map), relocations (consistent symbol->address map)
and branch targets (by instruction index), then reports the first differing instruction and a difflib share."""
import sys, os, re, csv, subprocess, difflib, collections
csv.field_size_limit(10**8)
sys.path.insert(0, "/home/user/FableDecomp/tools/decomp_pipeline")
import verify_and_land as V
from capstone import Cs, CS_ARCH_X86, CS_MODE_32
S = os.environ['S']; A = f'{S}/w22/agent885'
args = sys.argv[1:]
obj = args[0]
def opt(name, d=None):
    return args[args.index(name)+1] if name in args else d
leaf = opt('--leaf', '?RunCutsceneMacro@@')
ctx = int(opt('--ctx', '12'))
loose = '--loose' in args
RET = 0xCBFB7D
row = next(r for r in csv.DictReader(open(f'{S}/w22/chunk876.tsv', encoding='utf-8-sig'), delimiter='\t'))
retail = bytes.fromhex(row['bytes'])
while retail and retail[-1] in (0xcc, 0x90): retail = retail[:-1]
built, sec, sym = V.obj_text(obj, leaf)
# relocations of the selected section: offset -> symbol
pr = subprocess.run(['objdump', '-r', '-j', '.text', obj], capture_output=True, text=True)
# objdump -r prints all .text sections in order; pick the sec-th block
blocks = []; cur = None
for line in subprocess.run(['objdump', '-r', obj], capture_output=True, text=True).stdout.splitlines():
    m = re.match(r'RELOCATION RECORDS FOR \[(.*)\]:', line)
    if m:
        cur = {'name': m.group(1), 'r': {}}; blocks.append(cur); continue
    m = re.match(r'^([0-9a-f]{8}) (\S+)\s+(.*)$', line)
    if m and cur is not None: cur['r'][int(m.group(1), 16)] = m.group(3).strip()
# map section index (count of .text sections) to reloc block: sections are numbered by objdump -h order
hs = subprocess.run(['objdump', '-h', obj], capture_output=True, text=True).stdout
secnames = re.findall(r'^\s*\d+\s+(\S+)', hs, re.M)
textsecs = [i for i, n in enumerate(secnames) if n.startswith('.text')]
# V.obj_text counts .text-like sections in -d output; sec is that ordinal
relocs = {}
tblocks = [b for b in blocks if b['name'].startswith('.text')]
# the k-th .text block in -r output may skip sections without relocs; match by order of section names with relocs
want = None
k = 0
for i, n in enumerate(secnames):
    if n.startswith('.text'):
        if k == sec: want = i; break
        k += 1
# objdump -r blocks appear in section order for sections that have relocs; count .text sections with relocs before `want`
cnt = 0
with_rel = []
for b in blocks:
    with_rel.append(b)
# fallback: choose the block whose reloc offsets fit inside the function and is largest
best = None
for b in tblocks:
    if b['r'] and max(b['r']) < len(built) + 4:
        if best is None or len(b['r']) > len(best['r']): best = b
relocs = best['r'] if best else {}
md = Cs(CS_ARCH_X86, CS_MODE_32); md.detail = False
def dis(code, base):
    return [(i.address - base, i.size, i.mnemonic, i.op_str, bytes(i.bytes)) for i in md.disasm(code, base)]
R = dis(retail, RET); O = dis(built, 0)
Ridx = {a: k for k, (a, *_r) in enumerate(R)}; Oidx = {a: k for k, (a, *_r) in enumerate(O)}
JMP = re.compile(r'^(j\w+|call|loop\w*)$')
def norm(ins, base, idxmap, rel=None):
    off, size, mn, op, by = ins
    toks = {'frame': [], 'rel': None, 'tgt': None}
    o = op
    if rel is not None:
        # find reloc inside this instruction
        for ro in range(off, off+size):
            if ro in rel:
                toks['rel'] = rel[ro]; break
    if JMP.match(mn) and re.match(r'^0x[0-9a-f]+$', o):
        t = int(o, 16) - base
        toks['tgt'] = idxmap.get(t, ('ext', t))
        o = 'T'
    def fr(m):
        sign = -1 if m.group(1) == '-' else 1
        toks['frame'].append(sign * int(m.group(2), 16)); return '[ebp+F]'
    o = re.sub(r'\[ebp ([+-]) (0x[0-9a-f]+|\d+)\]', fr, o)
    o = re.sub(r'\[ebp\]', lambda m: (toks['frame'].append(0), '[ebp+F]')[1], o)
    if loose:
        o = re.sub(r'\b(esi|edi|ebx)\b', 'R', o); o = re.sub(r'\b(si|di|bx|bl|bh)\b', 'r', o)
    return mn, o, toks
mapF = {}; mapFr = {}; mapRel = {}; mapRelr = {}; conflicts = []
first = None
n = min(len(R), len(O))
for k in range(n):
    rmn, rop, rt = norm(R[k], RET, Ridx)
    omn, oop, ot = norm(O[k], 0, Oidx, relocs)
    same = (rmn == omn)
    if same and ot['rel'] is not None:
        # replace numeric immediates/addresses in retail operand by R and in ours by R
        rop2 = re.sub(r'0x[0-9a-f]{6,8}', 'R', rop); oop2 = re.sub(r'0x[0-9a-f]+|\b0\b', 'R', oop)
        same = rop2 == oop2 or rop2.replace('R', '') == oop2.replace('R', '')
        m = re.search(r'0x([0-9a-f]{6,8})', rop)
        if same and m:
            ra = int(m.group(1), 16); s = ot['rel']
            if mapRel.setdefault(s, ra) != ra or mapRelr.setdefault(ra, s) != s:
                conflicts.append((k, 'rel', s, hex(ra), hex(mapRel[s]), mapRelr[ra]))
    else:
        same = same and rop == oop
    if same and rt['tgt'] is not None and ot['tgt'] is not None:
        if isinstance(rt['tgt'], int) and isinstance(ot['tgt'], int) and rt['tgt'] != ot['tgt']:
            conflicts.append((k, 'tgt', R[k][0]+RET, rt['tgt'], ot['tgt']))
    if same:
        for a, b in zip(ot['frame'], rt['frame']):
            if mapF.setdefault(a, b) != b or mapFr.setdefault(b, a) != a:
                conflicts.append((k, 'frame', hex(a & 0xffffffff), hex(b & 0xffffffff), hex(mapF[a] & 0xffffffff)))
    if not same:
        first = k; break
print(f'retail {len(retail)} bytes / {len(R)} ins; ours {len(built)} bytes / {len(O)} ins; symbol {sym[:80]}')
if first is None: first = n
print(f'FIRST STRUCTURAL DIFF at ins {first} (retail 0x{(R[first][0]+RET) if first < len(R) else 0:08x}, ours +0x{O[first][0] if first < len(O) else 0:x}); prefix share {first/len(R):.1%}')
kinds = collections.Counter(c[1] for c in conflicts)
print('conflicts before it:', dict(kinds))
for c in [c for c in conflicts if c[1] != 'frame'][:8]: print('  ', c)
for c in [c for c in conflicts if c[1] == 'frame'][:4]: print('  ', c)
lo = max(0, first - ctx); hi = first + ctx
print('--- retail' + ' ' * 52 + '--- ours')
for k in range(lo, hi):
    r = R[k] if k < len(R) else None; o = O[k] if k < len(O) else None
    rs = f'{r[0]+RET:08x} {r[2]} {r[3]}' if r else ''
    os_ = f'{o[0]:05x} {o[2]} {o[3]}' if o else ''
    mark = '>>' if k == first else '  '
    print(f'{mark}{rs[:60]:60s} | {os_[:60]}')
if '--share' in args:
    sm = difflib.SequenceMatcher(None, [norm(x, RET, Ridx)[0] + ' ' + norm(x, RET, Ridx)[1] for x in R],
                                 [norm(x, 0, Oidx, relocs)[0] + ' ' + norm(x, 0, Oidx, relocs)[1] for x in O], autojunk=False)
    m = sum(b.size for b in sm.get_matching_blocks())
    print(f'difflib share: {m}/{len(R)} = {m/len(R):.1%}')
if '--hunks' in args:
    K = int(opt('--hunks', '10'))
    def key(x, base):
        mn, o, t = norm(x, base, {}, relocs if base == 0 else None)
        if base == 0 and t['rel'] is not None:
            o = re.sub(r'0x[0-9a-f]+|\b0\b', 'R', o)
        o = re.sub(r'0x[0-9a-f]{6,8}\b', lambda m: 'R' if int(m.group(0), 16) >= 0x400000 else m.group(0), o)
        if loose:
            o = re.sub(r'\b(eax|ecx|edx|esi|edi|ebx)\b', 'R', o)
        return mn + ' ' + o
    rk = [key(x, RET) for x in R]; ok = [key(x, 0) for x in O]
    sm = difflib.SequenceMatcher(None, rk, ok, autojunk=False)
    m = sum(b.size for b in sm.get_matching_blocks())
    print(f'difflib matched {m}/{len(R)} retail ins = {m/len(R):.1%} (ours {len(O)})')
    shown = 0
    for tag, i1, i2, j1, j2 in sm.get_opcodes():
        if tag == 'equal': continue
        if shown >= K: break
        shown += 1
        print(f'@@ {tag} retail[{i1}:{i2}] 0x{R[i1][0]+RET if i1 < len(R) else 0:08x} ours[{j1}:{j2}] +0x{O[j1][0] if j1 < len(O) else 0:x}')
        for k in range(i1, min(i2, i1 + 6)): print(f'  - {R[k][0]+RET:08x} {R[k][2]} {R[k][3]}')
        if i2 - i1 > 6: print(f'  - ... {i2-i1} retail ins')
        for k in range(j1, min(j2, j1 + 6)): print(f'  + {O[k][0]:05x} {O[k][2]} {O[k][3]}')
        if j2 - j1 > 6: print(f'  + ... {j2-j1} ours ins')
if '--big' in args:
    K = int(opt('--big', '20'))
    def key2(x, base):
        mn, o, t = norm(x, base, {}, relocs if base == 0 else None)
        if base == 0 and t['rel'] is not None:
            o = re.sub(r'0x[0-9a-f]+|\b0\b', 'R', o)
        o = re.sub(r'0x[0-9a-f]{6,8}\b', lambda m: 'R' if int(m.group(0), 16) >= 0x400000 else m.group(0), o)
        o = re.sub(r'\b(eax|ecx|edx|esi|edi|ebx)\b', 'R', o); o = re.sub(r'\b(al|cl|dl|bl|ah|bh)\b', 'r', o)
        o = re.sub(r'\[ebp\+F\]', 'M', o)
        return mn + ' ' + o
    sm = difflib.SequenceMatcher(None, [key2(x, RET) for x in R], [key2(x, 0) for x in O], autojunk=False)
    m = sum(b.size for b in sm.get_matching_blocks())
    print(f'loose difflib matched {m}/{len(R)} = {m/len(R):.1%}')
    ann = {}
    for k, l in enumerate(open(f'{A}/probe/ann.txt'), 1):
        mm = re.match(r'^([0-9a-f]{8}) ', l)
        if mm: ann[int(mm.group(1), 16)] = k
    hs = [(abs((i2-i1)-(j2-j1)), tag, i1, i2, j1, j2) for tag, i1, i2, j1, j2 in sm.get_opcodes() if tag != 'equal']
    tot_r = sum(i2-i1 for _, t, i1, i2, j1, j2 in hs); tot_o = sum(j2-j1 for _, t, i1, i2, j1, j2 in hs)
    print(f'non-equal: retail {tot_r} ins, ours {tot_o} ins, {len(hs)} hunks')
    for d, tag, i1, i2, j1, j2 in sorted(hs, reverse=True)[:K]:
        ra = R[i1][0]+RET if i1 < len(R) else 0
        print(f'{tag:7s} d={d:4d} retail[{i1}:{i2}] ann {ann.get(ra, "?")} 0x{ra:x} n={i2-i1} | ours[{j1}:{j2}] +0x{O[j1][0] if j1 < len(O) else 0:x} n={j2-j1}')

if '--bigseq' in args:
    # loose hunks in address order with retail ann line numbers and both sides' instructions
    def key3(x, base):
        mn, o, t = norm(x, base, {}, relocs if base == 0 else None)
        if base == 0 and t['rel'] is not None:
            o = re.sub(r'0x[0-9a-f]+|\b0\b', 'R', o)
        o = re.sub(r'0x[0-9a-f]{6,8}\b', lambda m: 'R' if int(m.group(0), 16) >= 0x400000 else m.group(0), o)
        o = re.sub(r'\b(eax|ecx|edx|esi|edi|ebx)\b', 'R', o); o = re.sub(r'\b(al|cl|dl|bl|ah|bh)\b', 'r', o)
        o = re.sub(r'\[ebp\+F\]', 'M', o)
        return mn + ' ' + o
    sm = difflib.SequenceMatcher(None, [key3(x, RET) for x in R], [key3(x, 0) for x in O], autojunk=False)
    ann = {}
    for k, l in enumerate(open(f'{A}/probe/ann.txt'), 1):
        mm = re.match(r'^([0-9a-f]{8}) ', l)
        if mm: ann[int(mm.group(1), 16)] = k
    lo = int(opt('--from', '0')); K = int(opt('--bigseq', '40')); shown = 0
    for tag, i1, i2, j1, j2 in sm.get_opcodes():
        if tag == 'equal' or i1 < lo: continue
        if shown >= K: break
        shown += 1
        ra = R[i1][0]+RET if i1 < len(R) else 0
        print(f'@@ {tag} retail[{i1}:{i2}] ann {ann.get(ra, "?")} 0x{ra:x} | ours +0x{O[j1][0] if j1 < len(O) else 0:x}')
        for k in range(i1, min(i2, i1 + 8)): print(f'  - {R[k][0]+RET:08x} {R[k][2]} {R[k][3]}')
        if i2 - i1 > 8: print(f'  - ... {i2-i1} retail ins')
        for k in range(j1, min(j2, j1 + 8)): print(f'  + {O[k][0]:05x} {O[k][2]} {O[k][3]}')
        if j2 - j1 > 8: print(f'  + ... {j2-j1} ours ins')
