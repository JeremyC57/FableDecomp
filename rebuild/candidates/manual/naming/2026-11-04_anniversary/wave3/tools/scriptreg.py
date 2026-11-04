#!/usr/bin/env python3
"""Wave 3e: level scripts WITHOUT entity registrations, named from CScriptInfoManager::RegisterScripts (0x00CD52D0):
each entry stores a script Alloc next to the name literal "<N>"; the Alloc (operator new + ctor) stores the script vtable;
class = NScript::C<N>Script accepted only when the PDB has ?Alloc@C<N>Script@NScript@@SIPAVCScriptBase@@.
Then the 5 script slots (0 sdd, 1 RegisterMain, 2 Main, 3 Init, 4 OnPersist; slot 1 must push the "Main" literal),
the Alloc, the ctor and CSpawnedFunc<T> (the vtable RegisterMain stores) slots 0/4. refs==1 + PDB member, as in name.py."""
import sys, os, re, json, struct, collections, csv
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import regscan as RS
from capstone.x86 import X86_OP_MEM, X86_OP_IMM
E = RS.E
W = os.path.dirname(os.path.abspath(__file__)) + '/..'; R = '/home/user/FableDecomp'
exec(open(os.path.dirname(os.path.abspath(__file__)) + '/common.py').read())
SVT = json.load(open(W + '/script_vts.json'))
done_outers = set(SVT)
REG = 0x00CD52D0; ins = RS.insns(REG, RS.fnext(REG))
reg = []; lastname = None
for i in ins:
    if i.mnemonic == 'push' and i.operands[0].type == X86_OP_IMM:
        s = RS.cstr(i.operands[0].imm)
        if s and not s.startswith('S_'): lastname = (s, i.address)
    if i.mnemonic == 'mov' and i.operands[0].type == X86_OP_MEM and i.operands[1].type == X86_OP_IMM and RS.text(i.operands[1].imm):
        reg.append((i.operands[1].imm, lastname, i.address))
by_alloc = collections.defaultdict(list)
for a_, nm, at in reg: by_alloc[a_].append((nm, at))
def tags_outer(o):
    t = set()
    for s in pdb:
        m = re.match(r'^\?(\w+)@%s@NScript@@[^@]' % re.escape(o), s)
        if m: t.add(m.group(1))
        m = re.match(r'^\?\?(_G|_E|1|0)%s@NScript@@[^@]' % re.escape(o), s)
        if m: t.add('??' + m.group(1))
    return t
def tmpl_tags(tmpl, outer):
    pre = '?$%s@V%s@NScript@@@@' % (tmpl, outer); t = set()
    for s in pdb:
        if pre in s:
            m = re.match(r'^\?(\w+)@\?\$', s)
            if m: t.add(m.group(1))
            m = re.match(r'^\?\?(_G|_E|1|0)\?\$', s)
            if m: t.add('??' + m.group(1))
    return t
