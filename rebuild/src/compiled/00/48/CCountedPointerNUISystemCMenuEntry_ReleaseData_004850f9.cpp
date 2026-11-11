#pragma optimize("s",on)
// CCountedPointer::ReleaseData() -- drop one reference (delete the pointee and the count block at zero), then zero Data and Info (T = NUISystem::CMenuEntry)
// Leaf: ?ReleaseData@?$CCountedPointer@VCMenuEntry@NUISystem@@@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004850F9, 44 bytes. Callees: the count block's DeleteFunc (indirect), operator delete 0x00BFE9BC.
// Was landed as _global::CCountedPointer_Release_004850f9 (stand-in name).
// T = NUISystem::CMenuEntry, proven by: CTCInventoryClothing::ConstructScreen 0x005B76D1 binds ReleaseData<NUISystem::CMenuEntry> (engine/CCountedPointer.h names the CMenuEntry pair 0x004850F9); ShareData 0x0048524C calls it; jmp thunk 0x00485501; FableWin.pdb ?ReleaseData@?$CCountedPointer@VCMenuEntry@NUISystem@@@@QAEXXZ.
// Member: ReleaseData, not the destructor. Retail's ~CCountedPointer is `{ ReleaseData(); }` (Ego_r.pdb 5-byte ??1<NUISystem::CComponent>
//   = retail jmp 0x00429C53 -> ReleaseData 0x004291DE); VC7.1 expands that destructor at the call site, so a size-TU caller and
//   every ShareData / Reset call ReleaseData itself (probe: /O1 scalar deleting dtor and scope end call ReleaseData, /O2 ones call ??1).
// Model: engine/CCountedPointer.h opt-in FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA (destructor = ReleaseData(); ReleaseData zeroes Data first).
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#include "engine/CCountedPointer.h"
namespace NUISystem { class CMenuEntry; }
template void CCountedPointer<NUISystem::CMenuEntry>::ReleaseData();
