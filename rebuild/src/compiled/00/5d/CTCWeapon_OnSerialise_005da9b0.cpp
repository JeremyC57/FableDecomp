// CTCWeapon::OnSerialise(CPersistContext&, bool) -- persists TimeWielded (+0x20, CPersistContext::Transfer<long>, default 0) and Wielded (+0x24, Transfer<bool>, default false)
// Leaf: ?OnSerialise@CTCWeapon@@UAEXAAVCPersistContext@@_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005DA9B0, thiscall, `ret 8`.
// Identity (agentFAM03 proof, re-verified by agentFAM05): slot 1 of CTCWeapon's vtable 0x012554FC (the slot holds this body in 1 vtable(s), owner CTCWeapon); the Xbox 360 Test PDB virtual order (third source) aligned on 15 TLC-named slots of that vtable puts OnSerialise(CPersistContext &, bool) at this slot [gap kind GS]; Ego_r/FableWin name set: ?OnSerialise@CTCWeapon@@UAEXAAVCPersistContext@@_N@Z; `ret` matches the signature.
// Was: CTCWeapon::vfunc_1 (rebuild/src/compiled/00/5d/CTCWeapon_vfunc1_005da9b0.cpp).
#define FABLE_RETAIL_CTCWEAPON_VSLOTS3
#define FABLE_RETAIL_CTCWEAPON
#define FABLE_RETAIL_CTCWEAPON_MEMBERS
#include "engine/CPersistContext.h"
#include "engine/CTCWeapon.h"
void CTCWeapon::OnSerialise(CPersistContext& context, bool)
{
    context.Transfer("TimeWielded", TimeWielded, 0L);
    context.Transfer("Wielded", Wielded, false);
}
