// 00930480 CAIStateGroup_MinionReactToPlayerInteraction::Update
// Leaf: ?Update@CAIStateGroup_MinionReactToPlayerInteraction@@UAEXXZ
// Flags: none
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
#define FABLE_RETAIL_MINION_INTERACTION_UPDATE_PASS29
#define FABLE_RETAIL_CAISTATEGROUPBASE_PEEK
#define FABLE_RETAIL_CAISTATEGROUPBASE_PEEK_PROTECTED
#define FABLE_RETAIL_CAISTATEGROUPBASE_PEEKPLAYERMANAGER
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CSUBTITLERENDERER
#define FABLE_RETAIL_CSUBTITLERENDERER_SETTEXT
#define FABLE_RETAIL_CSUBTITLERENDERER_SETTEXT_CHAR
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_TEXTBANK
#define FABLE_RETAIL_CWORLD_QUESTCOMPLETIONUI
#define FABLE_RETAIL_CQUESTMANAGER
#define FABLE_RETAIL_CTCBOASTUI
#define FABLE_RETAIL_CTCBOASTUI_ACTIVE_PASS26
#define FABLE_RETAIL_CAISTATEGROUPBASE_MOVETOPOSPROXIMITY
#define FABLE_RETAIL_CTCTALK
#define FABLE_RETAIL_CTCCREATURENAVIGATION
#define FABLE_RETAIL_CTCCREATURENAVIGATION_NAVIGABLE
#define FABLE_RETAIL_CTHINGCREATUREBASE_FREETOFINISH
#define FABLE_RETAIL_CTHINGCREATUREBASE_FINISHACTION
#define FABLE_RETAIL_CTHINGCREATUREBASE_PERSONALSPACE_PASS24
#define FABLE_RETAIL_CTCCREATUREOPINIONOFHERO
#define FABLE_RETAIL_CTCCREATUREOPINIONOFHERO_GREET
#define FABLE_RETAIL_C3DVECTOR_OPERATORS
#define FABLE_RETAIL_MINION_SINGLE_TARGET_PREDICATE_PASS29
#define FABLE_RETAIL_CMESSAGEEVENTMANAGER_ORIGINAL_LOCAL
#define FABLE_RETAIL_CMESSAGEEVENTFILTERS_PAYMENT
#define FABLE_RETAIL_MINION_GOSSIP_PREDICATES_PASS29
#define FABLE_RETAIL_CWORLD_MESSAGEEVENTMANAGER_CONST
#include "engine/EAIAction.h"
#include "fable_string.h"
#include "engine/CPlayer.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CThingAICreature.h"
#include "engine/CMessageEvent.h"
#include "engine/CMessageEventManager.h"
#include "engine/CMessageEventFilters.h"
#define FABLE_RETAIL_MINION_CONFIRMATION_PASS29
#include "engine/CAIStateGroup_MinionReactToPlayerInteraction.h"
#include "engine/CAIBrain.h"
#include "engine/CWorld.h"
#include "engine/CPlayerManager.h"
#include "engine/CTCWife.h"
#include "engine/CTCShopKeeper.h"
#include "engine/CTCSoundPlayer.h"
#include "engine/CTCHeroStats.h"
#include "engine/CTCHeroOnlineScoreboard.h"
#include "engine/CTCVillage.h"
#include "engine/CTCVillageMember.h"
#include "engine/ThingAlive.h"
#include "engine/NCreatureAI.h"

#include "engine/CMainGameComponent.h"
#include "engine/CSubtitleRenderer.h"
#include "engine/CPlayerGui.h"
#include "engine/CTCBoastUI.h"
#include "engine/CTCCreatureNavigation.h"
#include "engine/CTCTalk.h"
#include "engine/CTCCreatureOpinionOfHero.h"
#include "engine/EMovementType.h"

