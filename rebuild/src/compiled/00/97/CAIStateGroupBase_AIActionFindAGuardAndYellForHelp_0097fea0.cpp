// CAIStateGroupBase::AIAction_FindAGuardAndYellForHelp(C3DVector, unsigned long, bool) -- find the nearest guard (a creature with AI property 3 not under scripted control) and run to him while yelling for help (the yell message re-sent every 0.8 message lifespans, a one-off "run to guards" speech once moving); an apology from the hero cancels it (deed APOLOGY_ACCEPTED / REFUSED, crimes removed), reaching the guard has him report the crimes
// Leaf: ?AIAction_FindAGuardAndYellForHelp@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@VC3DVector@@K_N@Z
// Retail: Fable.exe (TLC, VC7.1 RTM) @ 0x0097FEA0, 1533 bytes, `ret 14h` (the C3DVector, the crime handle, the bool). Ego_r.pdb / FableWin.pdb
//   ?AIAction_FindAGuardAndYellForHelp@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@VC3DVector@@K_N@Z (protected); Ego_r donor 0x0084B160 sits
//   between AIAction_RunAwayFrom 0x0084AE40 and AIAction_FleeToThingProximity 0x0084B800 = the landed retail 0x0097FBE0 / 0x009804A0. Neither
//   the position nor the bool parameter is read by the retail body (as in Ego_r's signature they are passed and ignored).
// Callees (each checked against retail with tmp/rel.py): CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0,
//   CAIStateGroupBase::PeekDefinitionManager 0x008FCFB0 / PeekThingSearchTools 0x008FD000 / IsStopping 0x008FD010 / GetWorld 0x008FCFC0 /
//   AIAction_MoveToThingProximity 0x0097E380 / IsStillValid (vtable slot 5, +0x14), CDefinitionManager::GetDef<CMessageEventDef> 0x007923C0,
//   CThingPhysical::PeekPos 0x006A4D00, CThingSearchTools::GetPBestLeafThing<CFilter_Bind2<unary_negate<CThingFilter_HasScriptedControl>,
//   CThingFilter_AICreatureHasProperty>, CThingCompare_Nearest, CThingAICreature> 0x00982450 (same TU), CBaseIntelligentPointer ctor 0x00A01B10 /
//   GetPItem 0x00A01B50 / SetPItem 0x00A01B90 / dtor 0x00A01C10, __ftol2 0x00BFEA70, CVectorMap::LowerBound 0x0040F020 (the expanded GetTC
//   lookups: 0x23 CTCVillageMember, 0x0F CTCTalk, 0x42 CTCEntityEvents, 0x51 CTCHeroOpinionDeedLog, 0x4F CTCCreatureOpinionOfHero),
//   CTCVillageMember::GetPVillage 0x007EE540, CTCTalk::PlayAISpeech(const CCharString&, ...) 0x006E3B30, the brain's slot 2
//   SuspendSuspendableProcess (+0x08), NCreatureAI::StopSpeaking 0x00864580, the event's slot 1 GetType, the expression def's slot 10
//   GetInstantiationName, CDefString::operator==(const char*) 0x005B3440, CTCEntityEvents::RemoveEvent 0x008112D0,
//   CTCVillage::RemoveApologiseableCrimes 0x0065CE56 / RemoveCrimeIfApologisable 0x0065C1FC / NotifyHasApologised 0x006585EE /
//   IsHeroCriminal 0x0065BFCE, CMainGameComponent::PMainGameComponent 0x013B86A0, CWorld::PeekMyPlayer 0x0049D9D0 / GetFrame 0x0049D870,
//   CPlayer::GetPControlledCreature 0x00487DC0, CThing::GetPos 0x004C73D0, CTCHeroOpinionDeedLog::PostDeed 0x006C6990,
//   CThingCreatureBase::ClearAttackedByCreature 0x006691C0, CMessageEventManager::AddMessage(const CMessageEventDef&, ...) 0x00687730,
//   CTCCreatureOpinionOfHero::ReportAllCrimes 0x00684210. Data: "STANDARD_MESSAGE_EVENT_YELL_FOR_HELP" 0x012995C0,
//   "TEXT_AI_CRIME_RUN_TO_GUARDS" 0x0126C23C, "EXPRESSION_APOLOGY" 0x0124F590, 0.8f 0x01254DA8; vtables CIntelligentPointer<CThingAICreature>
//   0x0123E7E0 / CIntelligentPointer<const CThing> 0x01237F1C.
// Shape (each measured by a grade, 6 in pass 1): both literals are IMPLICIT const char* -> CCharString arguments (retail builds the
//   temporary before the other arguments are pushed); the village-member GetTC is a statement whose result is unused (the default GetTC
//   body stores the HasTC bit, 0x0097FFEF, into the byte the `moving` flag reuses); the speech retry loop is
//   `do { ..; if (speech != RETRY) break; yield; } while (!IsStopping()); if (speech == RETRY) stop` -- c2 threads the retest away, and a
//   for(;;) / while form nested in the outer loop is rotated (the call duplicated); the outer loop is a do-while on AI_ACTION_RUNNING (a
//   for(;;) with a bottom break loses retail's aligned loop head, `jmp` over `lea ebx,[ebx]` at 0x0097FFD8); last_yell_frame is initialised
//   before frames_between_yells (its store is scheduled between the fild and the fmul, 0x0097FFBC).
// Layout: shared headers plus the chunk 2015 overlay (hdr/README.md): new CThingFilter_HasScriptedControl / CMessageEventDef /
//   NEntityEvents_CEventExpressionPerformedTo headers and opt-ins in CAIStateGroupBase / CTCVillage / CTCEntityEvents / CThingCreatureBase /
//   CTCCreatureOpinionOfHero / CMessageEventManager.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CDEFSTRING_EQUALS
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CONVERT_MUTABLE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_ATTACKEDBY
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGAICREATURE_STATGETTYPE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_PEEK
#define FABLE_RETAIL_CAISTATEGROUPBASE_PEEKDEFINITIONMANAGER
#define FABLE_RETAIL_CAISTATEGROUPBASE_GOTOBED_WORLD
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_FINDAGUARD_B
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_SUSPENDABLE_BASE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_MESSAGEEVENTMANAGER
#define FABLE_RETAIL_CMESSAGEEVENTMANAGER_ADDDEFMESSAGE
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_WORLD
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_NAME_BODY
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_BESTLEAF
#define FABLE_RETAIL_STLP_REMOVE_COPY_IF
#define FABLE_RETAIL_STLP_NOT1
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CTCVILLAGE
#define FABLE_RETAIL_CTCVILLAGE_CRIMES
#define FABLE_RETAIL_CTCVILLAGE_APOLOGISE
#define FABLE_RETAIL_CTCVILLAGE_YELLFORHELP
#define FABLE_RETAIL_CTCVILLAGEMEMBER
#define FABLE_RETAIL_CTCTALK
#define FABLE_RETAIL_CTCTALK_AISPEECHTEXT
#define FABLE_RETAIL_CTCENTITYEVENTS
#define FABLE_RETAIL_CTCENTITYEVENTS_EVENTS
#define FABLE_RETAIL_CTCHEROOPINIONDEEDLOG
#define FABLE_RETAIL_CTCCREATUREOPINIONOFHERO
#define FABLE_RETAIL_CTCCREATUREOPINIONOFHERO_REPORTCRIMES
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingAICreature.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CWorld.h"
#include "engine/CMainGameComponent.h"
#include "engine/CPlayer.h"
#include "engine/CDefPointer.h"
#include "engine/CDefString.h"
#include "engine/CDefinitionManager.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CMessageEventDef.h"
#include "engine/CMessageEventManager.h"
#include "engine/CThingSearchTools.h"
#include "engine/CFilter_Bind2.h"
#include "engine/CThingFilter_HasScriptedControl.h"
#include "engine/CThingFilter_AICreatureHasProperty.h"
#include "engine/CThingCompare_Nearest.h"
#include "engine/CTCVillageMember.h"
#include "engine/CTCVillage.h"
#include "engine/CTCTalk.h"
#include "engine/CTCEntityEvents.h"
#include "engine/NEntityEvents_CEventExpressionPerformedTo.h"
#include "engine/CExpressionDef.h"
#include "engine/EOpinionDeedType.h"
#include "engine/CTCHeroOpinionDeedLog.h"
#include "engine/CTCCreatureOpinionOfHero.h"

