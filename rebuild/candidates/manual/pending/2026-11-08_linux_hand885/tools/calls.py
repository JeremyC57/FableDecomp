#!/usr/bin/env python3
"""Call-sequence diff for the 74 KB row: calls.py [--ctx N] [--max M]
Retail call targets (addresses) vs our call relocations (symbols mapped to retail addresses through
rebuild/oracles/reloc_bindings.tsv, then learned from 1:1 replace blocks). Prints the differing regions with the
retail ann.txt line numbers and our offsets, so source-level order/missing-call problems show up independent of
registers and frame slots."""
import sys, os, re, csv, difflib, collections, subprocess
csv.field_size_limit(10**8)
sys.path.insert(0, "/home/user/FableDecomp/tools/decomp_pipeline")
import verify_and_land as V
from capstone import Cs, CS_ARCH_X86, CS_MODE_32
S = os.environ['S']; A = f'{S}/w22/agent885'
args = sys.argv[1:]
def opt(n, d=None): return args[args.index(n)+1] if n in args else d
ctx = int(opt('--ctx', '3')); mx = int(opt('--max', '40'))
obj = os.environ.get('CALLS_OBJ', f'{S}/wine/q/00cbfb7d/00cbfb7d.obj')
leaf = open(f'{A}/src/00cbfb7d.cpp').read().split('// Leaf: ')[1].split('\n')[0].strip()
RET = 0xCBFB7D
row = next(r for r in csv.DictReader(open(f'{S}/w22/chunk876.tsv', encoding='utf-8-sig'), delimiter='\t'))
retail = bytes.fromhex(row['bytes'])
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
rel = best['r']
bind = {}
for r in csv.DictReader(open('/home/user/FableDecomp/rebuild/oracles/reloc_bindings.tsv', encoding='utf-8'), delimiter='\t'):
    bind.setdefault(r['symbol'], int(r['target'], 16))
md = Cs(CS_ARCH_X86, CS_MODE_32)
# retail ann line numbers by address
annline = {}
for k, l in enumerate(open(f'{A}/probe/ann.txt'), 1):
    m = re.match(r'^([0-9a-f]{8}) ', l)
    if m: annline[int(m.group(1), 16)] = k
Rc = []  # (addr, target, kind)
for i in md.disasm(retail, RET):
    if i.mnemonic == 'call':
        if i.op_str.startswith('0x'): Rc.append((i.address, int(i.op_str, 16)))
        else: Rc.append((i.address, i.op_str.replace('dword ptr ', '')))
Oc = []
for i in md.disasm(built, 0):
    if i.mnemonic == 'call':
        s = None
        for ro in range(i.address, i.address + i.size):
            if ro in rel: s = rel[ro]; break
        if s is not None: Oc.append((i.address, s))
        else: Oc.append((i.address, i.op_str.replace('dword ptr ', '')))
learn = dict(bind)
def okey(s):
    return learn.get(s, s)
for it in range(3):
    a = [t for _, t in Rc]; b = [okey(t) for _, t in Oc]
    sm = difflib.SequenceMatcher(None, a, b, autojunk=False)
    for tag, i1, i2, j1, j2 in sm.get_opcodes():
        if tag == 'replace' and i2 - i1 == j2 - j1:
            for x, y in zip(range(i1, i2), range(j1, j2)):
                rs = Rc[x][1]; os_ = Oc[y][1]
                if isinstance(rs, int) and isinstance(os_, str) and os_ not in learn:
                    learn[os_] = rs
a = [t for _, t in Rc]; b = [okey(t) for _, t in Oc]
sm = difflib.SequenceMatcher(None, a, b, autojunk=False)
m = sum(bl.size for bl in sm.get_matching_blocks())
print(f'calls retail {len(a)} ours {len(b)} matched {m} ({m/len(a):.1%})')
def rname(t):
    return hex(t) if isinstance(t, int) else t
n = 0
for tag, i1, i2, j1, j2 in sm.get_opcodes():
    if tag == 'equal': continue
    n += 1
    if n > mx: break
    ra = Rc[i1][0] if i1 < len(Rc) else 0
    oa = Oc[j1][0] if j1 < len(Oc) else 0
    print(f'@@ {tag} retail[{i1}:{i2}] ann {annline.get(ra, "?")} 0x{ra:x} | ours[{j1}:{j2}] +0x{oa:x}')
    for x in range(max(0, i1 - ctx), i1):
        print(f'   = {Rc[x][0]:08x} {rname(Rc[x][1])}')
    for x in range(i1, min(i2, i1 + 12)):
        print(f'   - {Rc[x][0]:08x} {rname(Rc[x][1])}  (ann {annline.get(Rc[x][0], "?")})')
    for y in range(j1, min(j2, j1 + 12)):
        print(f'   + {Oc[y][0]:05x} {Oc[y][1][:90]}')
if '--counts' in args:
    ca = collections.Counter(a); cb = collections.Counter(b)
    inv = {}
    for s, t in learn.items(): inv.setdefault(t, s)
    for k in sorted(set(ca) | set(cb), key=lambda k: -abs(ca[k] - cb[k])):
        if ca[k] != cb[k]:
            nm = inv.get(k, '') if isinstance(k, int) else ''
            print(f'{ca[k]:5d} {cb[k]:5d} {rname(k)} {nm[:100]}')
