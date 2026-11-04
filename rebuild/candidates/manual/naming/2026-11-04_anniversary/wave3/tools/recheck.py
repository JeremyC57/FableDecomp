import sys, os, re, json, csv, collections
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import regscan as RS
from capstone.x86 import X86_OP_MEM, X86_OP_IMM
E = RS.E; R = '/home/user/FableDecomp'
pdb = set()
for f in ('ghidra_out/fablewin_pdb_names.tsv', 'ghidra_out/egor_pdb_names.tsv'):
    for l in open(R + '/' + f, errors='replace'):
        c = l.rstrip('\n').split('\t')
        if len(c) > 1: pdb.add(c[1])
CHK = [('012f55f8', 'NScript::CExpression_WaitScript', 'L'), ('012e22c8', 'NScript::CQS_ScytheInfoScript', 'L'), ('012dcb40', 'NScript::CQ_PrisonWardenGameScript', 'L'),
       ('012c3208', 'NScript::CPersonalScript_GlobalThingsScript::CArcheryCompetitionSignpost', 'E'), ('012c95f0', 'NScript::CQ_BanditCampBossBattleScript::CCROWDBANDITS', 'E'),
       ('012cdf28', 'NScript::CQ_GuildTrainingScript::CSpeedFriend', 'E'), ('012db364', 'NScript::CQ_OrchardFarmRaidScript::CCrateTeamMember', 'E'),
       ('012ddf3c', 'NScript::CQ_SecretPassageScript::CSP_UndeadTrigger', 'E'), ('012e6c1c', 'NScript::CV_BeggarAndChildScript::CBeggarBully', 'E'),
       ('012e8e80', 'NScript::CV_DemonDoorsScript::CDF_SummonsCreatures', 'E'), ('012f2c98', 'NScript::CV_SingingStonesScript::CManWithDoorName', 'E'),
       ('012f42b0', 'NScript::CV_TravellingHeroesScript::CTH_SnowTroll', 'E')]
review = list(csv.DictReader(open('review.tsv'), delimiter='\t'))
# RegisterScripts entries
REG = 0x00CD52D0; ins = RS.insns(REG, RS.fnext(REG)); reg = []; lastname = None
for i in ins:
    if i.mnemonic == 'push' and i.operands[0].type == X86_OP_IMM:
        s = RS.cstr(i.operands[0].imm)
        if s and not s.startswith('S_'): lastname = (s, i.operands[0].imm, i.address)
    if i.mnemonic == 'mov' and i.operands[0].type == X86_OP_MEM and i.operands[1].type == X86_OP_IMM and RS.text(i.operands[1].imm):
        reg.append((i.operands[1].imm, lastname, i.address))
out = []
for vt, applied, kind in CHK:
    v = int(vt, 16)
    if kind == 'L':
        hits = []
        for a_, nm, at in reg:
            w, ctor = RS.last_vt_store(a_, set())
            if w == v: hits.append((a_, nm, at, ctor))
        names = sorted({h[1][0] for h in hits if h[1]})
        outer = applied.split('::')[1]
        pdbok = any(s.startswith('?Alloc@%s@NScript@@SIPAVCScriptBase@@' % outer) for s in pdb)
        if len(names) == 1 and 'C%sScript' % names[0] == outer and pdbok:
            h = hits[0]
            ev = 'CScriptInfoManager::RegisterScripts 00cd52d0 stores factory %08x at %s next to name literal "%s" (%08x pushed at %08x); the factory news the object%s stores vtable %s; class C<name>Script = %s; PDB ?Alloc@%s@NScript@@SIPAVCScriptBase@@...' % (
                h[0], ','.join('%08x' % x[2] for x in hits), names[0], h[1][1], h[1][2], (' and calls ctor %08x, which' % h[3]) if h[3] else ' and', vt, outer, outer)
            out.append((vt, applied, 'confirmed', ev))
        elif len(names) == 1:
            out.append((vt, applied, 'wrong:NScript::C%sScript' % names[0] if any(s.startswith('?Alloc@C%sScript@NScript@@' % names[0]) for s in pdb) else 'unproven:registry name %s has no PDB Alloc' % names[0], 'factory hits %s' % hits))
        else:
            out.append((vt, applied, 'unproven:registry hits %s' % [(hex(h[0]), h[1]) for h in hits], ''))
    else:
        rs = [r for r in review if r['vt'] == vt]
        if not rs: out.append((vt, applied, 'unproven:no registration stores an Alloc that constructs this vtable', '')); continue
        full = 'NScript::%s::%s' % (rs[0]['outer'], rs[0]['inner'])
        r0 = rs[0]
        ev = 'registration %s in %s (%s): binding name "%s", ClassAllocFunc %08x; the Alloc news 0x%x bytes%s stores vtable %s (its only vtable store in .text); PDB ?Alloc@%s@%s@NScript@@SI...; outer from %s' % (
            ','.join(r['site'] for r in rs), r0['func'], 'Main/Init', '","'.join(r['string'] for r in rs), int(r0['alloc'], 16), int(r0['size']),
            (' and calls ctor %s, which' % r0['ctor']) if r0['ctor'] else ' and, with the ctor inline,', vt, r0['inner'], r0['outer'], r0['outer_src'])
        if r0['verdict'] != 'OK': ev += '; ' + r0['verdict'] + ': ' + r0['note']
        out.append((vt, applied, 'confirmed' if full == applied else 'wrong:' + full, ev))
with open('recheck_high.tsv', 'w') as f:
    f.write('vtable\tapplied_name\tverdict\tevidence\n')
    for x in out: f.write('\t'.join(s.replace('\t', ' ') for s in x) + '\n')
for x in out: print(x[0], x[1], x[2])
