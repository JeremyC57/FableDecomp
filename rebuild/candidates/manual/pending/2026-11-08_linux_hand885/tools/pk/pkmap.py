#!/usr/bin/env python3
"""pkmap.py <pk.json> <pk.asm> <ours.obj> [--out items.tsv]: per frame item (c2 packer order): its slot (probe events),
our [ebp+X] offset (sim.py layout of the probe's slot array, checked against /FAs) and retail's offset (votes from the
regs-masked alignment of ours v retail 0x00CBFB7D, keyed by the /FAs symbol name AND our displacement)."""
import sys, os, re, csv, json, difflib, collections, struct
csv.field_size_limit(10**8)
sys.path.insert(0, "/home/user/FableDecomp/tools/decomp_pipeline")
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import verify_and_land as V
import sim
from capstone import Cs, CS_ARCH_X86, CS_MODE_32
S = os.environ['S']
pkj, asmf, obj = sys.argv[1:4]
outp = sys.argv[sys.argv.index('--out') + 1] if '--out' in sys.argv else None
ev = json.load(open(pkj))
i0 = [k for k, e in enumerate(ev) if e[0] == 'list'][0]
L = ev[i0][1]
slot_of = {}; cur = None
for e in ev[i0 + 1:]:
    if e[0] == 'pipe': break
    if e[0] == 'first': slot_of[e[1]] = e[2]
    elif e[0] == 'place': cur = e[1]
    elif e[0] in ('join2', 'new'): slot_of[cur] = e[1]
pre = [e for e in ev if e[0] == 'presort'][0]
SL = sim.slots_from_raw(pre[3]); nparams = pre[2]
v, _, _ = sim.layout(SL, nparams)
soff = {}; top = 0x68
for s in v[nparams:]:
    a = min(max(s['align'], 1), 4); o = top - s['size']; o -= o % a; soff[s['id']] = o; top = o
t = open(asmf, errors='replace').read().splitlines()
a0 = [k for k, l in enumerate(t) if 'RunCutsceneMacro@@YIX' in l and 'PROC' in l][0]
a1 = [k for k in range(a0, len(t)) if 'ENDP' in t[k] and 'RunCutsceneMacro' in t[k]][0]
tab = collections.defaultdict(list)
j = a0 - 1
while j >= 0:
    m = re.match(r'^(\S+)\s*=\s*(-?\d+)\s*;\s*size = (\d+)', t[j])
    if not m: break
    tab[m.group(1)].append(int(m.group(2))); j -= 1
asm_ins = [l for l in t[a0 + 1:a1] if l.startswith('\t')]
SYM = re.compile(r'(\$T\d+|tv\d+|_\w+?\$\d*)\[ebp(?:([+-]\d+))?\]')
refs = [[(m.group(1), int(m.group(2) or 0)) for m in SYM.finditer(l)] for l in asm_ins]
row = next(r for r in csv.DictReader(open(f'{S}/w22/chunk876.tsv', encoding='utf-8-sig'), delimiter='\t'))
retail = bytes.fromhex(row['bytes'])
built, sec, sym = V.obj_text(obj, '?RunCutsceneMacro@@')
md = Cs(CS_ARCH_X86, CS_MODE_32)
R = [(i.mnemonic, i.op_str) for i in md.disasm(retail, 0xCBFB7D)]
O = [(i.mnemonic, i.op_str) for i in md.disasm(built, 0)]
for k, x in enumerate(R):
    if x == ('ret', '0x10'): R = R[:k + 1]; break
O = O[:len(asm_ins)]
FR = re.compile(r'\[ebp(?: ([+-]) (0x[0-9a-f]+|\d+))?\]')
def offs(op):
    return [0 if m.group(1) is None else (-1 if m.group(1) == '-' else 1) * int(m.group(2), 16) for m in FR.finditer(op)]
def key(mn, op):
    o = FR.sub('M', op); o = re.sub(r'0x[0-9a-f]{5,8}\b|\b0\b', 'K', o)
    o = re.sub(r'\b(eax|ecx|edx|esi|edi|ebx)\b', 'R', o); o = re.sub(r'\b(al|cl|dl|bl|ah|bh)\b', 'r', o)
    return mn + ' ' + o
sm = difflib.SequenceMatcher(None, [key(*x) for x in R], [key(*x) for x in O], autojunk=False)
votes = collections.defaultdict(collections.Counter)
ok_asm = 0
for b in range(len(O)):
    od = offs(O[b][1]); so = refs[b]
    if len(od) == len(so) and so: ok_asm += 1
for tag, x1, x2, y1, y2 in sm.get_opcodes():
    if tag != 'equal': continue
    for a, b in zip(range(x1, x2), range(y1, y2)):
        ro = offs(R[a][1]); od = offs(O[b][1]); so = refs[b]
        if len(ro) == len(so) == len(od) and so:
            for (nm, k), d, r in zip(so, od, ro): votes[(nm, d - k)][r - k] += 1
def tname(it):
    idx, size, w, kind, fl, nm, tn = it[:7]
    if nm is None and tn is None: return 'tv%d' % struct.unpack_from('<I', bytes.fromhex(it[7]), 28)[0]
    if nm is None: return '$T%d' % tn
    for c in ('%s$%d' % (nm, tn), '%s$' % nm):
        if c in tab: return c
    return '%s$?%d' % (nm, tn)
rows = []; fas_bad = 0
for it in L:
    n = tname(it); s = slot_of.get(it[0]); our = soff.get(s)
    if n in tab and our not in tab[n]:
        alt = our + SL[s]['size'] - it[1] if our is not None else None
        if alt in tab[n]: our = alt
        elif len(tab[n]) == 1: our = tab[n][0]; fas_bad += 1
        else: fas_bad += 1
    vv = votes.get((n, our))
    best, nb, tot = (None, 0, 0)
    if vv: (best, nb), = vv.most_common(1); tot = sum(vv.values())
    rows.append(dict(idx=it[0], size=it[1], w=it[2], kind=it[3], fl=it[4], name=n, slot=s, our=our, ret=best, nv=nb, tot=tot))
good = [r for r in rows if r['ret'] is not None and r['nv'] == r['tot']]
print(f'items {len(rows)}; sim offsets disagreeing with /FAs: {fas_bad}; items with unanimous retail votes {len(good)}: '
      f'same offset {sum(1 for r in good if r["ret"] == r["our"])}')
if outp:
    with open(outp, 'w') as f:
        f.write('idx\tsize\tw\tkind\tfl\tname\tour\tret\tvotes\tslot\n')
        for r in rows:
            f.write(f"{r['idx']}\t{r['size']}\t{r['w']}\t{r['kind']}\t{r['fl']}\t{r['name']}\t{r['our']}\t{r['ret']}\t{r['nv']}/{r['tot']}\t{r['slot']}\n")
