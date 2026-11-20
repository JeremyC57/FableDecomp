// CTCHeroExperience::GetStatExperience(EHeroExperienceStatCategory) const -- returns StatExperiencePoints[stat_category], the hero's experience in that stat category (strength / skill / will)
// Leaf: ?GetStatExperience@CTCHeroExperience@@QBEJW4EHeroExperienceStatCategory@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006D6510, 13 bytes, thiscall, `ret 4` (`mov eax,[ecx+18h] ; mov ecx,[esp+4] ; mov eax,[eax+ecx*4]`).
// Identity (NF125 round 10): Ego_r.pdb 0x00587DA0 / FableWin.pdb 0x01EF42B0 ?GetStatExperience@CTCHeroExperience@@QBEJW4EHeroExperienceStatCategory@@@Z (public const); the Ego_r donor
//   order ForceSpendableExperienceTo 0x00587BE0 / GetExperienceSpentOnStat 0x00587C10 / GetStatExperience 0x00587DA0 / GetTrainableStatLevel
//   0x00587DB0 / GetCostToUpgradeTrainableStat 0x00587E00 is retail's 0x006D6470 / 0x006D64A0 / 0x006D6510 / 0x006D6520 / 0x006D67B0, and the body
//   reads StatExperiencePoints at +0x18 of the retail layout. Was a free __fastcall GetAbilityLevel(S*) stand-in (build-entry Module / manifest label CTCInventoryAbilities::GetAbilityLevel).
// Header: engine/CTCHeroExperience.h, retail layout with the three stat vectors (FABLE_RETAIL_CTCHEROEXPERIENCE_STATVECTORS).
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCHEROEXPERIENCE
#define FABLE_RETAIL_EHEROTRAINABLESTATTYPE
#define FABLE_RETAIL_CTCHEROEXPERIENCE_STATVECTORS
#include "engine/ETCInterfaceType.h"
#include "engine/CTCHeroExperience.h"
long CTCHeroExperience::GetStatExperience(EHeroExperienceStatCategory stat_category) const
{
    return StatExperiencePoints[stat_category];
}