// The header's MS forward enums; the enumerator names are descriptive (the PDB name sets on disk carry no enumerators), the values
// are the retail constants (the landed AIAction rows' spellings for EAIActionReturn / EAIActionRepeat / EMovementType).
enum EAIActionReturn { AI_ACTION_STOPPING = 1, AI_ACTION_DONE = 2, AI_ACTION_ABORTED = 3, AI_ACTION_RUNNING = 4 };
enum EAIActionRepeat { AI_ACTION_REPEAT_POLL = 2 };
enum EMovementType { MOVEMENT_RUN = 5 };
enum EOpinionPostDeedType { OPINION_POST_DEED_TYPE_1 = 1 };
namespace NSpeechGainManager { enum EDialogueLayer { DIALOGUE_LAYER_1 = 1 }; }
// Ego_r.pdb / FableWin.pdb ?StopSpeaking@NCreatureAI@@YIXAAVCThingCreatureBase@@@Z (free __fastcall; the creature in ecx, 0x0098013D).
namespace NCreatureAI { void __fastcall StopSpeaking(CThingCreatureBase& creature); }   // 0x00864580

EAIActionReturn CAIStateGroupBase::AIAction_FindAGuardAndYellForHelp(C3DVector position, unsigned long crime_handle, bool unused_flag)
{
    EAIActionReturn result = AI_ACTION_ABORTED;
    CThingAICreature* guard = 0;
    CDefPointer<const CMessageEventDef> message_def;
    if (!PeekDefinitionManager().GetDef("STANDARD_MESSAGE_EVENT_YELL_FOR_HELP", &message_def))
        return AI_ACTION_ABORTED;
    if (!PeekThingSearchTools().GetPBestLeafThing(
            GFFilter_Bind2(std::not1(CThingFilter_HasScriptedControl(true)), CThingFilter_AICreatureHasProperty(3)),
            CThingCompare_Nearest(GetCreature().PeekPos()), &guard))
        return AI_ACTION_ABORTED;
    CIntelligentPointer<CThingAICreature> p_guard(guard);
    long last_yell_frame = 0x80000000;
    long frames_between_yells = (long)(message_def->Lifespan * 0.8f);
    bool spoken = false;
    do {
        CTCVillageMember* village_member;
        GetCreature().GetTC(&village_member);
        CTCVillage* village = village_member->GetPVillage();
        if (village) {
            if (village->IsHeroEnemyBecauseOfCrime()) {
                result = AI_ACTION_DONE;
                break;
            }
            bool moving = false;
            result = AIAction_MoveToThingProximity(AI_ACTION_REPEAT_POLL, p_guard, 2.0f, moving, MOVEMENT_RUN, true, false,
                                                   true, true, 0.0f);
            if (!spoken && moving && (result == AI_ACTION_RUNNING || result == AI_ACTION_DONE)) {
                spoken = true;
                CTCTalk* talk;
                if (GetCreature().GetTC(&talk)) {
                    CTCTalk::ESpeechPlaybackResult speech;
                    do {
                        speech = talk->PlayAISpeech("TEXT_AI_CRIME_RUN_TO_GUARDS", false, NSpeechGainManager::DIALOGUE_LAYER_1, false, true);
                        if (speech != CTCTalk::SPEECH_PLAYBACK_FAILED_RETRY)
                            break;
                        PBrain->SuspendSuspendableProcess();
                    } while (!IsStopping());
                    if (speech == CTCTalk::SPEECH_PLAYBACK_FAILED_RETRY) {
                        NCreatureAI::StopSpeaking(GetCreature());
                        return AI_ACTION_STOPPING;
                    }
                }
            }
            CTCEntityEvents* entity_events;
            if (GetCreature().GetTC(&entity_events)) {
                for (std::list<CCountedPointer<NEntityEvents::CEventBase> >::const_iterator it = entity_events->GetEvents().begin();
                     it != entity_events->GetEvents().end(); ++it) {
                    NEntityEvents::CEventBase* event = (*it).Get();
                    if (event->GetType() == NEntityEvents::CEventExpressionPerformedTo::StatGetType()) {
                        CDefPointer<const CExpressionDef> expression =
                            GFDebugPDynamicCast(event, (NEntityEvents::CEventExpressionPerformedTo*)0)->GetPExpressionDef();
                        if (expression->GetInstantiationName() == "EXPRESSION_APOLOGY") {
                            entity_events->RemoveEvent(*event);
                            village->RemoveApologiseableCrimes(10);
                            village->RemoveCrimeIfApologisable(crime_handle);
                            village->NotifyHasApologised();
                            CThingPlayerCreature* hero =
                                CMainGameComponent::Get()->PeekWorld().PeekMyPlayer()->GetPControlledCreature();
                            if (hero) {
                                CTCHeroOpinionDeedLog* deed_log;
                                hero->GetTC(&deed_log);
                                deed_log->PostDeed(IsStillValid() ? OPINION_DEED_TYPE_APOLOGY_REFUSED : OPINION_DEED_TYPE_APOLOGY_ACCEPTED,
                                                   hero->GetPos(), 0, &GetCreature(), OPINION_POST_DEED_TYPE_1, false, false, 0,
                                                   false, 0);
                                if (!village->IsHeroCriminal() && GetCreature().GetPLastAttackedByCreature() == hero)
                                    GetCreature().ClearAttackedByCreature();
                            }
                            return AI_ACTION_ABORTED;
                        }
                    }
                }
            }
        }
        if (CWorld::GetFrame() - last_yell_frame > frames_between_yells) {
            CThingAICreature& creature = GetCreature();
            GetWorld().GetMessageEventManager().AddMessage(*message_def, creature.GetPos(), &creature, p_guard.Get(), 0);
            last_yell_frame = CWorld::GetFrame();
        }
    } while (result == AI_ACTION_RUNNING);
    if (result != AI_ACTION_DONE)
        return result;
    if (p_guard.Get()) {
        CTCVillageMember* guard_village_member;
        p_guard->GetTC(&guard_village_member);
        CTCVillage* guard_village = guard_village_member->GetPVillage();
        if (guard_village)
            guard_village->SetFrameBribeEnds(0);
        CTCCreatureOpinionOfHero* guard_opinion;
        if (p_guard->GetTC(&guard_opinion)) {
            CTCCreatureOpinionOfHero* opinion;
            if (GetCreature().GetTC(&opinion))
                opinion->ReportAllCrimes(guard_opinion);
        }
    }
    return AI_ACTION_DONE;
}