// CHeroAbilityDef::Transfer(CPersistContext&) -- transfer ability fields in persistence order.
// Retail00469ab3,215 bytes. The quarantined donor header has a 4-byte larger
// base and16-byte vectors; native base is0x3c and vectors12, verified by all
// field addresses below. The leading definition base is opaque in the PDB.
#pragma optimize("s",on)
#define CHeroAbilityDef CHeroAbilityDefDonor
#include "engine/_quarantine/CHeroAbilityDef.h"
#undef CHeroAbilityDef
struct C2DVector {float X,Y;};
struct NativeLongVector {int* Start;int* Finish;int* End;};
struct CPersistContext {
 void TransferAbility(int&); //00452e01
 void TransferStat(int&); //00469b8a
 void TransferLong(int&); //00431102
 void TransferVector2(C2DVector&); //004568bc
 void TransferLongVector(NativeLongVector&); //00466a47
 void TransferFloat(float&); //00431061
 void TransferBool(bool&); //0043314a
 void TransferDefIndex(int&); //004522f4
};
struct CHeroAbilityDef {
 unsigned char DefinitionBase[0x3c];
 int Ability,Stat,IconGraphicIndex,IconEffectIndex,IconActiveEffectIndex;
 C2DVector IconEffectOffset; int Name,Description;
 NativeLongVector LevDescription;int MaxedOutDescription;
 NativeLongVector StaminaCost;int StatExperienceGainedOnUse;
 NativeLongVector ExperienceCostsToUpgrade;int ABXYPriority;
 float MoralityCostFactor;bool Aggressive;int DummyObject;
 void Transfer(CPersistContext&);
};
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CHeroAbilityDef,Ability)==0x3c);
FABLE_STATIC_ASSERT(offsetof(CHeroAbilityDef,StaminaCost)==0x70);
FABLE_STATIC_ASSERT(offsetof(CHeroAbilityDef,DummyObject)==0x98);
#endif
void CHeroAbilityDef::Transfer(CPersistContext& c){
 c.TransferAbility(Ability);c.TransferStat(Stat);
 c.TransferLong(IconGraphicIndex);c.TransferLong(IconEffectIndex);c.TransferLong(IconActiveEffectIndex);
 c.TransferVector2(IconEffectOffset);c.TransferLong(Name);c.TransferLong(Description);
 c.TransferLongVector(StaminaCost);c.TransferLong(StatExperienceGainedOnUse);
 c.TransferLongVector(ExperienceCostsToUpgrade);c.TransferLongVector(LevDescription);
 c.TransferLong(ABXYPriority);c.TransferFloat(MoralityCostFactor);c.TransferBool(Aggressive);
 c.TransferLong(MaxedOutDescription);c.TransferDefIndex(DummyObject);
}