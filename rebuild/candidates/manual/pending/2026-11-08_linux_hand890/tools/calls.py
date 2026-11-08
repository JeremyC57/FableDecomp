#!/usr/bin/env python3
"""calls.py <addr> <obj> [--leaf L]: the call sequence of retail (target address, or `[reg+slot]` for indirect calls) next
to ours (relocation symbol, demangled-ish, or `[reg+slot]`), aligned with difflib on a normalised key (retail address mapped
through rebuild/oracles/reloc_bindings.tsv symbols learned from equal blocks). Prints the differing hunks with positions."""
import sys, os, re, csv, subprocess, difflib, collections
csv.field_size_limit(10**8)
sys.path.insert(0, "/home/user/FableDecomp/tools/decomp_pipeline")
import verify_and_land as V
from capstone import Cs, CS_ARCH_X86, CS_MODE_32
S = os.environ.get('S', '/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad')
args = sys.argv[1:]
def opt(n, d=None): return args[args.index(n) + 1] if n in args else d
addr = args[0].lower(); obj = args[1]; RET = int(addr, 16)
rows = {r['address'].lower(): r for r in csv.DictReader(open(f'{S}/w22/chunk890.tsv', encoding='utf-8-sig'), delimiter='\t')}
LEAF = {'00551ea0': '?ConstructSpritesToDraw@CTable@NUISystem@@', '0056ad40': '?UpdateUI_QuestMenu@CQuestManager@@',
        '00498490': '?DrawRetailDisplay@CProgressDisplay@@'}
leaf = opt('--leaf', LEAF.get(addr))
retail = bytes.fromhex(rows[addr]['bytes'])
built, sec, sym = V.obj_text(obj, leaf)
blocks = []; cur = None
for line in subprocess.run(['objdump', '-r', obj], capture_output=True, text=True).stdout.splitlines():
    m = re.match(r'RELOCATION RECORDS FOR \[(.*)\]:', line)
    if m: cur = {'name': m.group(1), 'r': {}}; blocks.append(cur); continue
    m = re.match(r'^([0-9a-f]{8}) (\S+)\s+(.*)$', line)
    if m and cur is not None: cur['r'][int(m.group(1), 16)] = m.group(3).strip()
best = max((b for b in blocks if b['name'].startswith('.text') and b['r'] and max(b['r']) < len(built) + 4), key=lambda b: len(b['r']))
rel = best['r']
md = Cs(CS_ARCH_X86, CS_MODE_32)
def seq(code, base, ours):
    out = []
    for i in md.disasm(code, base):
        if i.mnemonic != 'call': continue
        if i.op_str.startswith('0x'):
            t = rel.get(i.address - base + 1, '?') if ours else int(i.op_str, 16)
        else:
            t = re.sub(r'dword ptr ', '', i.op_str)
            t = re.sub(r'\b(eax|ecx|edx|esi|edi|ebx|ebp)\b', 'r', t)
        out.append((i.address, t))
    return out
R = seq(retail, RET, False); O = seq(built, 0, True)
# learn symbol -> address from position-equal pairs
bind = {}
for r in csv.DictReader(open('/home/user/FableDecomp/rebuild/oracles/reloc_bindings.tsv', encoding='utf-8'), delimiter='\t'):
    bind.setdefault(r['symbol'], int(r['target'], 16))
def rk(t): return t if isinstance(t, str) else '%x' % t
def ok(t): return t if t.startswith('[') else ('%x' % bind[t] if t in bind else t)
learned = {}
sm0 = difflib.SequenceMatcher(None, [rk(t) if isinstance(t, str) else 'C' for _, t in R], [t if t.startswith('[') else 'C' for _, t in O], autojunk=False)
for tag, i1, i2, j1, j2 in sm0.get_opcodes():
    if tag == 'equal':
        for a, b in zip(range(i1, i2), range(j1, j2)):
            if not isinstance(R[a][1], str) and not O[b][1].startswith('['):
                learned.setdefault(O[b][1], collections.Counter())[R[a][1]] += 1
def ok2(t):
    if t.startswith('['): return t
    if t in bind: return '%x' % bind[t]
    if t in learned: return '%x' % learned[t].most_common(1)[0][0]
    return t
rkeys = [rk(t) for _, t in R]; okeys = [ok2(t) for _, t in O]
sm = difflib.SequenceMatcher(None, rkeys, okeys, autojunk=False)
m = sum(b.size for b in sm.get_matching_blocks())
print(f'calls: retail {len(R)}, ours {len(O)}, matched {m}')
if '--counts' in args:
    rc = collections.Counter(rkeys); oc = collections.Counter(okeys)
    for k in sorted(set(rc) | set(oc), key=lambda k: -(rc[k] + oc[k])):
        if rc[k] != oc[k]: print(f'  {k:30s} retail {rc[k]:3d}  ours {oc[k]:3d}')
for tag, i1, i2, j1, j2 in sm.get_opcodes():
    if tag == 'equal': continue
    print(f'@@ {tag} retail[{i1}:{i2}] ours[{j1}:{j2}]')
    for k in range(i1, i2): print(f'  - {R[k][0]:08x} {rkeys[k]}')
    for k in range(j1, j2): print(f'  + +{O[k][0]:04x} {okeys[k]}  ({O[k][1][:70]})')
