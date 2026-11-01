# fixlab26.py: post-fixes LABELS.tsv from mklabels26.py.
#  - 0139c8fc: DECISIONS_W25_RESOLVED D1 settled _NullString (wave 25 held the label), so the generated row stands.
#  - labels the label DB carries in a superseded spelling next to the right one (mklabels skips a target whose right name is anywhere
#    in labels_*.tsv): an explicit wave-26 row so the last-applied label is the settled name.
#  - targets this wave settled whose address carries no label, or only a stand-in one.
import os
W = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
p = os.path.join(W, 'LABELS.tsv'); R = [l.rstrip('\n').split('\t') for l in open(p)]
out = [R[0]]; seen = set()
for r in R[1:]:
    if r[0] in seen: continue
    seen.add(r[0]); out.append(r)
RK = 'rechecked RELOCATION_MATCH + PASS'
ADD = [
 ('00e241b0', 'NScript::CIsThingAlive::~CIsThingAlive',
  'naming wave 26 (c693): own row defines Ego_r.pdb / FableWin.pdb ??1CIsThingAlive@NScript@@UAE@XZ; supersedes the label DB\'s '
  'CScriptThingFunctor_00e241b0::~CScriptThingFunctor_00e241b0; ' + RK),
 ('00f35b10', 'CEntityScriptBase::SetPredicate',
  'naming wave 26 (pred): Ego_r.pdb / FableWin.pdb ?SetPredicate@CEntityScriptBase@@QAEXABVCPredicateBase@NScript@@@Z; supersedes the '
  'label DB\'s RegisterFilter spellings; eleven binders respelled over the empty global CEntityScriptBase; ' + RK),
 ('00440990', 'std::vector<long,std::allocator<long> >::_M_insert_overflow',
  'naming wave 26 (vlong / gst): ?_M_insert_overflow@?$vector@JV?$allocator@J@std@@@std@@IAEXPAJABJABU__true_type@2@I_N@Z (STLport 4.6, '
  'the own row\'s spelling); supersedes the label DB\'s InsertMultipleAndGrow / FUN_ spellings; 18 callers respelled; ' + RK),
 ('00412330', 'std::vector<CWideString,std::allocator<CWideString> >::_M_insert_overflow',
  'naming wave 26 (vws): ?_M_insert_overflow@?$vector@VCWideString@@V?$allocator@VCWideString@@@std@@@std@@IAEXPAVCWideString@@ABV3@ABU__false_type@2@I_N@Z; '
  'supersedes the label DB\'s Std_Vector_Insert_Range_Copies / FUN_ spellings; four callers respelled; ' + RK),
 ('007ad180', 'std::_Rb_tree<CCharString,std::pair<const CCharString,bool>,std::_Select1st<std::pair<const CCharString,bool> >,std::less<CCharString>,std::allocator<std::pair<const CCharString,bool> > >::erase',
  'naming wave 26 (rbt): own row defines ?erase@?$_Rb_tree@VCCharString@@U?$pair@$$CBVCCharString@@_N@std@@... (erase(iterator, iterator), '
  'the spelling map::erase 0x007ADE60 binds); supersedes the label DB\'s LTextBinTree_EraseRange; ' + RK),
 ('009f31c0', 'CSurface::SetPixel',
  'naming wave 26 (wcol, DECISIONS_W25 D7): own row defines Ego_r.pdb / FableWin.pdb ?SetPixel@CSurface@@QAEXJJABVCRGBColour@@ABVCSurfaceLockedRect@@@Z '
  '(colour by reference); ' + RK),
 ('00d9f270', 'NScript::CQ_MinionCampScript::CMC_Minion::Main',
  'naming wave 26 (pred): the row\'s catalog module (Ego_r.pdb nested CMC_Minion of CQ_MinionCampScript); no label in the DB; ' + RK),
]
for t, n, e in ADD:
    if t not in seen: seen.add(t); out.append((t, n, e))
open(p, 'w').write(''.join('\t'.join(r) + '\n' for r in out)); print(len(out) - 1, 'labels after fix')
