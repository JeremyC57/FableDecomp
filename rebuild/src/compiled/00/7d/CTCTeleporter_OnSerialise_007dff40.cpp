// CTCTeleporter::OnSerialise(CPersistContext&, bool) -- when the flag argument is set, persists Active (+0x10, key "TeleportActive") and Activated (+0x11, key "TeleportActivated") through CPersistContext::Transfer<bool>, default false
// Leaf: ?OnSerialise@CTCTeleporter@@UAEXAAVCPersistContext@@_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007DFF40, thiscall, `ret 8`.
// Identity (agentFAM03 proof, re-verified by agentFAM05): slot 1 of CTCTeleporter's vtable 0x0126FDA4 (the slot holds this body in 1 vtable(s), owner CTCTeleporter); the Xbox 360 Test PDB virtual order (third source) aligned on 16 TLC-named slots of that vtable puts OnSerialise(CPersistContext &, bool) at this slot [gap kind GS]; Ego_r/FableWin name set: ?OnSerialise@CTCTeleporter@@UAEXAAVCPersistContext@@_N@Z; `ret` matches the signature.
// Was: CTCTeleporter::vfunc_1 (rebuild/src/compiled/00/7d/CTCTeleporter_vfunc1_007dff40.cpp).
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCTELEPORTER_VSLOTS3
#include "engine/ETCInterfaceType.h"
#include "engine/CPersistContext.h"
#include "engine/CTCTeleporter.h"
void CTCTeleporter::OnSerialise(CPersistContext& context, bool transfer)
{
    if (transfer) {
        context.Transfer("TeleportActive", Active, false);
        context.Transfer("TeleportActivated", Activated, false);
    }
}
