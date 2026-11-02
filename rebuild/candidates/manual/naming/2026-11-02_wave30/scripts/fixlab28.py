# fixlab28.py: post-fixes LABELS.tsv from mklabels28.py: one row per address, plus explicit rows for targets this wave settled whose
#  label DB entry is a stand-in (mklabels skips a target whose right name is anywhere in labels_*.tsv).
import os
W = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
p = os.path.join(W, 'LABELS.tsv'); R = [l.rstrip('\n').split('\t') for l in open(p)]
out = [R[0]]; seen = set()
for r in R[1:]:
    if r[0] in seen or r[0] == '00bdfa10': continue   # own Leaf is a member stand-in (see fixren28 DROP)
    seen.add(r[0]); out.append(r)
RK = 'rechecked RELOCATION_MATCH + PASS'
ADD = [
 ('006a5230', 'CThingPhysical::InitialActivate',
  'naming wave 28 (d6, DECISIONS_W27 D6): own row defines Ego_r.pdb / FableWin.pdb ?InitialActivate@CThingPhysical@@MAEXXZ (slot 9 of vtable 0x01244E84, '
  'RTTI .?AVCThingPhysical@@); supersedes C3DAnimationBlendState::AddCombinationBlend; ' + RK),
 ('007345d0', 'CTCCombat::GetAppropriateCombatAnimationFromList',
  'naming wave 28 (d4, DECISIONS_W27 D4): own row defines Ego_r.pdb / FableWin.pdb ?GetAppropriateCombatAnimationFromList@CTCCombat@@QBEJABV?$vector@PBVCAnimationEntry@@..@Z; '
  'supersedes GetAppropriateCombatAnimation_FilterAndSelect / CTCCombat_Methods; ' + RK),
 ('00666b50', 'CThingCreatureBase::GetAppropriateCombatAnimationFromList',
  'naming wave 28 (d4, DECISIONS_W27 D4): own row defines the Ego_r.pdb / FableWin.pdb const member over vector<const CAnimationEntry*>; three callers respelled; ' + RK),
 ('004125a0', 'std::vector<CDateAndTime,std::allocator<CDateAndTime> >::_M_insert_overflow',
  'naming wave 28 (d3, DECISIONS_W27 D3): own row defines the protected STLport __false_type member (proven from the bytes: null-checked placement '
  'copies, no memmove); supersedes Std_Vector_Insert_Range_Copies_28byte; ' + RK),
 ('00ed7db0', 'NScript::CV_TalentlessBardScript::CTB_Bard::`scalar deleting destructor\'',
  'naming wave 28 (bard, c696 queue): own row defines Ego_r.pdb ??_GCTB_Bard@CV_TalentlessBardScript@NScript@@UAEPAXI@Z (slot 0 of vtable 0x012F3494); '
  'supersedes CLandscapeBackgroundPatch::`vector deleting destructor\' / CVtable_012f3494; ' + RK),
 ('00b81e20', 'CEnginePrimitiveSimulationInfo::Clear',
  'naming wave 28 (c697 queue): own row defines Ego_r.pdb / FableWin.pdb ?Clear@CEnginePrimitiveSimulationInfo@@QAEXXZ; supersedes ClearSimulations; ' + RK),
 ('0055a3e0', 'NUISystem::CDraggable::Update',
  'naming wave 28 (clk): own row defines the Ego_r.pdb / FableWin.pdb virtual ?Update@CDraggable@NUISystem@@UAEXM@Z; ' + RK),
 ('009d4a20', 'CDefStringTable::GetStringCRC',
  'naming wave 28 (peek): own row defines the Ego_r.pdb / FableWin.pdb const ?GetStringCRC@CDefStringTable@@QBEKJ@Z; ' + RK),
]
for t, n, e in ADD:
    if t not in seen: seen.add(t); out.append((t, n, e))
open(p, 'w').write(''.join('\t'.join(r) + '\n' for r in out)); print(len(out) - 1, 'labels after fix')
