// CCountedPointer<CActiveEntityScriptBase>::CCountedPointer(T*) -- take ownership of a new pointee: a fresh count block (count 1, DeleteData, Data) when non-null
// Leaf: ??0?$CCountedPointer@VCActiveEntityScriptBase@@@@QAE@PAVCActiveEntityScriptBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CE10B0, 78 bytes. Callees: the count block's DeleteFunc (indirect), operator delete 0x00BFE9BC.
// Was landed as _unattributed::CCountedPointer_00ce10b0 (stand-in name).
// T = CActiveEntityScriptBase, proven by: all 240 callers are CEntityScriptBinding<X>::CreateActiveEntityScript (one per script), which wrap their new CActiveEntityScript<X> in a CCountedPointer<CActiveEntityScriptBase> (Ego_r pair<unsigned __int64, CCountedPointer<CActiveEntityScriptBase>>); it sits next to CActiveEntityScriptBase::CActiveEntityScriptBase 0x00CE1110; FableWin.pdb ??0?$CCountedPointer@VCActiveEntityScriptBase@@@@QAE@PAVCActiveEntityScriptBase@@@Z.
// Member: the in-class (T*) constructor of engine/CCountedPointer.h, emitted out of line as its own COMDAT (`ret 4`); the count
//   block stores CCountedPointer<T>::DeleteData (its own COMDAT; T is only forward-declared here).
#include "engine/CCountedPointer.h"
class CActiveEntityScriptBase;
template CCountedPointer<CActiveEntityScriptBase>::CCountedPointer(CActiveEntityScriptBase*);
