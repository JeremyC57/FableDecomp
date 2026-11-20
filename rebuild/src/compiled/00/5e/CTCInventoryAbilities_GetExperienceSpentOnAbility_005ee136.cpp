// CTCInventoryAbilities::GetExperienceSpentOnAbility(EHeroAbility ability) const -- the experience spent so far on one ability
// Leaf: ?GetExperienceSpentOnAbility@CTCInventoryAbilities@@QBEJW4EHeroAbility@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005EE136, 16 bytes (`mov eax,[ecx+148h] ; mov ecx,[esp+4] ; mov eax,[eax+ecx*4] ; ret 4`).
// Identity (NF168, 2026-11-20): Ego_r.pdb 0x0057CC91 / FableWin.pdb 0x01EBC6E6 ?GetExperienceSpentOnAbility@CTCInventoryAbilities@@QBEJW4EHeroAbility@@@Z
//   (public const) + the body: element `ability` of ExperienceSpentOnAbilities (std::vector<long>, Ego_r +0x148, the engine/CTCInventoryAbilities.h
//   retail layout). The catalog label GetAbilityLevel belongs to 0x005EDE93 (the +0x160 Abilities vector, same 16-byte shape). Sole landed
//   caller: 0x005724CD InitialiseExperienceSpendValues (the ability spend values), respelled in the same hand-back. Was the stand-in S::GetAbilityLevel.
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCINVENTORYABILITIES
#define FABLE_RETAIL_CTCINVENTORYABILITIES_GETEXPERIENCESPENTONABILITY
#include "engine/ETCInterfaceType.h"
#include "engine/CTCInventoryAbilities.h"

long CTCInventoryAbilities::GetExperienceSpentOnAbility(EHeroAbility ability) const
{
    return ExperienceSpentOnAbilities[ability];
}
