// CTCTargeted::OnSerialise(CPersistContext&, bool) -- persists the Targetable flag (+0x10) through CPersistContext::Transfer<bool>, default false
// Leaf: ?OnSerialise@CTCTargeted@@MAEXAAVCPersistContext@@_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007EA550, thiscall, `ret 8`.
// Identity (agentFAM03 proof, re-verified by agentFAM05): slot 1 of CTCTargeted's vtable 0x0123F024 (the slot holds this body in 1 vtable(s), owner CTCTargeted); the Xbox 360 Test PDB virtual order (third source) aligned on 15 TLC-named slots of that vtable puts OnSerialise(CPersistContext &, bool) at this slot [gap kind GS]; Ego_r/FableWin name set: ?OnSerialise@CTCTargeted@@MAEXAAVCPersistContext@@_N@Z; `ret` matches the signature.
// Was: CTCTargeted::vfunc_1 (rebuild/src/compiled/00/7e/CTCTargeted_vfunc1_007ea550.cpp).
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCTARGETED
#define FABLE_RETAIL_CTCTARGETED_VSLOTS3
#include "engine/CPersistContext.h"
#include "engine/CTCTargeted.h"
void CTCTargeted::OnSerialise(CPersistContext& context, bool)
{
    context.Transfer("Targetable", Targetable, false);
}
