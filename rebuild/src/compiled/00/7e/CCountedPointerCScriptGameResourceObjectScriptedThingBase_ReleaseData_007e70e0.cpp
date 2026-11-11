// CCountedPointer<CScriptGameResourceObjectScriptedThingBase>::ReleaseData() -- drop one reference (delete the pointee and the count block at zero), then zero Data and Info
// Leaf: ?ReleaseData@?$CCountedPointer@VCScriptGameResourceObjectScriptedThingBase@@@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007E70E0, 53 bytes. Callees: the count block's DeleteFunc (indirect), operator delete 0x00BFE9BC.
// Was landed as _global::CCountedPointer_Release_007e70e0 (stand-in name).
// T = CScriptGameResourceObjectScriptedThingBase, proven by: CScriptGameResourceObjectScriptedThingBase::Release 0x00CD2770 (PDB name, size TU) calls it on this+8 = PImp (Ego_r: CCountedPointer<CScriptGameResourceObjectScriptedThingBase> PImp +8), then zeroes Data (PImp.Reset(0) with ReleaseData out of line); FableWin.pdb ReleaseData name.
// Member: ReleaseData, not the destructor. Retail's ~CCountedPointer is `{ ReleaseData(); }` (Ego_r.pdb 5-byte ??1<NUISystem::CComponent>
//   = retail jmp 0x00429C53 -> ReleaseData 0x004291DE); VC7.1 expands that destructor at the call site, so a size-TU caller and
//   every ShareData / Reset call ReleaseData itself (probe: /O1 scalar deleting dtor and scope end call ReleaseData, /O2 ones call ??1).
// Model: engine/CCountedPointer.h opt-in FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA (destructor = ReleaseData(); ReleaseData zeroes Data first).
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#include "engine/CCountedPointer.h"
class CScriptGameResourceObjectScriptedThingBase;
template void CCountedPointer<CScriptGameResourceObjectScriptedThingBase>::ReleaseData();
