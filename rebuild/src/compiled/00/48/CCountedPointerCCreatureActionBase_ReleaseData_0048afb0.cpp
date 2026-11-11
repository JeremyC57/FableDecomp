// CCountedPointer<CCreatureActionBase>::ReleaseData() -- drop one reference (delete the pointee and the count block at zero), then zero Data and Info
// Leaf: ?ReleaseData@?$CCountedPointer@VCCreatureActionBase@@@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0048AFB0, 53 bytes. Callees: the count block's DeleteFunc (indirect), operator delete 0x00BFE9BC.
// Was landed as _global::CCountedPointer_Release_0048afb0 (stand-in name).
// T = CCreatureActionBase, proven by: ~CCreatureAction_UnsheatheItemFromInventory 0x005CC091 (size TU, unpadded) releases +0xB0 = PFollowUpAction (Ego_r +0xBC, the class's only CCountedPointer: CCountedPointer<CCreatureActionBase>); its speed-TU twin 0x0048C5C0 is the destructor ~CCountedPointer<CCreatureActionBase> (called from padded /O2 scalar deleting dtors); FableWin.pdb ReleaseData name.
// Member: ReleaseData, not the destructor. Retail's ~CCountedPointer is `{ ReleaseData(); }` (Ego_r.pdb 5-byte ??1<NUISystem::CComponent>
//   = retail jmp 0x00429C53 -> ReleaseData 0x004291DE); VC7.1 expands that destructor at the call site, so a size-TU caller and
//   every ShareData / Reset call ReleaseData itself (probe: /O1 scalar deleting dtor and scope end call ReleaseData, /O2 ones call ??1).
// Model: engine/CCountedPointer.h opt-in FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA (destructor = ReleaseData(); ReleaseData zeroes Data first).
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#include "engine/CCountedPointer.h"
class CCreatureActionBase;
template void CCountedPointer<CCreatureActionBase>::ReleaseData();
