#pragma optimize("s",on)
// CCountedPointer<CPlayerGui>::ReleaseData() -- drop one reference (delete the pointee and the count block at zero), then zero Data and Info
// Leaf: ?ReleaseData@?$CCountedPointer@VCPlayerGui@@@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0041915A, 44 bytes. Callees: the count block's DeleteFunc (indirect), operator delete 0x00BFE9BC.
// Was landed as _global::CCountedPointer_Release_0041915a (stand-in name).
// T = CPlayerGui, proven by: CMainGameComponent::Shutdown 0x004175E5 calls it on 0x013B8790 = CTBaseSingletonNoDefaultAlloc<CPlayerGui>::PClass (landed PDB spelling), size-TU caller (dtor inlined to a ReleaseData call); Reset 0x004195AF calls it on a live pointer; FableWin.pdb ?ReleaseData@?$CCountedPointer@VCPlayerGui@@@@QAEXXZ.
// Member: ReleaseData, not the destructor. Retail's ~CCountedPointer is `{ ReleaseData(); }` (Ego_r.pdb 5-byte ??1<NUISystem::CComponent>
//   = retail jmp 0x00429C53 -> ReleaseData 0x004291DE); VC7.1 expands that destructor at the call site, so a size-TU caller and
//   every ShareData / Reset call ReleaseData itself (probe: /O1 scalar deleting dtor and scope end call ReleaseData, /O2 ones call ??1).
// Model: engine/CCountedPointer.h opt-in FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA (destructor = ReleaseData(); ReleaseData zeroes Data first).
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#include "engine/CCountedPointer.h"
class CPlayerGui;
template void CCountedPointer<CPlayerGui>::ReleaseData();
