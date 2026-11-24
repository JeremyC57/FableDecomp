// CQuestManager::SetQuestAsCompleted -- finish a registered quest and award its quest card.
// Leaf: ?SetQuestAsCompleted@CQuestManager@@QAE_NABVCCharString@@_N11@Z
// Flags: none
#define FABLE_RETAIL_ASSERT_DEAD_ARM
#define FABLE_RETAIL_CQUESTMANAGER_COMPLETE_PASS26
#define FABLE_RETAIL_CQUESTMANAGER_GETACTIVEQUESTFROMSCRIPTNAME
#define FABLE_RETAIL_CTCHEROSTATS_QUEST_COMPLETION_PASS26
#define FABLE_RETAIL_CTCHEROSTATS_ADDRENOWN
#define FABLE_RETAIL_CTCQUESTCARD_ACCESSORS
#define FABLE_RETAIL_CTCQUESTCARD_ISFEAT_PASS26
#define FABLE_RETAIL_CTCQUESTCOMPLETIONUI_GLOBALUPDATE
#define FABLE_RETAIL_CWORLD_QUEST_COMPLETION_SAVE_PASS26
#define FABLE_RETAIL_CTHINGOBJECT_CREATE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CTOR_OUTOFLINE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_ORIGINAL_COMPLETE_METHODS_PASS22
#define FABLE_RETAIL_CTCINVENTORY
#define FABLE_RETAIL_CTCINVENTORY_ORIGINAL_COMPLETE_CLASS_PASS22
#define FABLE_RETAIL_CTCINVENTORYWEAPONS
#define FABLE_RETAIL_CTCINVENTORYWEAPONS_ACTIVEMELEE
#define FABLE_RETAIL_CTCINVENTORYCLOTHING_CALCULATE
#define FABLE_RETAIL_CTCINVENTORYCLOTHING_STAT_INTERFACE_PASS26
#define FABLE_RETAIL_CTCINVENTORYITEM
#define FABLE_RETAIL_CTCINVENTORYITEM_INVENTORYACCESS
#define FABLE_RETAIL_CTCINVENTORYITEM_ADDTOINVENTORY
#define FABLE_RETAIL_CTCINVENTORYITEM_STATGETINTERFACETYPE
#define FABLE_RETAIL_CDEFPOINTER_RELEASE_SELFDELETING_POINTEE
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_ORIGINAL_SCOPES_PASS22
#define FABLE_RETAIL_STLP_LIST_PUSHBACK_DECL_PASS26
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_CQUESTMANAGER_FAILUREBOOKKEEP
#define FABLE_RETAIL_CQUESTMANAGER_FAILQUEST
#define FABLE_RETAIL_CQUESTMANAGER_COMPLETIONUI
#define FABLE_RETAIL_CTCQUESTCARD
#define FABLE_RETAIL_CTCQUESTCARD_STATGETINTERFACETYPE
#define FABLE_RETAIL_CTCQUESTCARD_FAILURE
#define FABLE_RETAIL_CTCHEROSTATS
#define FABLE_RETAIL_CTCHEROSTATS_SERIALISE
#define FABLE_RETAIL_CTCHEROSTATS_FAILEDQUEST
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_STLP_LIST_REMOVE_IF_OUTOFLINE
#define FABLE_RETAIL_CQUESTMANAGER
#define FABLE_RETAIL_CQUESTMANAGER_ACTIVEQUEST
#define FABLE_RETAIL_CQUESTMANAGER_BOASTMADE
#define FABLE_RETAIL_CQUESTMANAGER_POSTBOAST
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_PLAYERMANAGER
#define FABLE_RETAIL_CMAINGAMECOMPONENT_GETPLAYERMANAGER
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_MESSAGEEVENTMANAGER
#define FABLE_RETAIL_STLP_LIST
#include "fable_assert.h"
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
#include "engine/CQuestManager.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CTCQuestCompletionUI.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CPlayer.h"
#include "engine/CPlayerManager.h"
#include "engine/CMainGameComponent.h"
#include "engine/CMessageEventManager.h"
#define FABLE_RETAIL_IS_THING_ALIVE
#include "engine/ThingAlive.h"
#include "engine/CWorld.h"

#include "engine/CTCQuestCard.h"
#include "engine/CTCHeroStats.h"
#include "engine/CDefString.h"
#include "engine/CIsQuestCardWithDefName.h"

#include "engine/CThingObject.h"
#include "engine/CTCInventory.h"
#include "engine/CTCInventoryWeapons.h"
#include "engine/CTCInventoryClothing.h"
#include "engine/CTCInventoryItem.h"

void CQuestManager::RemoveAvailableQuestCardType(const CCharString& def_name)
{
    AvailableQuestCardTypes.remove_if(CIsQuestCardWithDefName(def_name));
    SetQuestCardsNeedUpdating(true);
}

