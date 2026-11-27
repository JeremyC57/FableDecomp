// 007ec710 CTCActionUseBed::GlobalUpdate
// Leaf: ?GlobalUpdate@CTCActionUseBed@@UAEX_N@Z
// Flags: /GS
#define FABLE_RETAIL_BED_GLOBAL_UPDATE_PASS29
#define FABLE_RETAIL_CTHING_ORIGINAL_VTABLE
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CTHINGPHYSICAL_MAXHEALTH
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_CMAINGAMECOMPONENT_DISPLAYENGINE
#define FABLE_RETAIL_CMAINGAMECOMPONENT_PLAYERMANAGER
#define FABLE_RETAIL_CDISPLAYENGINE
#define FABLE_RETAIL_CDISPLAYENGINE_STARTSCREENFADEOUT
#define FABLE_RETAIL_CDISPLAYENGINE_ENDSCREENFADEOUT
#define FABLE_RETAIL_CDISPLAYENGINE_SCREENFADEOUT
#define FABLE_RETAIL_CDISPLAYENGINE_ISSCREENFULLYFADEDOUT
#define FABLE_RETAIL_CFADEINFADEOUTBASE_ISOPENING_PASS26
#define FABLE_RETAIL_CFADEINFADEOUTBASE_ISCLOSING
#define FABLE_RETAIL_CFADEINFADEOUTBASE_FADE
#define FABLE_RETAIL_CMESSAGEEVENT
#define FABLE_RETAIL_CMESSAGEEVENTFILTERS
#define FABLE_RETAIL_CMESSAGEEVENTMANAGER_REMOVE
#define FABLE_RETAIL_CWORLD_MESSAGEEVENTMANAGER
#define FABLE_RETAIL_CMESSAGEEVENTMANAGER_FINDRECENT_DECL
#define FABLE_RETAIL_CMESSAGEEVENTFILTERS_ISOFTYPE_CTOR
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_CGAMETIMEMANAGER_FULL
#define FABLE_RETAIL_CMESSAGEEVENTMANAGER
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_LEAFBODY
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_LEAFBODY_CASTARG
#define FABLE_RETAIL_CTHINGVILLAGE_STATGETTYPE
#define FABLE_RETAIL_CTHING_SCANINCLUSION_ISALIVE
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CTCTRESPASSER
#define FABLE_RETAIL_CTCBULLETTIME
#define FABLE_RETAIL_CTCHEROEXPERIENCE
#define FABLE_RETAIL_CTCSPECIALABILITIES_FULL
#define FABLE_RETAIL_CTCHEROSTATS
#define FABLE_RETAIL_CTCHEROSTATS_STAMINA
#define FABLE_RETAIL_CTCHEROSTATS_ADDMONEY
#define FABLE_RETAIL_CTCBASE_REMOVEFROMGLOBALTCS
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_IS_THING_ALIVE
// 007ec710 CTCActionUseBed::GlobalUpdate
#define FABLE_RETAIL_BED_USE_PASS28
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGBUILDING
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCACTIONUSEBASE_VSLOTS
#define FABLE_RETAIL_CTCVILLAGE
#define FABLE_RETAIL_CTCVILLAGEMEMBER
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_ADDGAMEEVENT
#define FABLE_RETAIL_CGAMEEVENT_INLINE
#include "rebuild_abi.h"
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CCountedPointer.h"
#include "engine/CPlayer.h"
#include "engine/CThingPhysical.h"
#include "engine/CThingGameObject.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingAICreature.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CTCActionUseBed.h"
#include "engine/CTCVillage.h"
#include "engine/CTCVillageMember.h"
#include "engine/CPlayerManager.h"
#include "engine/CWorld.h"
#include "engine/CMainGameComponent.h"
#include "engine/CGameEvent.h"

