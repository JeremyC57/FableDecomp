// 007b3120
// Leaf: ?FrameUpdate@CTCGuard@@UAEXXZ
// Flags: none
#define FABLE_RETAIL_GUARD_HEALTH_PRECISION_PASS29
#define FABLE_RETAIL_CMAINGAMECOMPONENT_GETWORLD
#define FABLE_RETAIL_CTCHEROOPINIONDEEDLOG
#define FABLE_RETAIL_CTCHEROOPINIONDEEDLOG_DEEDS
#define FABLE_RETAIL_CTCCREATUREOPINIONOFHERO
#define FABLE_RETAIL_CTCCREATUREOPINIONOFHERO_INFORM
#define FABLE_RETAIL_CTCCREATUREOPINIONOFHERO_INFORM_HANDLE
#define FABLE_RETAIL_CTCVILLAGE_WITNESSEDCRIME
#define FABLE_RETAIL_CREATURE_ONCREATE_PASS27
#define FABLE_RETAIL_CTHINGCREATUREDEF_MEMBERS
#include "engine/EThingCreatureProperty.h"
#define FABLE_RETAIL_NCREATUREAI_SPEAKING
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_STLP_STACK_EMPTY
#define FABLE_RETAIL_GUARD_REACT_TO_BRIBE_PASS27
#define FABLE_RETAIL_GUARD_CRIMINAL_UPDATE_PASS29
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_CTHING_GETTC_DONOR_NAMES_PASS23
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGPHYSICAL_MAXHEALTH
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CTHING_ORIGINAL_VTABLE
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CTCVILLAGE
#define FABLE_RETAIL_CTCVILLAGE_FINEDIALOGUE
#define FABLE_RETAIL_CTCVILLAGE_FINEAMOUNT
#define FABLE_RETAIL_CTCVILLAGE_YELLFORHELP
#define FABLE_RETAIL_CTCVILLAGEMEMBER
#define FABLE_RETAIL_CTCPERCEIVETHING_SMELL
#define FABLE_RETAIL_CTCPERCEIVETHING_AWAREOFTHING
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CTCHEROONLINESCOREBOARD
#define FABLE_RETAIL_CUSERPROFILEMANAGER_SCOREBOARD
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_LHHTTP2
#define FABLE_RETAIL_CTCHEROONLINESCOREBOARD_NETWORK
#define FABLE_RETAIL_CTCHEROONLINESCOREBOARD_MEMBERS
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_CDISPLAYENGINE
#define FABLE_RETAIL_CDISPLAYENGINE_STARTSCREENFADEOUT
#define FABLE_RETAIL_CTCEMOTEICON_STATTYPE
#define FABLE_RETAIL_EXPRESSION_INIT_PASS29
#define FABLE_RETAIL_CTCTALK_VOICED_GROUP_PASS24
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CSUBTITLERENDERER
#define FABLE_RETAIL_CSUBTITLERENDERER_SETTEXT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_NGAMETEXT_CONSTRUCTSTRING_LONGS
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCHEROSTATS
#define FABLE_RETAIL_CTCHEROSTATS_FINEPAID
#define FABLE_RETAIL_GUARD_FRAMEUPDATE_PASS29
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_PLAYERMANAGER
#define FABLE_RETAIL_CMAINGAMECOMPONENT_DISPLAYENGINE
#define FABLE_RETAIL_CMAINGAMECOMPONENT_TEXTBANK
#define FABLE_RETAIL_CREGION
#define FABLE_RETAIL_CREGION_PREGIONDEF
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CDEFSTRING_EQUALS
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CTCENTITYEVENTS
#define FABLE_RETAIL_CTCENTITYEVENTS_ADDEVENT
#define FABLE_RETAIL_CTCENTITYEVENTS_EVENTS
#include "engine/CDefString.h"
#include "engine/CExpressionDef.h"
#include "engine/NEntityEvents_EEventType.h"
#ifdef _MSC_VER
#pragma optimize("s",on)
#endif
#include "engine/NEntityEvents_CEventExpressionPerformedTo.h"
#ifdef _MSC_VER
#pragma optimize("",on)
#endif
#include "engine/NEntityEvents_CEventApologyReceived.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CTCEntityEvents.h"
#include "engine/CRegionDef.h"
#include "engine/CRegion.h"
#include "engine/CThing.h"
#include "engine/CThingPhysical.h"
#include "engine/CThingGameObject.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingAICreature.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/ThingAlive.h"
#include "engine/CMainGameComponent.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"
#include "engine/CTCHeroStats.h"
#include "engine/CPlayerGui.h"
#include "engine/NGameText_CDataBank.h"
#include "engine/fable_script_text.h"
void* __cdecl operator new(size_t size);               // 0x00BFEA1A