bool CQuestManager::SetQuestAsCompleted(const CCharString& script_name, bool display_screen,
                                       bool auto_save, bool force_regular_auto_save)
{
    long index = GetIndexFromRegisteredQuest(script_name);
    if (index > -1) {
        CTCQuestCard* ptc_quest_card = 0;
        World.GetMessageEventManager().AddMessage(MESSAGE_EVENT_QUEST_COMPLETED_BEFORE_SCREEN_SHOWN,
                                                  50, C3DVector(0, 0, 0), 0, 0, 0, 0, index);
        if (!display_screen)
            World.GetMessageEventManager().AddMessage(MESSAGE_EVENT_QUEST_COMPLETED,
                                                      50, C3DVector(0, 0, 0), 0, 0, 0, 0, index);
        CThingPlayerCreature* phero =
            CMainGameComponent::Get()->GetPlayerManager().GetMainPlayer().GetPControlledCreature();
        if (IS_THING_ALIVE(phero)) {
            if (display_screen) {
                CTCQuestCompletionUI* ptc_ui;
                if (!phero->GetTC(&ptc_ui))
                    FABLE_RETAIL_ASSERT_ARM(false);
                ptc_ui->Activate(CTCQuestCompletionUI::QUEST_SUCCESS, script_name, false, L"", true);
            } else {
                OnQuestFinishScreenDisplayed(script_name);
            }

            CThing* pquest_card = GetActiveQuestCardFromScriptName(script_name);
            if (pquest_card) {
                if (!pquest_card->GetTC(&ptc_quest_card))
                    FABLE_RETAIL_ASSERT_ARM(false);
                if (ptc_quest_card) {
                    ptc_quest_card->SetAsFinished(true);
                    CompletedQuestCards.push_back(pquest_card);
                    CTCHeroStats* ptc_hero_stats = 0;
                    if (!phero->GetTC(&ptc_hero_stats))
                        FABLE_RETAIL_ASSERT_ARM(false);
                    ptc_hero_stats->SetNumCompletedAQuest(ptc_hero_stats->GetNumCompletedAQuest() + 1);
                    if (ptc_quest_card->IsCore())
                        ptc_hero_stats->SetNumCompletedACoreQuest(ptc_hero_stats->GetNumCompletedACoreQuest() + 1);
                    else if (ptc_quest_card->IsFeat())
                        ptc_hero_stats->SetNumCompletedAFeatQuest(ptc_hero_stats->GetNumCompletedAFeatQuest() + 1);
                    else
                        ptc_hero_stats->SetNumCompletedAnOptionalQuest(ptc_hero_stats->GetNumCompletedAnOptionalQuest() + 1);

                    if (!ptc_quest_card->GetPDef().IsNull()) {
                        ptc_hero_stats->AddMoney(ptc_quest_card->GetGoldReward(), true);
                        ptc_hero_stats->AddRewardMoney(ptc_quest_card->GetGoldReward());
                        ptc_hero_stats->AddRenown(ptc_quest_card->GetRenownReward());
                        for (unsigned long count = 0; count < ptc_quest_card->GetPDef()->RewardObjects.size(); count++) {
                            CThing* pobject = CThingObject::Create(ptc_quest_card->GetPDef()->RewardObjects[count],
                                                                  phero->GetPos(), 4, 0, 0, "");
                            if (pobject) {
                                CTCInventoryItem* ptc_item = 0;
                                if (!pobject->GetTC(&ptc_item))
                                    FABLE_RETAIL_ASSERT_ARM(false);
                                if (ptc_item) {
                                    NInventory::CTCInventoryBase* ptc_inventory;
                                    if (!phero->GetTC(ptc_item->GetInventoryType(), &ptc_inventory))
                                        FABLE_RETAIL_ASSERT_ARM(false);
                                    if (ptc_inventory->IsSpaceInInventoryForItem(pobject->GetDefGlobalIndex()))
                                        ptc_item->AddToInventory(*ptc_inventory, false, true, 0, true, false);
                                }
                            }
                        }
                    }
                }

                RemoveAvailableQuestCardType(pquest_card->GetDefName());
                RemoveActiveQuestCard(script_name);
            }

            CTCInventory* ptc_inventory;
            if (phero->GetTC(&ptc_inventory))
                ptc_inventory->SetQuestPersistFlagForAllItems(true);
            CTCInventoryWeapons* ptc_inventory_weapons;
            if (phero->GetTC(&ptc_inventory_weapons))
                ptc_inventory_weapons->SetQuestPersistFlagForAllItems(true);
            CTCInventoryClothing* ptc_inventory_clothing;
            if (phero->GetTC(&ptc_inventory_clothing))
                ptc_inventory_clothing->SetQuestPersistFlagForAllItems(true);
        }

        CActiveQuest* pquest = GetActiveQuestFromScriptName(script_name);
        if (pquest)
            pquest->SetPersistent(false);
        if (auto_save && ptc_quest_card && ptc_quest_card->IsCore())
            World.AutoSave(force_regular_auto_save);
        CWorld::DeleteCheckPointSave();
        World.SetAutoSaveCheckPointExists(false);
        return true;
    }
    return false;
}