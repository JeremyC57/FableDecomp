#pragma optimize("s",on)
// CCountedPointer<CStartupDefinitionManager>::ReleaseData() -- drop one reference (delete the pointee and the count block at zero), then zero Data and Info
// Leaf: ?ReleaseData@?$CCountedPointer@VCStartupDefinitionManager@@@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00433CC0, 44 bytes. Callees: the count block's DeleteFunc (indirect), operator delete 0x00BFE9BC.
// Was landed as _global::CCountedPointer_Release_00433cc0 (stand-in name).
// T = CStartupDefinitionManager, proven by: the atexit destructor 0x01228AF1 of the static at 0x013B8760 (the instance pointer CStartupDefinitionManager::Get 0x0043368D returns) tail-jumps here; Reset 0x00433D3A calls it on the live pointer; jmp thunk 0x00433D35 is the size-TU destructor; FableWin.pdb ?ReleaseData@?$CCountedPointer@VCStartupDefinitionManager@@@@QAEXXZ.
// Member: ReleaseData, not the destructor. Retail's ~CCountedPointer is `{ ReleaseData(); }` (Ego_r.pdb 5-byte ??1<NUISystem::CComponent>
//   = retail jmp 0x00429C53 -> ReleaseData 0x004291DE); VC7.1 expands that destructor at the call site, so a size-TU caller and
//   every ShareData / Reset call ReleaseData itself (probe: /O1 scalar deleting dtor and scope end call ReleaseData, /O2 ones call ??1).
// Model: engine/CCountedPointer.h opt-in FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA (destructor = ReleaseData(); ReleaseData zeroes Data first).
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#include "engine/CCountedPointer.h"
class CStartupDefinitionManager;
template void CCountedPointer<CStartupDefinitionManager>::ReleaseData();
