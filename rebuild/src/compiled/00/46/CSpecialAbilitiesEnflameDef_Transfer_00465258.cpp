// CSpecialAbilitiesEnflameDef::Transfer(CPersistContext& context) -- persist the def: LevelUpTimings[3], Damage[4], the RadiusOfEffect vector, the four camera float[4] arrays and EnflameCombatMultiplierFactor
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00465258, length 312 bytes (vtable slot of CSpecialAbilitiesEnflameDef).
// Callees: CPersistContext::Transfer(float&) @ 0x00431061, CPersistContext::Transfer(vector<float>&) @ 0x00464CBF.
// Layout: Ego_r.pdb names; the TLC def root ends at +0x3c (PDB 0x40) and the retail STLport vector is 12 bytes:
// CamDist +0x3c, CamHeight +0x4c, CamFOV +0x5c, CamSwitchTime +0x6c, LevelUpTimings +0x7c, Damage +0x88,
// RadiusOfEffect +0x98, EnflameCombatMultiplierFactor +0xa4 (0xa8 bytes).
// Shape: the context reference lives in edi, every element address is formed with `lea eax ; push eax ; mov ecx,edi`; the
// last member's address is formed in place (`add esi,0A4h`). The array elements are transferred one call each (written
// out, as the retail source order is not the member order: the level/damage block first, then the camera block).
// Size-optimised def module (`#pragma optimize("s", on)`): speed picks ecx for each `lea` (ecx is dead before `mov ecx,edi`).
// Leaf: ?Transfer@CSpecialAbilitiesEnflameDef@@UAEXAAVCPersistContext@@@Z
#pragma optimize("s", on)
// Port (round 4, NF40): the local CPersistContext / def-class models (and any vector stand-ins) are replaced by
//   engine/CSpecialAbilitiesEnflameDef.h (its FABLE_RETAIL_CSPECIALABILITIESENFLAMEDEF_TRANSFER opt-in declares the override) and engine/CPersistContext.h; vector members go through the unnamed TransferVector<T>.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CSPECIALABILITIESENFLAMEDEF_TRANSFER
#include "engine/CSpecialAbilitiesEnflameDef.h"
#include "engine/CPersistContext.h"

void CSpecialAbilitiesEnflameDef::Transfer(CPersistContext& context)
{
    context.Transfer(LevelUpTimings[0]);
    context.Transfer(LevelUpTimings[1]);
    context.Transfer(LevelUpTimings[2]);
    context.Transfer(Damage[0]);
    context.Transfer(Damage[1]);
    context.Transfer(Damage[2]);
    context.Transfer(Damage[3]);
    context.TransferVector(RadiusOfEffect);
    context.Transfer(CamDist[0]);
    context.Transfer(CamDist[1]);
    context.Transfer(CamDist[2]);
    context.Transfer(CamDist[3]);
    context.Transfer(CamHeight[0]);
    context.Transfer(CamHeight[1]);
    context.Transfer(CamHeight[2]);
    context.Transfer(CamHeight[3]);
    context.Transfer(CamFOV[0]);
    context.Transfer(CamFOV[1]);
    context.Transfer(CamFOV[2]);
    context.Transfer(CamFOV[3]);
    context.Transfer(CamSwitchTime[0]);
    context.Transfer(CamSwitchTime[1]);
    context.Transfer(CamSwitchTime[2]);
    context.Transfer(CamSwitchTime[3]);
    context.Transfer(EnflameCombatMultiplierFactor);
}