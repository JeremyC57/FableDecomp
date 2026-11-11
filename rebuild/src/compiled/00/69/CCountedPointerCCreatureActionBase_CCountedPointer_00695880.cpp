// CCountedPointer<CCreatureActionBase>::CCountedPointer(T*) -- take ownership of a new pointee: a fresh count block (count 1, DeleteData, Data) when non-null
// Leaf: ??0?$CCountedPointer@VCCreatureActionBase@@@@QAE@PAVCCreatureActionBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00695880, 78 bytes. Callees: the count block's DeleteFunc (indirect), operator delete 0x00BFE9BC.
// Was landed as _unattributed::CCountedPointer_00695880 (stand-in name).
// T = CCreatureActionBase, proven by: 0x00692E50 binds ??0?$CCountedPointer@VCCreatureActionBase@@@@QAE@PAVCCreatureActionBase@@@Z; callers wrap new creature actions (CCreatureActionBase::GetPActionFromEnum, CActionFireProjectileWeapon::OnAdd, CAIStateGroup_WhisperRetreat::UseWill) whose locals the same functions release with ~CCountedPointer<CCreatureActionBase> 0x0048C5C0; FableWin.pdb (T*) ctor name.
// Member: the in-class (T*) constructor of engine/CCountedPointer.h, emitted out of line as its own COMDAT (`ret 4`); the count
//   block stores CCountedPointer<T>::DeleteData (its own COMDAT; T is only forward-declared here).
#include "engine/CCountedPointer.h"
class CCreatureActionBase;
template CCountedPointer<CCreatureActionBase>::CCountedPointer(CCreatureActionBase*);