#include "engine/ThingAlive.h"
#include "engine/CTCBulletTime.h"
#include "engine/CTCHeroExperience.h"
#include "engine/CTCSpecialAbilities.h"
#include "engine/CTCHeroStats.h"
#include "engine/CTCTrespasser.h"
#include "engine/CDisplayEngine.h"
#include "engine/CRGBColour.h"
#include "engine/EMessageEventType.h"
#include "engine/CMessageEvent.h"
#include "engine/CMessageEventManager.h"
#include "engine/CMessageEventFilters.h"
#include "engine/CGameTimeManager.h"
#include "engine/CThingVillage.h"
#include "engine/CThingFilter_None.h"
#include "engine/CThingSearchTools.h"
void CTCActionUseBed::GlobalUpdate(bool paused)
{
    if (WaitingForConfirmation) {
        CMessageEventManager& manager = GetWorld().GetMessageEventManager();
        const CMessageEvent* pevent = manager.FindMostRecentMessage(CMessageEventFilter_IsOfType(MESSAGE_EVENT_CONVERSATION_ANSWER_YES_OR_NO));
        if (pevent) {
            if (pevent->ExtraData == 1) {
                CMainGameComponent::Get()->GetDisplayEngine().StartScreenFadeOut(2.5f, 2.0f, CRGBColour(0,0,0), -1.0f);
                WaitingForFadeOut = true;
                InAtomicAction = true;
                CTCSpecialAbilities* ptc_spec;
                if (IS_THING_ALIVE(PThingUser) && PThingUser->GetTC(TCI_SPECIAL_ABILITIES, &ptc_spec))
                    ptc_spec->SetUsingBed(true);
                if (Cost > 0) {
                    CTCHeroStats* ptc_hero_stats;
                    CMainGameComponent::Get()->PeekPlayerManager().GetMainPlayer().GetPControlledCreature()->GetTC(TCI_HERO_STATS, &ptc_hero_stats);
                    ptc_hero_stats->AddMoney(-Cost,true);
                } else if (Cost == -1 && PBuilding.Get()) {
                    ForceTrespassOnceFadedOut = true;
                }
            } else {
                CGameEvent pause_event(9);
                pause_event.SetPlayer(PlayerNumberOfThingUsingBed);
                pause_event.AddUBYTE(0);
                CMainGameComponent::Get()->AddGameEvent(pause_event);
                RemoveFromGlobalTCs();
            }
            manager.RemoveMessage(pevent);
            WaitingForConfirmation = false;
        }
    } else if (WaitingForFadeOut && CMainGameComponent::Get()->GetDisplayEngine().IsScreenFullyFadedOut()) {
        WaitingForFadeOut=false;
        CGameTimeManager& game_time_manager=GetWorld().GetGameTimeManager();
        if (game_time_manager.IsNightTime()) game_time_manager.FastForwardTimeTo(0.3375f,0.0f);
        else game_time_manager.FastForwardTimeTo(0.875f,0.0f);
        if (ForceTrespassOnceFadedOut) {
            CTCVillageMember* tc_village_member;
            if (IS_THING_ALIVE(PBuilding) && PBuilding->HasTC(TCI_VILLAGE_MEMBER) && PBuilding->GetTC(TCI_VILLAGE_MEMBER,&tc_village_member) && tc_village_member->GetPVillage()) {
                CTCTrespasser* tc_trespasser;
                if (CMainGameComponent::Get()->PeekPlayerManager().GetMainPlayer().GetPControlledCreature()->GetTC(TCI_TRESPASSER,&tc_trespasser)) tc_trespasser->ForceTrespass();
            }
            ForceTrespassOnceFadedOut=false;
        }
        std::vector<CThingVillage*> villages;
        GetThingSearchTools().GetAllLeafThings<CThingVillage>(CThingFilter_None(),villages);
        if (false) {} else for (long c1=0;c1<villages.size();++c1) {
            CTCVillage* ptc_village=0;
            villages[c1]->GetTC(TCI_VILLAGE,&ptc_village);
            ptc_village->OnHeroSlept();
        }
        GetWorld().GetMessageEventManager().AddMessage(MESSAGE_EVENT_HERO_SLEPT,CMainGameComponent::GetConstantFPS(),GetParentThing().GetPos(),0,0,0.0f,0.0f,0);
        CThingPlayerCreature* phero=GetPlayerManager().GetMainPlayer().GetPPlayerCharacter();
        CTCBulletTime* ptc_bullet_time;
        if(phero->GetTC(TCI_BULLET_TIME,&ptc_bullet_time)) ptc_bullet_time->CancelMove();
        if(IS_THING_ALIVE(phero)) {
            CTCHeroExperience* ptc_hero_experience;
            if(phero->GetTC(TCI_HERO_EXPERIENCE,&ptc_hero_experience)) ptc_hero_experience->ResetExperiencePickUpMultiplier();
            phero->ModifyHealth(phero->GetMaxHealth(),false);
            CTCHeroStats* ptc_hero_stats;
            if(phero->GetTC(TCI_HERO_STATS,&ptc_hero_stats)) {
                ptc_hero_stats->ForceStaminaTo(1.0f);
                ptc_hero_stats->SoberUp();
            }
        }
        CMainGameComponent::Get()->GetDisplayEngine().EndScreenFadeOut();
        CGameEvent pause_event(9);
        pause_event.SetPlayer(PlayerNumberOfThingUsingBed);
        pause_event.AddUBYTE(0);
        CMainGameComponent::Get()->AddGameEvent(pause_event);
        WaitingForFadeIn=true;
    } else if (WaitingForFadeIn && !CMainGameComponent::Get()->GetDisplayEngine().IsScreenFadeOut()) {
        RemoveFromGlobalTCs();
        WaitingForFadeIn=false;
        InAtomicAction=false;
        CTCSpecialAbilities* ptc_spec;
        if(IS_THING_ALIVE(PThingUser) && PThingUser->GetTC(TCI_SPECIAL_ABILITIES,&ptc_spec))
            ptc_spec->SetUsingBed(false);
    }
}