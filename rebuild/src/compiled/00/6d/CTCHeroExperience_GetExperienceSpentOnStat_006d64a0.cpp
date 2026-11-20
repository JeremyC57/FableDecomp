// CTCHeroExperience::GetExperienceSpentOnStat(EHeroTrainableStatType) const -- returns ExperienceSpentOnStats[stat], the hero's experience spent on that trainable stat
// Leaf: ?GetExperienceSpentOnStat@CTCHeroExperience@@QBEJW4EHeroTrainableStatType@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006D64A0, 13 bytes, thiscall, `ret 4` (`mov eax,[ecx+44h] ; mov ecx,[esp+4] ; mov eax,[eax+ecx*4]`).
// Identity (NF125 round 10): Ego_r.pdb 0x00587C10 / FableWin.pdb 0x01EEFBE0 ?GetExperienceSpentOnStat@CTCHeroExperience@@QBEJW4EHeroTrainableStatType@@@Z (public const); the Ego_r donor
//   order ForceSpendableExperienceTo 0x00587BE0 / GetExperienceSpentOnStat 0x00587C10 / GetStatExperience 0x00587DA0 / GetTrainableStatLevel
//   0x00587DB0 / GetCostToUpgradeTrainableStat 0x00587E00 is retail's 0x006D6470 / 0x006D64A0 / 0x006D6510 / 0x006D6520 / 0x006D67B0, and the body
//   reads ExperienceSpentOnStats at +0x44 of the retail layout. Was a free __fastcall GetAbilityLevel(S*) stand-in (build-entry Module / manifest label CTCInventoryAbilities::GetAbilityLevel).
// Header: engine/CTCHeroExperience.h, retail layout with the three stat vectors (FABLE_RETAIL_CTCHEROEXPERIENCE_STATVECTORS) and its GetExperienceSpentOnStat declaration (FABLE_RETAIL_CTCHEROEXPERIENCE_SPENTONSTAT).
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCHEROEXPERIENCE
#define FABLE_RETAIL_EHEROTRAINABLESTATTYPE
#define FABLE_RETAIL_CTCHEROEXPERIENCE_STATVECTORS
#define FABLE_RETAIL_CTCHEROEXPERIENCE_SPENTONSTAT
#include "engine/ETCInterfaceType.h"
#include "engine/CTCHeroExperience.h"
long CTCHeroExperience::GetExperienceSpentOnStat(EHeroTrainableStatType stat) const
{
    return ExperienceSpentOnStats[stat];
}
