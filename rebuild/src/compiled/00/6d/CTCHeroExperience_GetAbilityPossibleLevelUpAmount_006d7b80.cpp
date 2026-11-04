// CTCHeroExperience::GetAbilityPossibleLevelUpAmount(EHeroAbility ability, float& fraction) -- how many levels the hero could buy for an ability with the experience at hand (the ability's stat pool plus the spendable points), and the fraction of the next level's cost covered; -1 without an ability inventory
// Leaf: ?GetAbilityPossibleLevelUpAmount@CTCHeroExperience@@QAEJW4EHeroAbility@@AAM@Z
// Flags: none
// Retail 0x006D7B80, 297 bytes (`ret 8`). Ego_r.pdb ?GetAbilityPossibleLevelUpAmount@CTCHeroExperience@@QAEJW4EHeroAbility@@AAM@Z.
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound 0x0040F020 (inlined CThing::GetTC<CTCInventoryAbilities>, type
//   111, `test ch,ch ; jns`), CTCInventoryAbilities::IsAbilityPotentiallyAvailable 0x005EE02E, ::GetAbilityMaximumLevel
//   0x005EDEA3, ::GetAbilityLevel 0x005EDE93 (twice), ::GetAbilityStatType 0x005EDEEB, ::GetAbilityMaximumLevelWithRenown
//   0x005EE6BF (per pass), ::GetExperienceCostToUpgradeAbility(EHeroAbility, long, bool) 0x005EE77C.
// Model: the retail headers engine/CThing.h (GetTC<T>), engine/CTCHeroExperience.h (overlay FABLE_RETAIL_CTCHEROEXPERIENCE_STATEXPERIENCE:
//   StatExperiencePoints as std::vector<long>) and engine/CTCInventoryAbilities.h (overlay FABLE_RETAIL_CTCINVENTORYABILITIES_LEVELUP).
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTCHEROEXPERIENCE
#define FABLE_RETAIL_CTCHEROEXPERIENCE_STATEXPERIENCE
#define FABLE_RETAIL_CTCINVENTORYABILITIES
#define FABLE_RETAIL_CTCINVENTORYABILITIES_LEVELUP
#include "engine/CThing.h"
#include "engine/CTCHeroExperience.h"
#include "engine/CTCInventoryAbilities.h"

long CTCHeroExperience::GetAbilityPossibleLevelUpAmount(EHeroAbility ability, float& fraction)
{
    fraction = 0.0f;
    long available = ExperiencePointsAvailableToSpend;
    CTCInventoryAbilities* abilities;
    if (!ParentThing->GetTC(&abilities))
        return -1;
    if (abilities->IsAbilityPotentiallyAvailable(ability)) {
        long maxLevel = abilities->GetAbilityMaximumLevel(ability);
        if (abilities->GetAbilityLevel(ability) < maxLevel) {
            long experience = StatExperiencePoints[abilities->GetAbilityStatType(ability)] + available;
            long levels = 1;
            long nextLevel = abilities->GetAbilityLevel(ability) + 1;
            while (abilities->GetAbilityMaximumLevelWithRenown(ability) >= nextLevel) {
                long cost = abilities->GetExperienceCostToUpgradeAbility(ability, levels, true);
                if (cost == -1)
                    break;
                if (experience < cost) {
                    fraction = (float)experience / cost;
                    return levels - 1;
                }
                experience -= cost;
                ++levels;
                ++nextLevel;
            }
            return levels - 1;
        }
    }
    return 0;
}