// CTCGatherPointBase::OnSerialise(CPersistContext&, bool) -- persists MaxOccupation (+0x18) through CPersistContext::Transfer<long> (default 0), then calls the protected leaf hook OnSerialiseLeaf (slot 30)
// Leaf: ?OnSerialise@CTCGatherPointBase@@UAEXAAVCPersistContext@@_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007FFE40, thiscall, `ret 8`.
// Identity (agentFAM03 proof, re-verified by agentFAM05): slot 1 of CTCGatherPointRadialArea's vtable 0x012422E4 (the slot holds this body in 3 vtable(s), owner CTCGatherPointBase); the Xbox 360 Test PDB virtual order (third source) aligned on 12 TLC-named slots of that vtable puts OnSerialise(CPersistContext &, bool) at this slot [gap kind GS]; Ego_r/FableWin name set: ?OnSerialise@CTCGatherPointBase@@UAEXAAVCPersistContext@@_N@Z; `ret` matches the signature.
// Was: CTCGatherPointBase::vfunc_1 (rebuild/src/compiled/00/7f/CTCGatherPointBase_vfunc1_007ffe40.cpp).
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCGATHERPOINTBASE_VSLOTS3
#include "engine/ETCInterfaceType.h"
#include "engine/CPersistContext.h"
#include "engine/CTCGatherPointBase.h"
void CTCGatherPointBase::OnSerialise(CPersistContext& context, bool)
{
    context.Transfer("MaxOccupation", MaxOccupation, 0L);
    OnSerialiseLeaf(context);
}
