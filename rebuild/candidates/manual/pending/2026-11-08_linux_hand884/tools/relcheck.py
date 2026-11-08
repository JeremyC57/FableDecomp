#!/usr/bin/env python3
"""relcheck.py <obj> [leaf]: relocation identity for the matched RegisterTCs object.
Pairs every relocation in our function with the retail operand at the same offset (the code is byte-identical outside
relocations), then checks: one symbol <-> one retail address; string literals equal the retail string; ?Alloc@X / ?StatGetClassTypeName@X
targets are the ones regs.tsv derived for class X (the retail callee's literal for StatGetClassTypeName); prints the symbol -> address table
of every non-class callee/global so it can be compared with reloc_bindings."""
import sys, os, re, csv, struct, subprocess, collections
sys.path.insert(0, "/home/user/FableDecomp/tools/decomp_pipeline")
import verify_and_land as V
S = os.environ.get('S', '/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad')
A = f'{S}/w22/agent884'
obj = sys.argv[1]; leaf = sys.argv[2] if len(sys.argv) > 2 and not sys.argv[2].startswith('--') else '?RegisterTCs@CTCBase@@SIXXZ'
exe = open(f"{S}/dl/game/Fable The Lost Chapters/Fable.exe", "rb").read()
pe = struct.unpack_from("<I", exe, 0x3c)[0]
nsec = struct.unpack_from("<H", exe, pe+6)[0]; opt = struct.unpack_from("<H", exe, pe+20)[0]
secs = []
for i in range(nsec):
    o = pe+24+opt+40*i; vs, va, rs, ro = struct.unpack_from("<IIII", exe, o+8); secs.append((va, max(vs, rs), ro))
def rd(va, n):
    for sva, rs, ro in secs:
        if sva <= va-0x400000 < sva+rs:
            off = va-0x400000-sva+ro; return exe[off:off+n]
def cstr(va):
    b = rd(va, 300); return b[:b.find(b'\0')]
START = 0x004EE23F
built, sec, sym = V.obj_text(obj, leaf)
retail = rd(START, len(built))
# relocations of our function's section: objdump -dr on the symbol block
out = subprocess.run(['objdump', '-dr', obj], capture_output=True, text=True).stdout
blk = None
for b in out.split('Disassembly of section'):
    if f'<{sym}>:' in b: blk = b; break
rels = [(int(m.group(1), 16), m.group(2), m.group(3)) for m in re.finditer(r'^\s+([0-9a-f]+): (DISP32|dir32|DIR32|REL32)\s+(\S+)', blk, re.M)]
# string literal contents from the object (.rdata COMDATs ??_C@...): read via objdump -s per section is heavy; decode the mangled literal instead
def lit_from_mangled(s):
    m = re.match(r'\?\?_C@_0[^@]*@[^@]*@(.*)\$AA@$', s)
    if not m: return None
    t = m.group(1)
    t = t.replace('?$AA', '\0')
    return t
regs = [l.rstrip('\n').split('\t') for l in open(f'{A}/probe/regs.tsv')][1:]
alloc_of = {}; stat_of = {}
for r in regs:
    if r[1] in ('TC', 'DEF'): alloc_of[r[2]] = int(r[3], 16)
    if r[1] == 'TC' and r[5] != 'inl': stat_of[r[2]] = int(r[5], 16)
sym2a = {}; a2sym = collections.defaultdict(set); bad = []
for off, kind, s in rels:
    raw = struct.unpack_from('<I', retail, off)[0]
    ours = struct.unpack_from('<I', built, off)[0]
    if kind in ('REL32', 'DISP32'):
        tgt = START + off + 4 + struct.unpack_from('<i', retail, off)[0]
    else:
        tgt = raw - ours   # dir32 with addend: retail = symbol + addend
    if sym2a.setdefault(s, tgt) != tgt: bad.append(('multi-addr', s, hex(tgt), hex(sym2a[s])))
    a2sym[tgt].add(s)
    m = re.match(r'\?Alloc@(\w+)@', s)
    if m and alloc_of.get(m.group(1)) != tgt: bad.append(('alloc', s, hex(tgt), hex(alloc_of.get(m.group(1), 0))))
    m = re.match(r'\?StatGetClassTypeName@(\w+)@', s)
    if m:
        b = rd(tgt, 8)
        lit = cstr(struct.unpack_from('<I', b, 4)[0]).decode()
        if lit != m.group(1) or stat_of.get(m.group(1)) != tgt: bad.append(('statname', s, hex(tgt), lit))
    if s.startswith('??_C@'):
        rs = cstr(tgt).decode('latin1')
        m = re.match(r'\?\?_C@_0[^@]*@[^@]*@(.*)$', s)
        t = m.group(1).replace('?$AA@', '').rstrip('@') if m else ''
        if not (rs.startswith(t) and len(t) >= min(len(rs), 20)): bad.append(('string', s, hex(tgt), rs))
for a, ss in a2sym.items():
    if len(ss) > 1: bad.append(('multi-sym', hex(a), sorted(ss)))
print(f'{len(rels)} relocations, {len(sym2a)} symbols, {len(bad)} problems')
for b in bad[:30]: print('  BAD', b)
if '--table' in sys.argv:
    for s, a in sorted(sym2a.items(), key=lambda t: t[1]):
        if s.startswith(('?Alloc@', '?StatGetClassTypeName@', '??_C@')): continue
        print(f'{a:08x}\t{s}')
