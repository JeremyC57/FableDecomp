// CTCBoastingCrowdControl::OnSerialise(CPersistContext&, bool) -- persists CrowdSizesSuppressed (+0x1c) through CPersistContext::Transfer<bool> (default false) when the flag argument is set
// Leaf: ?OnSerialise@CTCBoastingCrowdControl@@UAEXAAVCPersistContext@@_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00783210, thiscall, `ret 8`.
// Identity (agentFAM03 proof, re-verified by agentFAM05): slot 1 of CTCBoastingCrowdControl's vtable 0x0126B3AC (the slot holds this body in 1 vtable(s), owner CTCBoastingCrowdControl); the Xbox 360 Test PDB virtual order (third source) aligned on 13 TLC-named slots of that vtable puts OnSerialise(CPersistContext &, bool) at this slot [gap kind GS]; Ego_r/FableWin name set: ?OnSerialise@CTCBoastingCrowdControl@@UAEXAAVCPersistContext@@_N@Z; `ret` matches the signature.
// Was: CTCBoastingCrowdControl::vfunc_1 (rebuild/src/compiled/00/78/CTCBoastingCrowdControl_vfunc1_00783210.cpp).
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CQUESTMANAGER
#include "engine/CPersistContext.h"
#include "engine/CTCBoastingCrowdControl.h"
void CTCBoastingCrowdControl::OnSerialise(CPersistContext& context, bool transfer)
{
    if (transfer)
        context.Transfer("CrowdSizesSuppressed", CrowdSizesSuppressed, false);
}
