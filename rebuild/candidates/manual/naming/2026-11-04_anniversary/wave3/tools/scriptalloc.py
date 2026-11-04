#!/usr/bin/env python3
"""Wave 3d: level-script static Alloc + ctor for the outer scripts proven by the entity registrations.
Sources: (1) the script vtable (RegisterMain-anchored, outer proven in name.py) is stored after operator new by exactly
one Alloc-shaped function (and/or its out-of-line ctor); (2) CScriptInfoManager::RegisterScripts 0x00CD52D0 stores that
Alloc next to the script-name literal "<name>" with class C<name>Script; (3) PDB ?Alloc@<outer>@NScript@@SIPAVCScriptBase@@."""
import sys, os, re, json, struct, collections, csv
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import regscan as RS
from capstone.x86 import X86_OP_MEM, X86_OP_IMM
E = RS.E; md = RS.md
W = os.path.dirname(os.path.abspath(__file__)) + '/..'; R = '/home/user/FableDecomp'
exec(open(os.path.dirname(os.path.abspath(__file__)) + '/common.py').read())
SVT = json.load(open(W + '/script_vts.json'))
# RegisterScripts table: alloc -> name
REG = 0x00CD52D0; hi = RS.fnext(REG)
ins = RS.insns(REG, hi)
reg_name = {}; lastname = None
for i in ins:
    if i.mnemonic == 'push' and i.operands[0].type == X86_OP_IMM:
        s = RS.cstr(i.operands[0].imm)
        if s and not s.startswith('S_'): lastname = (s, i.address)
    if i.mnemonic == 'mov' and i.operands[0].type == X86_OP_MEM and i.operands[1].type == X86_OP_IMM and RS.text(i.operands[1].imm):
        reg_name.setdefault(i.operands[1].imm, []).append((lastname, i.address))
rows = []; log = []
d = E.d
for outer, (svt, eslot, func) in sorted(SVT.items()):
    vt = int(svt, 16)
    q = struct.pack('<I', vt); t0 = E.off(E.tlo); j = t0; storers = set()
    while True:
        j = d.find(q, j, t0 + len(E.text))
        if j < 0: break
        storers.add(RS.fstart(E.tlo + j - t0)); j += 1
    allocs = []
    for s_ in storers:
        v, ctor = RS.last_vt_store(s_, set())
        if v == vt and struct.pack('<I', RS.NEW) not in b'' and any(i.mnemonic == 'call' and i.op_str == hex(RS.NEW) for i in RS.insns(s_, min(RS.fnext(s_), s_ + 0x40))):
            allocs.append((s_, ctor))
    # also allocs whose ctor is the storer
    for a_, n_ in reg_name.items():
        v, ctor = RS.last_vt_store(a_, set())
        if v == vt and (a_, ctor) not in allocs: allocs.append((a_, ctor))
    allocs = sorted(set(allocs))
    if len(allocs) != 1:
        log.append((outer, 'SKIP', 'allocs=%s storers=%s' % (allocs, sorted('%08x' % x for x in storers)))); continue
    a_, ctor = allocs[0]
    rn = reg_name.get(a_, [])
    names = sorted({x[0][0] for x in rn if x[0]})
    regok = names == [outer[1:-6]] if outer.startswith('C') and outer.endswith('Script') else False
    pdbok = any(s.startswith('?Alloc@%s@NScript@@SIPAVCScriptBase@@' % outer) for s in pdb)
    ctorok = any(s.startswith('??0%s@NScript@@QAE@PAVCScriptDataBase@@' % outer) for s in pdb)
    ev = 'level-script Alloc: operator new + stores script vtable %s (outer %s proven by its Main %s entity registrations)' % (svt, outer, func)
    if rn: ev += '; CScriptInfoManager::RegisterScripts %s stores it at %s with name "%s"' % ('%08x' % REG, ','.join('%08x' % x[1] for x in rn), '/'.join(names))
    if pdbok and regok:
        if refs(a_) == len(rn):
            propose(rows, log, '%08x' % a_, 'NScript::%s::Alloc' % outer, ev + '; PDB ?Alloc@%s@NScript@@SIPAVCScriptBase@@...; refs=%d (RegisterScripts only)' % (outer, refs(a_)))
        else: log.append(('%08x' % a_, 'SKIP', 'refs=%d vs reg sites %d' % (refs(a_), len(rn))))
        if not ctor and ctorok:
            # out-of-line copy of the (inlined) ctor: another storer of the script vtable, `this` in ecx, base ctor CScriptBase 0x00CB8110 call, ret 8
            cands = []
            for s_ in storers:
                if s_ == a_: continue
                b_ = E.read(s_, min(RS.fnext(s_), s_ + 0x100) - s_)
                v, _ = RS.last_vt_store(s_, {'ecx'})
                calls_base = any(i.mnemonic == 'call' and i.op_str == '0xcb8110' for i in RS.insns(s_, s_ + 12))
                if v == vt and calls_base and b'\xc2\x08\x00' in b_: cands.append(s_)
            if len(cands) == 1:
                ctor = cands[0]; ev_c = 'out-of-line copy of the ctor the Alloc %08x inlines: base CScriptBase ctor 0x00CB8110 on ecx, stores script vtable %s, ret 8 (two args)' % (a_, svt)
            else: ev_c = None
        else: ev_c = 'level-script ctor: called by the Alloc %08x on the new block, stores script vtable %s' % (a_, svt) if ctor else None
        if ctor and ctorok and ev_c:
            propose(rows, log, '%08x' % ctor, 'NScript::%s::%s' % (outer, outer), ev_c + '; PDB ??0%s@NScript@@QAE@PAVCScriptDataBase@@...; outer %s proven by its Main %s entity registrations' % (outer, outer, func))
    else:
        log.append(('%08x' % a_, 'SKIP', 'pdb=%s regname=%s (%s)' % (pdbok, regok, names)))
with open(W + '/labels_scriptalloc.tsv', 'w') as f:
    f.write('address\tname\tevidence\n')
    for a, n, ev in sorted(rows): f.write('%s\t%s\t%s\n' % (a, n, ev.replace('\t', ' ')))
with open(W + '/scriptalloc_log.tsv', 'w') as f:
    for x in log: f.write('\t'.join(map(str, x)) + '\n')
print('script alloc/ctor rows', len(rows), collections.Counter(x[1] for x in log if len(x) > 1))
