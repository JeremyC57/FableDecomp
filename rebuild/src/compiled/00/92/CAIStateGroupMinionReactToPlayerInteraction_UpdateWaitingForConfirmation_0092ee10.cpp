// 0092ee10 CAIStateGroup_MinionReactToPlayerInteraction::UpdateWaitingForConfirmation
// Leaf: ?UpdateWaitingForConfirmation@CAIStateGroup_MinionReactToPlayerInteraction@@QAEXAAV?$CIntelligentPointer@VCThingPlayerCreature@@@@@Z
// Flags: none
#define FABLE_RETAIL_MINION_CONFIRMATION_PASS29
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_GUARD_REACT_TO_BRIBE_PASS27
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CONVERT_MUTABLE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_SPEECH
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUP_MINIONREACTTOPLAYERINTERACTION_VIRTUAL_MODEL
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_MESSAGEEVENTMANAGER
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_CMESSAGEEVENT
#define FABLE_RETAIL_CMESSAGEEVENTMANAGER_REMOVE
#define FABLE_RETAIL_CMESSAGEEVENTMANAGER_FINDRECENT
#define FABLE_RETAIL_CMESSAGEEVENTFILTERS
#define FABLE_RETAIL_CMESSAGEEVENTFILTERS_ISOFTYPE_CTOR
#define FABLE_RETAIL_CTCWIFE
#define FABLE_RETAIL_CTCSHOPKEEPER_SHOP
#define FABLE_RETAIL_CTCSOUNDPLAYER
#define FABLE_RETAIL_CTCSOUNDPLAYER_ATTACHEDDIALOGUE
#define FABLE_RETAIL_CTCVILLAGE
#define FABLE_RETAIL_CTCVILLAGE_BRIBE
#define FABLE_RETAIL_CTCVILLAGE_YELLFORHELP
#define FABLE_RETAIL_CTCHEROSTATS
#define FABLE_RETAIL_CTCHEROSTATS_BRIBE
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CTCHEROSTATS_ADDMONEY
#include "engine/EAIAction.h"
#include "fable_string.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CThingAICreature.h"
#include "engine/CMessageEvent.h"
#include "engine/CMessageEventManager.h"
#include "engine/CMessageEventFilters.h"
#include "engine/CAIStateGroup_MinionReactToPlayerInteraction.h"
#include "engine/CAIBrain.h"
#include "engine/CWorld.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"
#include "engine/CTCWife.h"
#include "engine/CTCShopKeeper.h"
#include "engine/CTCSoundPlayer.h"
#include "engine/CTCHeroStats.h"
#include "engine/CTCHeroOnlineScoreboard.h"
#include "engine/CTCVillage.h"
#include "engine/CTCVillageMember.h"
#include "engine/ThingAlive.h"
#include "engine/NCreatureAI.h"

void CAIStateGroup_MinionReactToPlayerInteraction::UpdateWaitingForConfirmation(CIntelligentPointer<CThingPlayerCreature>& player)
{
    EAIActionReturn ret;
    do {
        if (!IS_THING_ALIVE(player) || !WaitingForConfirmation)
            return;
        CMessageEventManager& manager = GetWorld().GetMessageEventManager();
        const CMessageEvent* pevent = manager.FindMostRecentMessage(CMessageEventFilter_IsOfType(MESSAGE_EVENT_CONVERSATION_ANSWER_YES_OR_NO));
        if (pevent) {
            switch (ReactionType) {
            case INTERACTION_REACTION_WIFE: {
                CTCWife* ptc_wife;
                if (GetCreature().GetTC(&ptc_wife))
                    ptc_wife->ReceivedConfirmation(pevent->ExtraData == 1);
                break;
            }
            case INTERACTION_REACTION_TRADER: {
                ReactionType = INTERACTION_REACTION_GOSSIP;
                if (pevent->ExtraData == 1) {
                    CTCShopKeeper* ptc_shopkeeper;
                    if (GetCreature().GetTC(&ptc_shopkeeper)) {
                        CCharString group_name = ptc_shopkeeper->GetFriendlynessIndicator();
                        CTCSoundPlayer* ptc_sound_player;
                        GetCreature().GetTC(&ptc_sound_player);
                        ptc_sound_player->PlayAttachedDialogue(group_name, false, false, true, false);
                        ptc_shopkeeper->OpenShoppingInterface();
                        DidTraderInteraction = true;
                        if (AIAction_DoNothing() != AI_ACTION_IN_PROGRESS) {
                            Terminate();
                            return;
                        }
                    }
                }
                break;
            }
            case INTERACTION_REACTION_BRIBE: {
                if (pevent->ExtraData == 1) {
                    CTCVillageMember* tc_village_member;
                    if (GetCreature().GetTC(&tc_village_member) && tc_village_member->GetPVillage()) {
                        CThingPlayerCreature* phero = GetPlayerManager().GetMainPlayer().GetPControlledCreature();
                        CTCVillage* village = tc_village_member->GetPVillage();
                        CTCHeroStats* ptc_hero_stats;
                        if (phero->GetTC(&ptc_hero_stats)) {
                            ptc_hero_stats->AddMoney(-village->GetCostToBribe(), true);
                            ptc_hero_stats->SetTotalBribeAmount(ptc_hero_stats->GetTotalBribeAmount() + village->GetCostToBribe());
                            CTCHeroOnlineScoreboard* ptc_hero_scoreboard;
                            GetPlayerManager().GetMainPlayer().GetPControlledCreature()->GetTC(&ptc_hero_scoreboard);
                            ptc_hero_scoreboard->UpdateScoreForActivity(CTCHeroOnlineScoreboard::OSA_BRIBES,(float)ptc_hero_stats->GetTotalBribeAmount(),false);
                        }
                        tc_village_member->GetPVillage()->SetFrameBribeEnds(GetWorld().GetFrame()+450);
                    }
                }
                break;
            }
            }
            manager.RemoveMessage(pevent);
            WaitingForConfirmation = false;
        }
        if (NCreatureAI::IsThingRunningAwayFromThing(*player,GetCreature(),PositionPlayerWasWhenWaitingForConfirmation))
            return;
        ret = AIAction_LookAtThing(ACTION_REPEAT_CALLBACK,player,19,0.0027777f);
    } while (ret != AI_ACTION_TERMINATE_PROCESS);
}