#include "engine/CTCVillage.h"
#include "engine/CTCVillageMember.h"
#include "engine/CTCPerceiveThing.h"
#include "engine/CWorldMap.h"
#include "engine/CWorld.h"
#include "engine/CTCHeroOnlineScoreboard.h"
#include "engine/CRGBColour.h"
#include "engine/CDisplayEngine.h"
#include "engine/CTCEmoteIcon.h"



#include "engine/NCreatureAI.h"
#include "engine/CTCGuard.h"

#include "engine/GFLimit.h"
const long TIME_FOR_APOLOGY_IN_SECONDS = 10;
const long TIME_NEEDED_BETWEEN_APOLOGIES_IN_SECONDS = 15;
const long MIN_FRAMES_BETWEEN_CRIMINALITY_AND_GREETING = 300;

template<> inline const long& FABLE_FASTCALL GFLimitReturn<long>(
    const long& value, const long& lower, const long& upper)
{
    return value < lower ? lower : (upper < value ? upper : value);
}
long CTCGuard::GetTimeAgoThatCrimesCanBeApologisedFor() const
{
    long frames = CWorld::GetFrame() - FrameLastReceivedApology;
    long seconds = (frames - CMainGameComponent::GetConstantFPS() *
        TIME_NEEDED_BETWEEN_APOLOGIES_IN_SECONDS) / CMainGameComponent::GetConstantFPS();
    return GFLimitReturn(seconds, 0L, TIME_FOR_APOLOGY_IN_SECONDS);
}

#ifdef _MSC_VER
#pragma optimize("s",on)
#endif


#ifdef _MSC_VER
__declspec(noinline)
#endif
bool CTCVillage::IsHeroCriminal()
{
    bool is_hero_criminal = GetFineAmountForCurrentCrimes() > 0;
    if (CurrentIsHeroCriminal != is_hero_criminal) {
        CPlayerGui::Get().UpdateMiniMapMarkers(false);
        CurrentIsHeroCriminal = is_hero_criminal;
    }
    return CurrentIsHeroCriminal;
}

#ifdef _MSC_VER
#pragma optimize("t",on)
#endif

#include "engine/CBaseObjectPointer.h"
inline void CBaseIntelligentPointer::ClearData() const {
    if (PData) {
        --PData->refcount;
        if (!PData->refcount && !PData->object) delete PData;
        PData = 0;
    }
}

#ifdef _MSC_VER
__declspec(noinline)
#endif
CBaseObject* CBaseIntelligentPointer::GetPItem() const {
    if (PData) {
        if (PData->object) return PData->object;
        ClearData();
    }
    return 0;
}
#include "engine/CThingCreatureDef.h"

#ifdef _MSC_VER
__declspec(noinline)
#endif
unsigned long CThingCreatureBase::GetProperty() const { return PDef->Property; }

