# fixlab27.py: post-fixes LABELS.tsv from mklabels27.py: one row per address, plus explicit rows for targets this wave settled whose
#  label DB entry is a stand-in (mklabels skips a target whose right name is anywhere in labels_*.tsv).
import os
W = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
p = os.path.join(W, 'LABELS.tsv'); R = [l.rstrip('\n').split('\t') for l in open(p)]
out = [R[0]]; seen = set()
for r in R[1:]:
    if r[0] in seen: continue
    seen.add(r[0]); out.append(r)
RK = 'rechecked RELOCATION_MATCH + PASS'
ADD = [
 ('00994480', 'CFileDataOutputStream::Write',
  'naming wave 27 (fdosw, DECISIONS_W26 D2): own row defines Ego_r.pdb / FableWin.pdb ?Write@CFileDataOutputStream@@UAEXPBXJ@Z (vtable 0x0129A6F8 '
  'slot 4); was a free __fastcall Write; ' + RK),
 ('004128a0', 'std::vector<CWideString,std::allocator<CWideString> >::push_back',
  'naming wave 27 (pb, DECISIONS_W26 D7): own row defines ?push_back@?$vector@VCWideString@@V?$allocator@VCWideString@@@std@@@std@@QAEXABVCWideString@@@Z '
  '(the bound spelling; Ego_r / FableWin); was the free Vector_PushBack stand-in; ' + RK),
 ('007196b0', 'std::_List_base<CCountedPointer<CTCScriptedControl::CActionBase>,std::allocator<CCountedPointer<CTCScriptedControl::CActionBase> > >::~_List_base',
  'naming wave 27 (c695 queue): own row defines the compiler-emitted ??1?$_List_base@V?$CCountedPointer@VCActionBase@CTCScriptedControl@@@@V?$allocator@..@std@@@std@@QAE@XZ '
  '(clear 0x00719280 then free); supersedes CActiveFile::OnReadFinished / StdList_Dtor_007196b0; ' + RK),
 ('009e5c30', 'CSymbolMap::UpdateDirty',
  'naming wave 27 (dm / sym, c695 queue): ?UpdateDirty@CSymbolMap@@QBEXXZ (Ego_r.pdb + FableWin.pdb); four binders respelled from ?Finalise@CSymbolMap / '
  '?Finalise@CSymbolTable; ' + RK),
]
for t, n, e in ADD:
    if t not in seen: seen.add(t); out.append((t, n, e))
open(p, 'w').write(''.join('\t'.join(r) + '\n' for r in out)); print(len(out) - 1, 'labels after fix')
