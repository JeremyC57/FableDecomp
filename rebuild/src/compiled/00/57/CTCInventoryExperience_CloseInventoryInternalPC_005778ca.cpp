// CTCInventoryExperience::CloseInventoryInternalPC(bool) -- vtable slot 90, the PC twin of slot 89 (0x0057770A): when ForGood, give back every undoable stat and ability purchase (drop the bought levels, re-upgrade them so the derived values rebuild, refund the spent experience into the hero stats' totals), clear both undo maps and mark the spending screen as opened; then restore the previous hero screen, close the base inventory, delete every UI component (keeping the mouse pointer unless ForGood), reset the UI manager and, unless ForGood, hand the mouse pointer back to it
// Leaf: ?CloseInventoryInternalPC@CTCInventoryExperience@@UAEX_N@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005778CA, `ret 4`, no frame pointer (size TU).
// NF76 port (2026-11-17): self is Ego_r.pdb 0x005ED15B ?CloseInventoryInternalPC@CTCInventoryExperience@@UAEX_N@Z (engine/CTCInventoryExperience.h
//   VIRTUALS maps slot 90 to 0x005778CA; the landed self was the placeholder vfunc_90). On the shared headers with the sibling
//   UndoBuyAbility 0x00576C4F and CTCInventoryTrade::CloseInventoryPC 0x0059E2C7 macro blocks: the undo maps, the typed Components
//   vector (erase out of line 0x00485468), NInventory::CTCInventoryBase::CloseInventory 0x005BD597 (queue nf44:54), CTCInventoryAbilities
//   GetAbilityLevel (nf44:7) / SetAbilityLevelTo, CTCHeroExperience ModifyTrainableStatLevel / UpgradeTrainableStat,
//   CPlayerGui::GetPHeroInfoScreensManager 0x00437CD0 -> NHeroInformationScreens::CManager::SelectScreenOfType 0x0066E799, the UI manager's
//   CObservable / CManager virtuals (retail vtable 0x01230134: slot 6 ClearObservers, 29 ClearActionParams, 37 ClearBastardChild, 2
//   AddObserver; the landed ResetA / ResetB / ResetC / SetMousePointer), NUISystem::CList::GPreviousSelected.clear() (0x004855A3 on
//   0x013B8A98, the 0x00625A20 / 0x0059E2C7 idiom; was g_UIList.clear()) and the pointer's CObserver::ObserveEvent(UI_EVENT_MOUSE_MOVEMENT = 0x19) (slot 3 of the
//   +4 base); my overlay opt-ins CTCHEROSTATS_SPENTEXPERIENCE (Ego_r / FableWin inline AddSpentExperience / GetExperienceSpentOn) and
//   CTCINVENTORYABILITIES_UPGRADEABILITY. Locals as Ego_d.pdb names them (ForGood, it, it1, total, i, the component it).
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
#define FABLE_RETAIL_STLP_VECTOR_ERASE_OUTOFLINE
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

void CTCInventoryExperience::CloseInventoryInternalPC(bool ForGood)
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
        std::vector<NUISystem::CComponent*>::iterator it = Components.begin();
        while (it != Components.end()) {
            if (!ForGood && *it == PMousePointer) {
                ++it;
                continue;
            }
            delete *it;
            it = Components.erase(it);
        }
    }
    NUISystem::CManager::GetInstance()->ClearObservers();
    NUISystem::CManager::GetInstance()->ClearActionParams();
    NUISystem::CList::GPreviousSelected.clear();
    NUISystem::CManager::GetInstance()->ClearBastardChild();
    if (!ForGood) {
        NUISystem::CManager::GetInstance()->AddObserver(PMousePointer);
        PMousePointer->ObserveEvent(NUISystem::UI_EVENT_MOUSE_MOVEMENT);
    }
}