#include "engine/CFilter_Bind2.h"
inline const CMessageEvent* CAIStateGroup_MinionReactToPlayerInteraction::FindPlayerInteractionEvent() const
{
    return PeekWorld().GetMessageEventManager().FindMostRecentMessage(
        GFFilter_Bind2(CMessageEventFilter_IsOfType(MESSAGE_EVENT_PLAYER_INTERACTION),
                       CMessageEventFilter_IsTargettedAtThing(GetCreature())));
}
void CAIStateGroup_MinionReactToPlayerInteraction::Update()
{
    const CMessageEvent* pevent = FindPlayerInteractionEvent();
    if (pevent) {
        long event_extra_data = pevent->ExtraData;
        GetWorld().GetMessageEventManager().RemoveMessage(pevent);
        CIntelligentPointer<CThingPlayerCreature> player_character(PeekPlayerManager().GetMainPlayer().GetPPlayerCharacter());
        if (!IS_THING_ALIVE(player_character)) {
            Terminate();
            return;
        }
        if (ReactionType == INTERACTION_REACTION_TRADER && !GetWorld().IsPerformingRegionLoad() && !CPlayerGui::Get().GetSubtitleRenderer().IsDisplayingText()) {
            const CTCBoastUI* ptc_boast_ui;
            if (!player_character->PeekTC(&ptc_boast_ui) || !ptc_boast_ui->IsActive()) {
                CPlayerGui::Get().GetSubtitleRenderer().SetText(CMainGameComponent::Get()->PeekCutSceneMainFont(),"TEXT_GUI_TRADE_START_TRADING",4,0.0f,false,true);
                WaitingForConfirmation=true;
                PositionPlayerWasWhenWaitingForConfirmation=player_character->GetPos();
                UpdateWaitingForConfirmation(player_character);
            }
        }
        bool too_far=IsTooFarFromThePlayer(*player_character);
        bool too_near=IsTooNearToThePlayer(*player_character);
        if(GetCreature().HasTC(TCI_SHOP_KEEPER)) {
            CTCCreatureNavigation* tc_navigation;
            if(GetCreature().GetTC(&tc_navigation)) {
                float left,right;
                tc_navigation->CalculateNavigableWidthOfLine(GetCreature().GetPos(),player_character->GetPos(),left,right);
                if(left==0.0f || right==0.0f) { too_near=false; too_far=false; }
            }
        }
        if(GetCreature().IsFreeToFinishAction()) GetCreature().FinishCurrentAction();
        if(too_far || too_near) {
            C3DVector away_dir=GetCreature().PeekPos()-player_character->PeekPos();
            away_dir.Normalise();
            long time_to_give_up=CWorld::GetFrame()+CMainGameComponent::GetConstantFPS()*10;
            bool calculated_nav_route;
            EAIActionReturn ret;
            do {
                C3DVector move_to_pos=player_character->PeekPos()+away_dir*CThingCreatureBase::GetPersonalSpaceRadius();
                ret=AIAction_MoveToPosProximity(ACTION_REPEAT_CALLBACK,move_to_pos,0.1f,calculated_nav_route,AI_MOVEMENT_WALKING,too_far,false,true,player_character.Get(),true,0.0f);
            } while(CWorld::GetFrame()<time_to_give_up && ret==AI_ACTION_IN_PROGRESS);
            if(ret!=AI_ACTION_FINISHED && event_extra_data==5) { Terminate();return; }
            long frame_started=CWorld::GetFrame();
            const long SECONDS_TO_WAIT_FOR_GIFT=5;
            do {
                if(AIAction_LookAtThing(ACTION_REPEAT_UNTIL_FINISHED,player_character,11,0.0027777f)!=AI_ACTION_FINISHED) {
                    Terminate();
                    return;
                }
                if(event_extra_data!=5)break;
            } while(CWorld::GetFrame()-frame_started<CMainGameComponent::GetConstantFPS()*SECONDS_TO_WAIT_FOR_GIFT);
        }
        CTCTalk* tc_talk;
        GetCreature().GetTC(&tc_talk);
        while(tc_talk->IsTalking()) {
            if(AIAction_DoNothing()!=AI_ACTION_IN_PROGRESS)break;
        }
        if(GetCreature().HasTC(TCI_WIFE)) {
            CTCWife* ptc_wife=0;
            GetCreature().GetTC(&ptc_wife);
            ptc_wife->OnGreet();
        }
        if(TestOrDoInteraction(false,player_character.Get()) || DidTraderInteraction) {
            CTCCreatureOpinionOfHero* ptc_opinion;
            if(GetCreature().GetTC(&ptc_opinion))ptc_opinion->SetHasInteracted();
        }
        if(WaitingForConfirmation)UpdateWaitingForConfirmation(player_character);
    }
    Terminate();
}