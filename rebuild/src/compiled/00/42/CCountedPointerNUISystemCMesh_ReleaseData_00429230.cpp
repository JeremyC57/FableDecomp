#pragma optimize("s",on)
// CCountedPointer::ReleaseData() -- drop one reference (delete the pointee and the count block at zero), then zero Data and Info (T = NUISystem::CMesh)
// Leaf: ?ReleaseData@?$CCountedPointer@VCMesh@NUISystem@@@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00429230, 44 bytes. Callees: the count block's DeleteFunc (indirect), operator delete 0x00BFE9BC.
// Was landed as _global::CCountedPointer_Release_00429230 (stand-in name).
// T = NUISystem::CMesh, proven by: SetStaticCast<CComponent> on CCountedPointer<NUISystem::CMesh> 0x0042A2B9 (CManager::Action binding) -> ShareData 0x0042943A -> 0x00429230; jmp thunk 0x00429D54 is the size-TU destructor; CManager::Action binds ReleaseData<CMesh>; FableWin.pdb ?ReleaseData@?$CCountedPointer@VCMesh@NUISystem@@@@QAEXXZ.
// Member: ReleaseData, not the destructor. Retail's ~CCountedPointer is `{ ReleaseData(); }` (Ego_r.pdb 5-byte ??1<NUISystem::CComponent>
//   = retail jmp 0x00429C53 -> ReleaseData 0x004291DE); VC7.1 expands that destructor at the call site, so a size-TU caller and
//   every ShareData / Reset call ReleaseData itself (probe: /O1 scalar deleting dtor and scope end call ReleaseData, /O2 ones call ??1).
// Model: engine/CCountedPointer.h opt-in FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA (destructor = ReleaseData(); ReleaseData zeroes Data first).
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#include "engine/CCountedPointer.h"
namespace NUISystem { class CMesh; }
template void CCountedPointer<NUISystem::CMesh>::ReleaseData();
