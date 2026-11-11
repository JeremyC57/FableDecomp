#pragma optimize("s",on)
// CCountedPointer<CTexture>::operator=(const CCountedPointer&) -- share the other pointer: ShareData(Data, Info), return *this
// Leaf: ??4?$CCountedPointer@VCTexture@@@@QAEAAV0@ABV0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00429A62, 23 bytes. Callees: the count block's DeleteFunc (indirect), operator delete 0x00BFE9BC.
// Was landed as _global::CCountedPointer_Assign_00429a62 (stand-in name).
// T = CTexture, proven by: NUISystem::CManager::Action 0x0041E6D3 binds CCountedPointer<CTexture>::operator=; it calls ShareData 0x0042913A, whose release is ReleaseData<CTexture> 0x0041BB30 (also bound by CManager::Action); FableWin.pdb ??4?$CCountedPointer@VCTexture@@ name.
// Member: the in-class operator= of engine/CCountedPointer.h (loads Data / Info, then the ShareData body), emitted as its own COMDAT.
// Model: opt-in FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA (ReleaseData zeroes Data first; its stores die under ShareData's own).
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#include "engine/CCountedPointer.h"
class CTexture;
template CCountedPointer<CTexture>& CCountedPointer<CTexture>::operator=(const CCountedPointer<CTexture>&);
