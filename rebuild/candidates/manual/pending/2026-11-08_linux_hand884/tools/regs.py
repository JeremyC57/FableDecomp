#!/usr/bin/env python3
"""regs.py: segment retail CTCBase::RegisterTCs (0x004EE23F) into its registration statements -> probe/regs.tsv
Columns: idx, kind (TC / DEF / CALL), class name (from the literal or the out-of-line StatGetClassTypeName callee), alloc VA,
driver (call = CTCBase::StatGetDriverType, else the immediate), name form (inl = expanded literal, call = callee VA),
pair dtor / CDefClassInit form (inl / call), frame offsets of the temporaries (name, info, pair) or (name, init)."""
import os, re, struct, sys
from capstone import Cs, CS_ARCH_X86, CS_MODE_32
S = os.environ.get('S', '/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad')
A = f'{S}/w22/agent884'
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
    b = rd(va, 200); return b[:b.find(b'\0')].decode()
md = Cs(CS_ARCH_X86, CS_MODE_32)
START, END = 0x004EE23F, 0x004F9145
ins = list(md.disasm(rd(START, END-START), START))
def stat_name(callee):
    # push esi ; push -1 ; push imm32 ; mov esi,ecx ; call CCharString(const char*, long)
    b = rd(callee, 12)
    assert b[:4] == bytes.fromhex('566aff68'), hex(callee)
    return cstr(struct.unpack_from('<I', b, 4)[0])
def fo(op):
    m = re.search(r'\[ebp ([+-]) 0x([0-9a-f]+)\]', op)
    return (1 if m.group(1) == '+' else -1) * int(m.group(2), 16) if m else None
rows = []; i = 0
# skip the prologue and the clear() call
while not (ins[i].mnemonic == 'call' and ins[i].op_str == '0x4e1b5d'): i += 1
i += 1
cur = None
while i < len(ins):
    x = ins[i]; m, op = x.mnemonic, x.op_str
    if m == 'call':
        t = int(op, 16) if op.startswith('0x') else None
        if t in (0x749d60, 0x7330f0, 0x7899c0, 0x7fffa0, 0x6cba00, 0x75c0e0, 0x8162f0, 0x802370, 0x73b130):
            rows.append(dict(kind='CALL', name=hex(t), va=x.address)); i += 1; continue
        if t == 0x99ebf0:   # CCharString(const char*, -1): a TC literal name or a def name
            lit = cstr(int(ins[i-2].op_str, 16)); off = fo(ins[i-1].op_str)
            cur = dict(name=lit, nameform='inl', name_off=off, va=ins[i-3].address)
        elif t is not None and 0x4d0000 <= t < 0x4f0000 and t not in (0x4d2ef0, 0x4d9d2f, 0x4d6902, 0x4e40c3, 0x4e1b5d, 0x4ebace):
            # out-of-line StatGetClassTypeName (hidden result in ecx)
            cur = dict(name=stat_name(t), nameform=hex(t), name_off=fo(ins[i-1].op_str), va=ins[i-1].address)
        elif t == 0x4d2ef0:  # CRegisteredTCInfo(alloc, driver, name)
            cur['kind'] = 'TC'
            cur['alloc'] = ins[i-2].op_str
            p = ins[i-3]
            cur['driver'] = 'call' if (ins[i-4].mnemonic == 'call' and ins[i-4].op_str == '0x6869c0') else p.op_str
            cur['info_off'] = fo(ins[i-1].op_str)
        elif t == 0x4d9d2f:
            cur['pair_off'] = fo(ins[i-1].op_str)
        elif t == 0x4d6902:
            cur['pairdtor'] = 'call'
        elif t == 0x42dae0:  # CDefClassInit(name, alloc) out of line
            cur['kind'] = 'DEF'; cur['alloc'] = ins[i-4].op_str; cur['init'] = 'call'; cur['init_off'] = fo(ins[i-1].op_str)
        elif t == 0x99ec30:  # inline CDefClassInit: Name copy
            cur['kind'] = 'DEF'; cur['init'] = 'inl'; cur['init_off'] = fo(ins[i-1].op_str)
            # the AllocFunc store follows: mov dword ptr [ebp - x], imm
            j = i + 1
            while not (ins[j].mnemonic == 'mov' and ins[j].op_str.startswith('dword ptr [ebp')): j += 1
            cur['alloc'] = ins[j].op_str.split(', ')[1]
        elif t == 0x9b0ac0:
            rows.append(cur); cur = None
        elif t == 0x4e40c3:
            pass
        elif t == 0x99eae0:
            if cur is not None and cur.get('kind') == 'TC' and 'pairdtor' not in cur and fo(ins[i-1].op_str) == cur['pair_off'] + 0xC:
                cur['pairdtor'] = 'inl'
            # the TC block ends with the name's destructor
            if cur is not None and cur.get('kind') == 'TC' and fo(ins[i-1].op_str) == cur['name_off']:
                rows.append(cur); cur = None
        elif t == 0x4ebace:
            pass
    i += 1
with open(f'{A}/probe/regs.tsv', 'w') as f:
    f.write('idx\tkind\tname\talloc\tdriver\tnameform\tpairdtor_or_init\toffs\tva\n')
    for k, r in enumerate(rows):
        if r['kind'] == 'CALL':
            f.write(f"{k}\tCALL\t{r['name']}\t\t\t\t\t\t{r['va']:08x}\n")
        elif r['kind'] == 'TC':
            f.write(f"{k}\tTC\t{r['name']}\t{r['alloc']}\t{r['driver']}\t{r['nameform']}\t{r.get('pairdtor')}\t{r['name_off']},{r['info_off']},{r['pair_off']}\t{r['va']:08x}\n")
        else:
            f.write(f"{k}\tDEF\t{r['name']}\t{r['alloc']}\t\t\t{r['init']}\t{r['name_off']},{r['init_off']}\t{r['va']:08x}\n")
import collections
print(len(rows), collections.Counter(r['kind'] for r in rows))
print('TC name inl', sum(1 for r in rows if r['kind'] == 'TC' and r['nameform'] == 'inl'),
      'pairdtor inl', sum(1 for r in rows if r['kind'] == 'TC' and r.get('pairdtor') == 'inl'),
      'def init inl', sum(1 for r in rows if r['kind'] == 'DEF' and r['init'] == 'inl'),
      'drivers', sum(1 for r in rows if r['kind'] == 'TC' and r['driver'] != 'call'))
