// CSpecialAbilitiesDrainLifeDef::Transfer(CPersistContext& context) -- persist the drain-life spell def's four per-level vectors and its delay
// Leaf: ?Transfer@CSpecialAbilitiesDrainLifeDef@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004724C2, 68 bytes, virtual `ret 4`: the unnamed TransferVector<float> 0x00464CBF
//   (+0x3C), TransferVector<long> 0x00466A47 (+0x48), TransferVector<float> (+0x54), TransferVector<CCharString> 0x004331F9
//   (+0x60), then Transfer<float> 0x00431061 (+0x6C). Ego_r.pdb / FableWin.pdb ?Transfer@CSpecialAbilitiesDrainLifeDef@@UAEXAAVCPersistContext@@@Z.
// Port (round 4, NF40): was a local CPersistContext model with stand-in PersistA_00464cbf / PersistB_00466a47 / PersistC
//   members over opaque 12-byte fields; now engine/CSpecialAbilitiesDrainLifeDef.h (FABLE_RETAIL_CDRAINLIFEDEF_PASS22 layout:
//   AmountOfHealthSucked, NumEffects, SpellRange, ShotName, SpellDelaySecs; the override through
//   FABLE_RETAIL_CSPECIALABILITIESDRAINLIFEDEF_TRANSFER) and engine/CPersistContext.h.
#pragma optimize("s",on)
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDRAINLIFEDEF_PASS22
#define FABLE_RETAIL_CSPECIALABILITIESDRAINLIFEDEF_TRANSFER
#include "engine/CSpecialAbilitiesDrainLifeDef.h"
#include "engine/CPersistContext.h"

void CSpecialAbilitiesDrainLifeDef::Transfer(CPersistContext& context)
{
    context.TransferVector(AmountOfHealthSucked);
    context.TransferVector(NumEffects);
    context.TransferVector(SpellRange);
    context.TransferVector(ShotName);
    context.Transfer(SpellDelaySecs);
}
