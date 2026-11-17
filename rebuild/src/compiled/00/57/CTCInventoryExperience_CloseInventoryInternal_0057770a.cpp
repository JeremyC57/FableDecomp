// CTCInventoryExperience::CloseInventoryInternal(bool ForGood) -- vtable slot 89, the console twin of slot 90 (0x005778CA): when ForGood, give back every undoable stat and ability purchase (drop the bought levels, re-upgrade them so the derived values rebuild, refund the spent experience into the hero stats' totals), clear both undo maps and mark the spending screen as opened; then restore the previous hero screen, close the base inventory, delete every UI component and reset the UI manager
// Leaf: ?CloseInventoryInternal@CTCInventoryExperience@@UAEX_N@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0057770A, 445 bytes, `ret 4`, no frame pointer (size TU: `pop ecx` after the cdecl tree
//   increment).
// Identity (two sources): Ego_r.pdb 0x005ECF86 + FableWin.pdb 0x023ABEF2 ?CloseInventoryInternal@CTCInventoryExperience@@UAEX_N@Z, the donor
//   right before CloseInventoryInternalPC (Ego_r 0x005ED15B = retail 0x005778CA); engine/CTCInventoryExperience.h VIRTUALS maps slot 89 to
//   0x0057770A (CloseInventory 0x0056F30B calls slot 89).
// NF79 port (round 7, q10970; also the q10959 / q10960 binder): NF76's 0x005778CA recipe (same macro block, with the vector range erase
//   0x00485AA7 out of line instead of the single erase); the undo part is the PC twin's text; then, in place of the PC component walk, a
//   `count` loop deleting every component and Components.clear() (erase(begin, end) 0x00485AA7), the UI manager's ClearObservers /
//   ClearActionParams, NUISystem::CList::GPreviousSelected.clear() and ClearBastardChild. Locals as Ego_d.pdb names them (ForGood, it, it1,
//   total, i, count). Was the placeholder vfunc_89 over local models (undo maps, CTCHeroStats / CTCHeroExperience / CTCHeroAbilities,
//   NUISystem::CManager slots ResetA / ResetB / ResetC, g_UIList, CHeroInfoScreens::SetScreenType, CPlayerGui::GetHeroInfoScreens,
//   OnInventoryClosed).
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CMANAGER
#define FABLE_RETAIL_CLIST
#define FABLE_RETAIL_CTCHEROEXPERIENCE
#define FABLE_RETAIL_CTCHEROEXPERIENCE_UPGRADESTAT
#define FABLE_RETAIL_CTCHEROEXPERIENCE_MODIFYLEVEL
#define FABLE_RETAIL_CTCHEROSTATS
#define FABLE_RETAIL_CTCHEROSTATS_SERIALISE
#define FABLE_RETAIL_CTCHEROSTATS_SPENTEXPERIENCE
#define FABLE_RETAIL_EHEROTRAINABLESTATTYPE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_VTABLE
#define FABLE_RETAIL_CTCINVENTORYEXPERIENCE
#define FABLE_RETAIL_CTCINVENTORYEXPERIENCE_VIRTUALS
#define FABLE_RETAIL_CTCINVENTORYABILITIES
#define FABLE_RETAIL_CTCINVENTORYABILITIES_SETLEVEL
#define FABLE_RETAIL_CTCINVENTORYABILITIES_UPGRADEABILITY
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_NHEROINFORMATIONSCREENS_CMANAGER
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_TREE_ERASE
#define FABLE_RETAIL_STLP_TREE_CLEAR_OUTOFLINE
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_LIST_CLEAR_OUTOFLINE
#define FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_OUTOFLINE
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CTCHeroExperience.h"
#include "engine/CTCHeroStats.h"
#include "engine/CTCInventoryExperience.h"
#include "engine/CTCInventoryAbilities.h"
#include "engine/CPlayerGui.h"
#include "engine/NHeroInformationScreens_CManager.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CList.h"

void CTCInventoryExperience::CloseInventoryInternal(bool ForGood)
{
    if (ForGood) {
        std::map<EHeroTrainableStatType, SExpSpendUndo>::iterator it;
        std::map<EHeroAbility, SExpSpendUndo>::iterator it1;
        for (it = UndoableStatBuys.begin(); it != UndoableStatBuys.end(); ++it) {
            GetTCHeroExperience()->ModifyTrainableStatLevel((*it).first, -(*it).second.Levels);
            for (int i = 0; i != (*it).second.Levels; ++i)
                GetTCHeroExperience()->UpgradeTrainableStat((*it).first);
            long total = (*it).second.Will + (*it).second.Skill + (*it).second.Strength + (*it).second.General;
            GetTCHeroStats()->AddSpentExperience(total);
            GetTCHeroStats()->GetExperienceSpentOn()[(*it).first] += total;
        }
        for (it1 = UndoableAbilityBuys.begin(); it1 != UndoableAbilityBuys.end(); ++it1) {
            GetTCInventoryAbilities()->SetAbilityLevelTo((*it1).first, GetTCInventoryAbilities()->GetAbilityLevel((*it1).first) - (*it1).second.Levels);
            for (int i = 0; i != (*it1).second.Levels; ++i)
                GetTCInventoryAbilities()->UpgradeAbility((*it1).first);
            long total = (*it1).second.Will + (*it1).second.Skill + (*it1).second.Strength + (*it1).second.General;
            GetTCHeroStats()->AddSpentExperience(total);
            GetTCHeroStats()->GetExperienceSpentOn()[10] += total;
        }
        UndoableStatBuys.clear();
        UndoableAbilityBuys.clear();
        OpenedExperienceSpending = true;
    }
    CPlayerGui::Get().GetPHeroInfoScreensManager()->SelectScreenOfType(PreviousScreenType);
    NInventory::CTCInventoryBase::CloseInventory();
    {
        for (unsigned long count = 0; count < Components.size(); ++count)
            delete Components[count];
    }
    Components.clear();
    NUISystem::CManager::GetInstance()->ClearObservers();
    NUISystem::CManager::GetInstance()->ClearActionParams();
    NUISystem::CList::GPreviousSelected.clear();
    NUISystem::CManager::GetInstance()->ClearBastardChild();
}
