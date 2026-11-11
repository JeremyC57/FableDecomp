// CCountedPointer<CEntityScriptBase>::CCountedPointer(T*) -- take ownership of a new pointee: a fresh count block (count 1, DeleteData, Data) when non-null
// Leaf: ??0?$CCountedPointer@VCEntityScriptBase@@@@QAE@PAVCEntityScriptBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CDEDB0, 78 bytes. Callees: the count block's DeleteFunc (indirect), operator delete 0x00BFE9BC.
// Was landed as _unattributed::CCountedPointer_00cdedb0 (stand-in name).
// T = CEntityScriptBase, proven by: 0x00CDEB40 binds ??0?$CCountedPointer@VCEntityScriptBase@@@@QAE@PAVCEntityScriptBase@@@Z; the NScript ...::Alloc functions (CArcheryCompetitionSignpost::Alloc) wrap their new script in it (CActiveEntityScriptBase::PEntityScript is CCountedPointer<CEntityScriptBase>, Ego_r); FableWin.pdb (T*) ctor name.
// Member: the in-class (T*) constructor of engine/CCountedPointer.h, emitted out of line as its own COMDAT (`ret 4`); the count
//   block stores CCountedPointer<T>::DeleteData (its own COMDAT; T is only forward-declared here).
#include "engine/CCountedPointer.h"
class CEntityScriptBase;
template CCountedPointer<CEntityScriptBase>::CCountedPointer(CEntityScriptBase*);
