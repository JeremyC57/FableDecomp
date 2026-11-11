// CCountedPointer<CCodeSectionManager>::Reset(T*) -- release the old pointee, take the new one and give it a fresh count block
// Leaf: ?Reset@?$CCountedPointer@VCCodeSectionManager@@@@QAEXPAVCCodeSectionManager@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00413590, 103 bytes. Callees: the count block's DeleteFunc (indirect), operator delete 0x00BFE9BC.
// Was landed as _global::CCountedPointer_Set_00413590 (stand-in name).
// T = CCodeSectionManager, proven by: every caller resets the static at 0x013CAA68 that CCodeSectionManagerSingleton::Get 0x00414CE0 returns, with `new CCodeSectionManager` (ctor 0x009F0320) or 0; CXMVPlayer::StaticLoadXMVCode 0x009BCF70 binds ?Reset@?$CCountedPointer@VCCodeSectionManager@@@@QAEXPAVCCodeSectionManager@@@Z; FableWin.pdb Reset name.
// Member: the in-class Reset of engine/CCountedPointer.h (ReleaseData expanded; its Data store dies under Data = data), own COMDAT.
// The count block stores CCountedPointer<T>::DeleteData (its own COMDAT; T is only forward-declared here).
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#include "engine/CCountedPointer.h"
class CCodeSectionManager;
template void CCountedPointer<CCodeSectionManager>::Reset(CCodeSectionManager*);
