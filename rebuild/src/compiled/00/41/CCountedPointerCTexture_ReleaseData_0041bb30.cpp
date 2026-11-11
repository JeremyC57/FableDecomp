// CCountedPointer<CTexture>::ReleaseData() -- drop one reference (delete the pointee and the count block at zero), then zero Data and Info
// Leaf: ?ReleaseData@?$CCountedPointer@VCTexture@@@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0041BB30, 53 bytes. Callees: the count block's DeleteFunc (indirect), operator delete 0x00BFE9BC.
// Was landed as _global::CCountedPointer_Release_0041bb30 (stand-in name).
// T = CTexture, proven by: ShareData 0x0042913A (called by operator= 0x00429A62, which CManager::Action binds as CCountedPointer<CTexture>::operator=) calls it on a live pointer; CManager::Action (size TU) binds ReleaseData<CTexture>; FableWin.pdb ?ReleaseData@?$CCountedPointer@VCTexture@@@@QAEXXZ.
// Member: ReleaseData, not the destructor. Retail's ~CCountedPointer is `{ ReleaseData(); }` (Ego_r.pdb 5-byte ??1<NUISystem::CComponent>
//   = retail jmp 0x00429C53 -> ReleaseData 0x004291DE); VC7.1 expands that destructor at the call site, so a size-TU caller and
//   every ShareData / Reset call ReleaseData itself (probe: /O1 scalar deleting dtor and scope end call ReleaseData, /O2 ones call ??1).
// Model: engine/CCountedPointer.h opt-in FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA (destructor = ReleaseData(); ReleaseData zeroes Data first).
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#include "engine/CCountedPointer.h"
class CTexture;
template void CCountedPointer<CTexture>::ReleaseData();