rows = []; log = []; vts_out = {}
for a_, lst in sorted(by_alloc.items()):
    names = sorted({x[0][0] for x in lst if x[0]})
    if len(names) != 1: log.append(('%08x' % a_, 'SKIP', 'names %s' % names)); continue
    N = names[0]; outer = 'C%sScript' % N
    if outer in done_outers: continue
    vt, ctor = RS.last_vt_store(a_, set())
    if not vt: log.append(('%08x' % a_, 'SKIP', 'no vtable store')); continue
    rm = E.dword(vt + 4)
    if not (rm and RS.text(rm) and struct.pack('<I', 0x012C2F94) in E.read(rm, 0x90)):
        log.append(('%08x' % a_, 'SKIP', '%s: slot 1 of %08x is not a RegisterMain' % (outer, vt))); continue
    tg = tags_outer(outer)
    if 'Alloc' not in tg: log.append(('%08x' % a_, 'SKIP', 'PDB has no ?Alloc@%s@NScript@@' % outer)); continue
    vts_out[outer] = '%08x' % vt
    sites = ','.join('%08x' % x[1] for x in lst)
    base = 'CScriptInfoManager::RegisterScripts 00cd52d0 stores Alloc %08x at %s with name "%s"; Alloc news + stores script vtable %08x; PDB ?Alloc@%s@NScript@@SIPAVCScriptBase@@' % (a_, sites, N, vt, outer)
    if refs(a_) == len(lst): propose(rows, log, '%08x' % a_, 'NScript::%s::Alloc' % outer, base + '; refs=%d' % refs(a_))
    else: log.append(('%08x' % a_, 'SKIP', 'alloc refs %d' % refs(a_)))
    if ctor and '??0' in tg:
        propose(rows, log, '%08x' % ctor, 'NScript::%s::%s' % (outer, outer), base + '; ctor called by the Alloc on the new block; PDB ??0%s@NScript@@' % outer)
    elif '??0' in tg:
        q = struct.pack('<I', vt); t0 = E.off(E.tlo); j = t0; storers = set()
        while True:
            j = E.d.find(q, j, t0 + len(E.text))
            if j < 0: break
            storers.add(RS.fstart(E.tlo + j - t0)); j += 1
        cands = []
        sym0 = [x for x in pdb if x.startswith('??0%s@NScript@@QAE@' % outer)]
        want = b'\xc2\x08\x00' if any('PAVCScriptDataBase@@AAVCGameScriptInterfaceBase' in x for x in sym0) else b'\xc2\x04\x00'
        for s_ in storers:
            if s_ == a_: continue
            b_ = E.read(s_, min(RS.fnext(s_), s_ + 0x100) - s_)
            v, _ = RS.last_vt_store(s_, {'ecx'})
            calls_base = any(i.mnemonic == 'call' and i.op_str == '0xcb8110' for i in RS.insns(s_, s_ + 12))
            if v == vt and calls_base and want in b_ and len(sym0) == 1: cands.append(s_)
        if len(cands) == 1:
            propose(rows, log, '%08x' % cands[0], 'NScript::%s::%s' % (outer, outer), base + '; out-of-line copy of the ctor the Alloc inlines: base CScriptBase ctor 0x00CB8110 on ecx, stores the script vtable, ret %d = PDB arity; PDB %s' % (want[1], sym0[0]))
    for slot, meth in ((0, 'sdd'), (1, 'RegisterMain'), (2, 'Main'), (3, 'Init'), (4, 'OnPersist')):
        f = E.dword(vt + 4 * slot); n = refs(f)
        has = ('??_G' in tg or '??1' in tg) if meth == 'sdd' else meth in tg
        nm = 'NScript::%s::%s' % (outer, "`scalar deleting destructor'" if meth == 'sdd' else meth)
        if n == 1 and has: propose(rows, log, '%08x' % f, nm, base + '; script vtable slot %d (slot 1 pushes "Main" 0x012C2F94); PDB %s; refs=1' % (slot, meth))
        else: log.append(('%08x' % f, 'SKIP', '%s refs=%d pdb=%s' % (nm, n, has)))
    fvt, _ = RS.last_vt_store(rm, set())
    st = tmpl_tags('CSpawnedFunc', outer)
    if fvt:
        for slot, meth in ((0, 'sdd'), (4, 'SuspendableProcess')):
            f = E.dword(fvt + 4 * slot); n = refs(f)
            has = ('??_G' in st or '??_E' in st or '??1' in st) if meth == 'sdd' else meth in st
            nm = 'CSpawnedFunc<NScript::%s>::%s' % (outer, "`scalar deleting destructor'" if meth == 'sdd' else meth)
            if n == 1 and has: propose(rows, log, '%08x' % f, nm, base + '; CSpawnedFunc<%s> vtable %08x stored after operator new(0x3c) by RegisterMain %08x; slot %d; PDB %s; refs=1' % (outer, fvt, rm, slot, meth))
            else: log.append(('%08x' % f, 'SKIP', '%s refs=%d pdb=%s' % (nm, n, has)))
seen = set()
with open(W + '/labels_scriptreg.tsv', 'w') as f:
    f.write('address\tname\tevidence\n')
    for a, n, ev in sorted(rows):
        if a in seen: continue
        seen.add(a); f.write('%s\t%s\t%s\n' % (a, n, ev.replace('\t', ' ')))
json.dump(vts_out, open(W + '/scriptreg_vts.json', 'w'), indent=0)
with open(W + '/scriptreg_log.tsv', 'w') as f:
    for x in log: f.write('\t'.join(map(str, x)) + '\n')
print('scripts', len(vts_out), 'rows', len(seen), collections.Counter(x[1] for x in log))