#include "stlport/fable_map.h"
#include "engine/CTCHeroOpinionDeedLog.h"
#include "engine/CTCCreatureOpinionOfHero.h"
#include "engine/COpinionDeedEffectsDef.h"
inline void CTCGuard::ReportCrime(CTCVillage& tc_village,long deed_handle,EOpinionDeedType deed_type,bool make_enemy_immediately)
{
    tc_village.ReportCrime(deed_handle);
    if (make_enemy_immediately) tc_village.SetHeroAsEnemyBecauseOfCrime();
    CTCCreatureOpinionOfHero* ptc_village_opinion;
    if (tc_village.GetParentThing().GetTC(&ptc_village_opinion) && !ptc_village_opinion->KnowsOfDeed(deed_handle,deed_type,true))
        ptc_village_opinion->InformOfDeed(deed_handle,false);
}
void CTCGuard::ReportAllPendingCrimes(CTCVillage* pvillage)
{
    if (pvillage) {
        CThingPlayerCreature* player = CMainGameComponent::Get()->GetWorld().PeekMyPlayer()->GetPControlledCreature();
        CTCHeroOpinionDeedLog* ptc_deed_log=0;
        if (player) player->GetTC(&ptc_deed_log);
        bool needs_security_sweep=false;
        for (std::deque<CCrimeWaitingForBribe>::iterator it=PendingCrimes.begin(),end_it=PendingCrimes.end();it!=end_it;++it) {
            ReportCrime(*pvillage,it->DeedHandle,it->Type,false);
            if (ptc_deed_log) {
                CDefPointer<const COpinionDeedEffectsDef> pdeed_effects_def=ptc_deed_log->PeekPDeedEffectsDef(it->Type);
                if (pdeed_effects_def->CrimeSeverity==CRIME_SEVERITY_MODERATE || pdeed_effects_def->CrimeSeverity==CRIME_SEVERITY_SERIOUS)
                    needs_security_sweep=true;
            }
        }
        if (needs_security_sweep) pvillage->StartSecuritySweep(false);
    }
    PendingCrimes.clear();
}

