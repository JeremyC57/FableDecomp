// CTattooDef::GetTypeBalancedAttractiveness(const std::vector<CAppearanceModifierScalingDef>&) const -- attractiveness scaled by the tattoo-type entry when all five type scalings exist
// Leaf: ?GetTypeBalancedAttractiveness@CTattooDef@@QBEMABV?$vector@VCAppearanceModifierScalingDef@@V?$allocator@VCAppearanceModifierScalingDef@@@std@@@std@@@Z
// Retail: Fable.exe @ 0x0057FC46, 43 bytes, `ret 4`, result on the x87 stack; Ego_r.pdb
// ?GetTypeBalancedAttractiveness@CTattooDef@@QBEMABV?$vector@VCAppearanceModifierScalingDef@@...@Z. The siblings
// GetTypeBalancedScariness (0x0057FC71, +0x58 / entry +0x40) and GetTypeBalancedGoodstrength (0x0057FC9C, +0x5c / entry
// +0x44) follow with the same shape. Body: if the vector holds 5 entries, scalings[4].Attractiveness * Attractiveness,
// else Attractiveness.
// Layout: engine/CTattooDef.h (Attractiveness +0x54, as Ego_r.pdb). CAppearanceModifierScalingDef is 0x48 bytes in
// retail (`idiv 48h`; Ego_r.pdb 0x4c) with Attractiveness at +0x3c (entry 4 at +0x15c; Ego_r.pdb +0x40).
// NF39 (2026-11-16): moved off its `CTattooDef_Methods` stand-in and local CAppearanceModifierScalingDef / std::vector models onto the shared
//   engine/CTattooDef.h (opt-ins MEMBERS + TYPEBALANCED, which names 0x0057FC46), engine/CAppearanceModifierScalingDef.h (retail 0x48-byte def)
//   and stlport/fable_containers.h (the vector with its allocator argument: the Ego_r.pdb spelling). Defines from the landed 0x005841F9.
// Opt: size TU (unaligned entry, `push 48h ; pop edi` for the divisor).
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTATTOODEF
#define FABLE_RETAIL_CTATTOODEF_MEMBERS
#define FABLE_RETAIL_CTATTOODEF_TYPEBALANCED
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CDefClassBase.h"
#include "engine/CTattooDef.h"
#include "engine/CAppearanceModifierScalingDef.h"

float CTattooDef::GetTypeBalancedAttractiveness(const std::vector<CAppearanceModifierScalingDef>& scalings) const
{
    if (scalings.size() == 5)
        return scalings[4].Attractiveness * Attractiveness;
    return Attractiveness;
}
