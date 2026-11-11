#pragma optimize("s",on)
// CCountedPointer::ReleaseData() -- drop one reference (delete the pointee and the count block at zero), then zero Data and Info (T = NHitLocations::CPhysicalPrimitiveBase)
// Leaf: ?ReleaseData@?$CCountedPointer@VCPhysicalPrimitiveBase@NHitLocations@@@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0044F3B2, 44 bytes. Callees: the count block's DeleteFunc (indirect), operator delete 0x00BFE9BC.
// Was landed as _global::CCountedPointer_Release_0044f3b2 (stand-in name).
// T = NHitLocations::CPhysicalPrimitiveBase, proven by: CHitLocationDef's scalar deleting dtor 0x0045887F releases +0x3C = CPhysicalPrimitiveInit::PPrimitive (Ego_r: CHitLocationDef +0x40 CPhysicalPrimitiveInit, whose only member is CCountedPointer<NHitLocations::CPhysicalPrimitiveBase>; retail def offsets run 4 low); Transfer<CPhysicalPrimitiveInit> 0x00458833 binds it; ShareData 0x00461B5C calls it; jmp thunks 0x00450195 / 0x00450BDE; FableWin.pdb ReleaseData name.
// Member: ReleaseData, not the destructor. Retail's ~CCountedPointer is `{ ReleaseData(); }` (Ego_r.pdb 5-byte ??1<NUISystem::CComponent>
//   = retail jmp 0x00429C53 -> ReleaseData 0x004291DE); VC7.1 expands that destructor at the call site, so a size-TU caller and
//   every ShareData / Reset call ReleaseData itself (probe: /O1 scalar deleting dtor and scope end call ReleaseData, /O2 ones call ??1).
// Model: engine/CCountedPointer.h opt-in FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA (destructor = ReleaseData(); ReleaseData zeroes Data first).
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#include "engine/CCountedPointer.h"
namespace NHitLocations { class CPhysicalPrimitiveBase; }
template void CCountedPointer<NHitLocations::CPhysicalPrimitiveBase>::ReleaseData();
