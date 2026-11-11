#pragma optimize("s",on)
// CCountedPointer<CGameDefinitionManager>::Reset(T*) -- release the old pointee, take the new one and give it a fresh count block
// Leaf: ?Reset@?$CCountedPointer@VCGameDefinitionManager@@@@QAEXPAVCGameDefinitionManager@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00450142, 59 bytes. Callees: the count block's DeleteFunc (indirect), operator delete 0x00BFE9BC.
// Was landed as _global::CCountedPointer_SetPItem_00450142 (stand-in name).
// T = CGameDefinitionManager, proven by: it calls ReleaseData<CGameDefinitionManager> 0x0044F3DE first; its caller 0x0044C71F resets the static 0x013B879C that CGameDefinitionManager::Get 0x0044C6B0 returns; FableWin.pdb ?Reset@?$CCountedPointer@VCGameDefinitionManager@@@@QAEXPAVCGameDefinitionManager@@@Z.
// Member: the in-class Reset of engine/CCountedPointer.h (ReleaseData expanded; its Data store dies under Data = data), own COMDAT.
// The count block stores CCountedPointer<T>::DeleteData (its own COMDAT; T is only forward-declared here).
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#include "engine/CCountedPointer.h"
class CGameDefinitionManager;
template void CCountedPointer<CGameDefinitionManager>::Reset(CGameDefinitionManager*);
