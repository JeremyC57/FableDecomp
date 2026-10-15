#pragma optimize("s", on)
// CTCInventoryExperience::UpdateExternalExperienceSpendAbility(EHeroAbility, long) -- forwards the experience spent on an ability to the hero's CTCInventoryAbilities (SetExperienceSpentOnAbility)
// Leaf: UpdateExternalExperienceSpendAbility
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00572689, length 23 bytes (unaligned, no padding; size-optimised inventory module).
// Manifest label "CTCHero::GiveTutorialForCategory" is propagated; the name is the Ego_r.pdb symbol
// ?UpdateExternalExperienceSpendAbility@CTCInventoryExperience@@QAEXW4EHeroAbility@@J@Z. One caller,
// CTCInventoryExperience::SpendExperienceOnAbility @ 0x005747A3 (call site 0x0057487A).
// Callees: CTCInventoryExperience::GetTCInventoryAbilities @ 0x0056FFC5 (manifest "GetTCHeroExperience" is a propagated twin
// label), CTCInventoryAbilities::SetExperienceSpentOnAbility @ 0x005EDF1F (stores into the vector at +0x148; manifest
// "OnRegistedZSortingPatchRelocation" is propagated).
// Shape: the twin of 0x00572672 -- both arguments re-pushed from the caller's frame before the getter, result in ecx, `ret 8`.
// Pragma: optimize("s", on).
// CTCInventoryAbilities_Calls: engine/CTCInventoryAbilities.h holds the PDB data layout of CTCInventoryAbilities; only its member call(s) are needed here, so the
// callee is declared on a separate call-only struct.
#include "engine/CTCInventoryExperience.h"

enum EHeroAbility { HERO_ABILITY_NULL };

struct CTCInventoryAbilities_Calls {
    void SetExperienceSpentOnAbility(EHeroAbility ability, long amount);       // 0x005EDF1F
};

struct CTCInventoryExperience_Methods : CTCInventoryExperience {
    CTCInventoryAbilities_Calls* GetTCInventoryAbilities();                          // 0x0056FFC5
    void UpdateExternalExperienceSpendAbility(EHeroAbility ability, long amount);
};

void CTCInventoryExperience_Methods::UpdateExternalExperienceSpendAbility(EHeroAbility ability, long amount)
{
    GetTCInventoryAbilities()->SetExperienceSpentOnAbility(ability, amount);
}