void CTCGuard::FrameUpdate()
{
    if (WaitingForRegionKickConfirmation) {
        if (!CPlayerGui::Get().GetSubtitleRenderer().IsDisplayingText()) {
            WaitingForRegionKickConfirmation = false;
            CThingPlayerCreature* phero = GetPlayerManager().GetMainPlayer().GetPControlledCreature();
            CTCHeroStats* ptc_hero_stats;
            if (IS_THING_ALIVE(phero) && phero->GetTC(&ptc_hero_stats))
                ptc_hero_stats->KickHeroToMostRecentSafeRegion();
        }
        return;
    }
    if (!IS_THING_ALIVE(&GetParentThing()))
        return;
    CTCVillageMember* ptc_village_member;
    GetParentThing().GetTC(&ptc_village_member);
    CTCVillage* pvillage=ptc_village_member->GetPVillage();
    CThingCreatureBase& guard=static_cast<CThingCreatureBase&>(GetParentThing());
    CTCEntityEvents* ptc_entity_events;
    if (guard.GetTC(&ptc_entity_events)) {
        const std::list<CCountedPointer<NEntityEvents::CEventBase> >& event_list = ptc_entity_events->GetEvents();
        std::list<CCountedPointer<NEntityEvents::CEventBase> >::const_iterator it=event_list.begin(), next_it=it;
        for (;it!=event_list.end();it=next_it) {
            ++next_it;
            NEntityEvents::CEventBase& event_base=**it;
            if (event_base.GetType() == NEntityEvents::CEventExpressionPerformedTo::StatGetType()) {
                NEntityEvents::CEventExpressionPerformedTo& devent=static_cast<NEntityEvents::CEventExpressionPerformedTo&>(event_base);
                CDefPointer<const CExpressionDef> expression_def=devent.GetPExpressionDef();
                if (expression_def->GetInstantiationName() == "EXPRESSION_APOLOGY") {
                    ptc_entity_events->RemoveEvent(event_base);
                    if (pvillage) {
                        long time_ago_that_crimes_will_be_removed = GetTimeAgoThatCrimesCanBeApologisedFor();
                        if (time_ago_that_crimes_will_be_removed > 0)
                            pvillage->RemoveApologiseableCrimes(time_ago_that_crimes_will_be_removed);
                        bool apology_accepted = !pvillage->IsHeroCriminal();
                        if (apology_accepted)
                            FrameLastReceivedApology = CWorld::GetFrame();
                        ptc_entity_events->AddEvent(new NEntityEvents::CEventApologyReceived(apology_accepted));
                    }
                }
            }
        }
    }
    if (pvillage) {
        CThingPlayerCreature* pplayer = GetPlayerManager().GetMainPlayer().GetPControlledCreature();
        if (pvillage->IsHeroCriminal()) {
            CThing* perceived_guard = &GetParentThing();
            CTCPerceiveThing* ptc_perceive_thing;
            if (perceived_guard->GetTC(&ptc_perceive_thing)) {
                if (ptc_perceive_thing->GetPerceivedThing() && ptc_perceive_thing->IsThingPerceived() && ptc_perceive_thing->GetPerceivedThing() == pplayer)
                    pvillage->SetFramePlayerLastSeenByGuard(CWorld::GetFrame());
            }
            long until = GetWorld().GetFrame() + MIN_FRAMES_BETWEEN_CRIMINALITY_AND_GREETING;
            long current = (long)pvillage->GetFrameForNextGuardGreeting();
            pvillage->SetFrameForNextGuardGreeting((float)std::max(current, until));
        }
        if (pplayer && pplayer->GetHealth() == 1.0f && pplayer->PeekPLastAttackedByCreature() && pplayer->PeekPLastAttackedByCreature()->GetProperty() == THING_CREATURE_PROPERTY_GUARD) {
            CWorldMap* worldMap = &GetWorldMap();
            const CRegion& region = GetWorldMap().GetRegion(GetWorldMap().GetRegionNumberMapIsIn(worldMap->GetMapNumberAt(GetParentThing().GetPos())));
            CDefPointer<const CRegionDef> pregion_def = region.GetPRegionDef();
            if (pregion_def.Get()) {
                if (pregion_def.Get()->UseCrimes) {
                    long fine_amount = pvillage->GetFineAmountForCurrentCrimes();
                    if (fine_amount > 0) {
                        long fine_amount_left_in_village = 0;
                        CTCHeroStats* ptc_hero_stats = 0;
                        if (pplayer->GetTC(&ptc_hero_stats)) {
                            long money_available = ptc_hero_stats->GetMoney();
                            if (money_available < fine_amount)
                                fine_amount = money_available;
                            ptc_hero_stats->AddMoney(-fine_amount, true);
                            pvillage->PayFines(fine_amount);
                            ptc_hero_stats->SetFinePaid(ptc_hero_stats->GetFinePaid() + fine_amount);
                            CThing* player = CMainGameComponent::Get()->PeekPlayerManager().GetMainPlayer().GetPControlledCreature();
                            CTCHeroOnlineScoreboard* ptc_hero_scoreboard;
                            player->GetTC(&ptc_hero_scoreboard);
                            ptc_hero_scoreboard->UpdateScoreForActivity(CTCHeroOnlineScoreboard::OSA_TOTAL_FINES_PAID, (float)ptc_hero_stats->GetFinePaid(), 0);
                            ptc_hero_stats->PrepareToKickHeroToMostRecentSafeRegion();
                            WaitingForRegionKickConfirmation = true;
                        } else
                            pvillage->ClearCrimes();
                        if (ptc_hero_stats) ptc_hero_stats->SetTimesThrownOut(ptc_hero_stats->GetTimesThrownOut() + 1);
                        CWideString message = CMainGameComponent::Get()->GetTextBank().GetTextBySymbol("TEXT_GUI_CRIME_FINE_KICKED_OUT");
                        NGameText::ConstructString(message, fine_amount);
                        if (fine_amount_left_in_village > 0) {
                            CWideString message2 = CMainGameComponent::Get()->GetTextBank().GetTextBySymbol("TEXT_GUI_CRIME_FINE_STILL_TO_PAY");
                            NGameText::ConstructString(message2, fine_amount_left_in_village);
                            message += L" ";
                            message += message2;
                        }
                        pplayer->ModifyHealth(pplayer->GetMaxHealth() * 0.25f, true);
                        CMainGameComponent::Get()->GetDisplayEngine().StartScreenFadeOut(0.5f, 0.5f, CRGBColour(0, 0, 0, 0xff), -1.0f);
                        CPlayerGui::Get().GetSubtitleRenderer().SetText(CMainGameComponent::Get()->PeekCutSceneMainFont(), message, 10, 0, 0, true);
                    }
                }
            }
        }
    }
    if (!PendingCrimes.empty() && CWorld::GetFrame() - FramePendingCrimesAdded > CMainGameComponent::GetConstantFPS() * 15)
        ReportAllPendingCrimes(pvillage);
    if (BribePool) {
        if (!NCreatureAI::IsSpeaking(guard)) {
            CTCEmoteIcon* emote_icon;
            guard.GetTC(&emote_icon);
            CCharString text = GFIntToCharString(BribePool);
            emote_icon->SetEmoteText(CTCEmoteIcon::EMOTE_PRIORITY_BRIBE, text.ToWideString(), 1.0f, 0.3f);
            emote_icon->SetReplaceable(true);
        }
    }
}