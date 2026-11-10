// CTCInventoryExperience::RecheckAvailabilities() -- drop every stat and ability entry the hero can no longer afford from the upgradable lists, refresh the four experience values, and close the screen when nothing is left to buy
// Leaf: ?RecheckAvailabilities@CTCInventoryExperience@@AAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x005726A0, 824 bytes, thiscall, frame-pointer size TU (`optimize("y", off)` + `("s", on)`,
//   as the landed TU-mates 0x00572636 / 0x005747A3), `leave ; ret`. Identity: Ego_r.pdb and FableWin.pdb
//   ?RecheckAvailabilities@CTCInventoryExperience@@AAEXXZ (private, no arguments): the body re-tests GetCostToUpgradeTrainableStat /
//   GetExperienceCostToUpgradeAbility against the stat's category experience plus the general pool; the catalog label
//   "UpdateAbilityDisplay" is in neither PDB.
// Shape: for each UpgradableStats / UpgradableAbilities entry (+0x224 / +0x230) still below its maximum level whose cost is now
//   above the available experience, the menu entry gets a UI_DELETE_THIS_INSTANCE deletion and the entry is erased (STLport
//   map::erase(iterator), out of line 0x00579300 / 0x0057926B; `it++` copy kept by hand as retail increments before erasing).
//   Then the general / strength / dexterity / will value texts of the screen component (Components[1]) are rewritten, and an
//   empty pair of lists queues the close event (base slot 32 AddCloseInventoryEvent).
// Levers: FABLE_RETAIL_CDELETION_BODIES (visible noinline CDeletion constructors) puts the `Method = 1` store right before the copy
//   constructor call, as retail (0x00572745 / 0x0057283E); FABLE_RETAIL_CTCINVENTORYABILITIES_MAXLEVEL_FIRST (GetAbilityMaximumLevel
//   declared before GetAbilityLevel): VC7.1 calls the later-declared callee of `Level < Max` first, and retail calls GetAbilityLevel
//   first in the ability loop (0x005727C8) but GetTrainableStatMaxLevel first in the stat loop (0x005726D3).
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_VTABLE
#define FABLE_RETAIL_CTCHEROEXPERIENCE
#define FABLE_RETAIL_EHEROTRAINABLESTATTYPE
#define FABLE_RETAIL_CTCINVENTORYEXPERIENCE
#define FABLE_RETAIL_CTCINVENTORYEXPERIENCE_PRIVATE
#define FABLE_RETAIL_CTCINVENTORYABILITIES
#define FABLE_RETAIL_CTCINVENTORYABILITIES_LEVELUP
#define FABLE_RETAIL_CTCINVENTORYABILITIES_MAXLEVEL_FIRST
#define FABLE_RETAIL_CDELETION
#define FABLE_RETAIL_CDELETION_BODIES
#define FABLE_RETAIL_CTEXT
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_TREE_ERASE
#include "engine/ETCInterfaceType.h"
#include "engine/CTCHeroExperience.h"
#include "engine/CTCInventoryExperience.h"
#include "engine/CTCInventoryAbilities.h"
#include "engine/NUISystem_CDeletion.h"
#include "engine/CText.h"
CWideString __fastcall GFIntToWideString(long value);                                // 0x00405F00
using namespace NUISystem;

void CTCInventoryExperience::RecheckAvailabilities()
{
    long general = GetTCHeroExperience()->GetExperiencePointsAvailableToSpend();
    for (std::map<EHeroTrainableStatType, CComponent*>::iterator it = UpgradableStats.begin(); it != UpgradableStats.end(); ) {
        if (GetTCHeroExperience()->GetTrainableStatLevel(it->first) < GetTCHeroExperience()->GetTrainableStatMaxLevel(it->first) &&
            GetTCHeroExperience()->GetCostToUpgradeTrainableStat(it->first) >
                GetTCHeroExperience()->GetStatExperience(GetTCHeroExperience()->GetCategoryTrainableStatIsIn(it->first)) + general) {
            CDeletion deletion;
            deletion.Method = CDeletion::UI_DELETE_THIS_INSTANCE;
            it->second->SetDeletion(deletion);
            std::map<EHeroTrainableStatType, CComponent*>::iterator next = it;
            ++next;
            UpgradableStats.erase(it);
            it = next;
        } else
            ++it;
    }
    for (std::map<EHeroAbility, CComponent*>::iterator it = UpgradableAbilities.begin(); it != UpgradableAbilities.end(); ) {
        if (GetTCInventoryAbilities()->IsAbilityPotentiallyAvailable(it->first) &&
            GetTCInventoryAbilities()->GetAbilityLevel(it->first) < GetTCInventoryAbilities()->GetAbilityMaximumLevel(it->first) &&
            GetTCInventoryAbilities()->GetExperienceCostToUpgradeAbility(it->first, 1, true) >
                GetTCHeroExperience()->GetStatExperience(GetTCInventoryAbilities()->GetAbilityStatType(it->first)) + general) {
            CDeletion deletion;
            deletion.Method = CDeletion::UI_DELETE_THIS_INSTANCE;
            it->second->SetDeletion(deletion);
            std::map<EHeroAbility, CComponent*>::iterator next = it;
            ++next;
            UpgradableAbilities.erase(it);
            it = next;
        } else
            ++it;
    }
    CComponent* screen = Components[1];
    static_cast<CText*>(screen->FindChild("UI_TEXT_EXPERIENCE_GENERAL_VALUE"))->SetValue(GFIntToWideString(GetTCHeroExperience()->GetExperiencePointsAvailableToSpend()), true, true);
    static_cast<CText*>(screen->FindChild("UI_TEXT_EXPERIENCE_STRENGTH_VALUE"))->SetValue(GFIntToWideString(GetTCHeroExperience()->GetStatExperience(HERO_STAT_STRENGTH)), true, true);
    static_cast<CText*>(screen->FindChild("UI_TEXT_EXPERIENCE_DEXTERITY_VALUE"))->SetValue(GFIntToWideString(GetTCHeroExperience()->GetStatExperience(HERO_STAT_SKILL)), true, true);
    static_cast<CText*>(screen->FindChild("UI_TEXT_EXPERIENCE_WILL_VALUE"))->SetValue(GFIntToWideString(GetTCHeroExperience()->GetStatExperience(HERO_STAT_WILL)), true, true);
    if (UpgradableStats.size() == 0 && UpgradableAbilities.size() == 0)
        AddCloseInventoryEvent();
}