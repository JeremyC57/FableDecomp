// CHeroAbilityDef::Transfer(CPersistContext&) -- transfer ability fields in persistence order.
// Leaf: ?Transfer@CHeroAbilityDef@@UAEXAAVCPersistContext@@@Z
// Retail00469ab3,215 bytes. The quarantined donor header has a 4-byte larger
// base and16-byte vectors; native base is0x3c and vectors12, verified by all
// field addresses below. The leading definition base is opaque in the PDB.
// Port (round 4, NF40): the local CPersistContext model (stand-in TransferAbility / TransferStat / TransferVector2 /
//   TransferLongVector / TransferDefIndex members), the NativeLongVector stand-in and the non-virtual QAE leaf are replaced by
//   engine/CHeroAbilityDef.h (its FABLE_RETAIL_CHEROABILITYDEF_TRANSFER opt-in declares the override
//   ?Transfer@CHeroAbilityDef@@UAEXAAVCPersistContext@@@Z, Ego_r.pdb / FableWin.pdb, and types DummyObject as the PDB's CDefIndex)
//   and engine/CPersistContext.h: Transfer<EHeroAbility> 0x00452E01, Transfer<EHeroExperienceStatCategory> 0x00469B8A,
//   Transfer<C2DVector> 0x004568BC, TransferVector<long> 0x00466A47, Transfer<CDefIndex> 0x004522F4.
#pragma optimize("s",on)
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CHEROABILITYDEF_TRANSFER
#include "engine/CHeroAbilityDef.h"
#include "engine/CPersistContext.h"

void CHeroAbilityDef::Transfer(CPersistContext& context)
{
    context.Transfer(Ability);
    context.Transfer(Stat);
    context.Transfer(IconGraphicIndex);
    context.Transfer(IconEffectIndex);
    context.Transfer(IconActiveEffectIndex);
    context.Transfer(IconEffectOffset);
    context.Transfer(Name);
    context.Transfer(Description);
    context.TransferVector(StaminaCost);
    context.Transfer(StatExperienceGainedOnUse);
    context.TransferVector(ExperienceCostsToUpgrade);
    context.TransferVector(LevDescription);
    context.Transfer(ABXYPriority);
    context.Transfer(MoralityCostFactor);
    context.Transfer(Aggressive);
    context.Transfer(MaxedOutDescription);
    context.Transfer(DummyObject);
}
