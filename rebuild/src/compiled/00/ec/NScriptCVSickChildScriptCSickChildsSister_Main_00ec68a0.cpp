// NScript::CV_SickChildScript::CSickChildsSister::Main() -- the sick child's sister: until she has led the hero (and the mother intro is not done) she follows the hero, calls him over every ten seconds with an attention animation and complains when hit; talked to, she asks the hero to follow and runs to her mother, waving and waiting whenever the hero falls behind; afterwards she thanks the hero, reminds him of the quest (numbered post-intro reminders) and complains when hit.
// Grade: RELOCATION_MATCH -- actual COFF operands checked against their native targets.
// Inline-budget: Forced original Timer equality/assignment preserve native expansions; plain comparison rejects the far site (measured score20157). Donor common latch ownership restores all layout without budget flags.
// Leaf: ?Main@CSickChildsSister@CV_SickChildScript@NScript@@EAEXXZ
// Retail: 0x00EC68A0, 5962 bytes, thiscall.
#define FABLE_RETAIL_CLEVELSCRIPTBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_CPREDICATEBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_CENTITYSCRIPTBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_CISTHINGALIVE_ORIGINAL_ACCESS
#define FABLE_RETAIL_CSCRIPTRESOURCE_ORIGINAL_ACCESS
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#define FABLE_RETAIL_CBASEOBJECT_ORIGINAL_ACCESS
#define FABLE_RETAIL_CSCRIPTTHING_ORIGINAL_ACCESS
#define FABLE_RETAIL_CTIMER_ORIGINAL_ACCESS
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#include "engine/C3DVector.h"
#include "engine/CV_SickChildScript.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/TRetVoid.h"
#include "engine/CIsThingAlive.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum ETextGroupSelectionMethod { GROUP_SELECT_NONE=0, GROUP_SELECT_RANDOM=1, GROUP_SELECT_RANDOM_NO_REPEAT=2, GROUP_SELECT_SEQUENTIAL=3, GROUP_SELECT_FIRST_ONLY=4 };
#include "engine/EHeroAbility.h"
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRING_OUTOFLINE_CONVERSION
#define FABLE_RETAIL_CDEFSTRINGTABLE
#include "engine/CTimer.h"
#include "engine/fable_script_def_global.h"
#include "engine/fable_script_text.h"
enum ECrime { CRIME_NONE=0, CRIME_WEAPON_OUT=1, CRIME_TRESPASS=2, CRIME_VANDALISM=3, CRIME_LOCKPICKING=4, CRIME_PICK_POCKETS=5, CRIME_THEFT=6, CRIME_ASSAULT=7, CRIME_GBH=8, CRIME_MURDER=9, CRIME_LAST=10 };
enum EOpinionPostDeedType { OPINION_POST_DEED_TYPE_NULL=0, OPINION_POST_DEED_TYPE_RECIPIENT=1, OPINION_POST_DEED_TYPE_WITNESSES=2, OPINION_POST_DEED_TYPE_VILLAGE=4, OPINION_POST_DEED_TYPE_GUARDS=16, OPINION_POST_DEED_TYPE_RECIPIENT_AND_WITNESSES=3, OPINION_POST_DEED_TYPE_ALL=7 };
template<> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing):Thing(thing) {}
extern CGameScriptInterfaceBase* pDSTGame;
inline bool CScriptThing::IsDead() const { return !IsAlive(); }
inline CTimer::CTimer() { TimerIndex=pDSTGame->RegisterTimer(); }
inline CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }
extern "C" int FABLE_CDECL rand();
namespace NGlobalConsole { extern bool GDefaultToUsingPhysicsInCutsceneAnimations; }   // 0x01375748
enum EScriptEntityMoveType { ENTITY_MOVE_WALK=0, ENTITY_MOVE_RUN=1, ENTITY_MOVE_SNEAK=2 };
inline long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
FABLE_FORCEINLINE bool CTimer::operator==(long seconds) const { return GetTime()==seconds; }
FABLE_FORCEINLINE CTimer& CTimer::operator=(long seconds) { pDSTGame->SetTimer(TimerIndex, seconds); return *this; }
bool FABLE_FASTCALL IsDistanceBetweenThingsUnder(const CScriptThing& thing_a, const CScriptThing& thing_b, float distance);   // 0x00CBE2FF
float FABLE_FASTCALL GetDistanceBetweenThings(const CScriptThing& thing_a, const CScriptThing& thing_b);   // 0x00CBE512
inline const C3DVector& CScriptThing::GetPos() const { if (PImp.IsNull()) return NullCoord; return PImp->GetPos(); }
void NScript::CV_SickChildScript::CSickChildsSister::Main()
{
    TRetVoid _Fail_Val;
    SetPredicate(CIsThingAlive(Me));
    Game.NewScriptFrame();
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    CScriptGameResourceObjectScriptedThingBase seh_me;
    if (seh_me.IsValid()) seh_me.Release();
    while (!Game.StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    if (!LeadHeroToSickChild && !ParentClass.MotherIntroDone) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        seh_me.FollowThing(Game.GetHero(),1.0f,true);
        CTimer GetAttentionTimer;
        while (!Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO") && !ParentClass.MotherIntroDone) {
            Game.NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (GetAttentionTimer==0) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                seh_me.StopFollowingThing(Game.GetHero());
                long HLStyleConversationIndex=Game.AddNewConversation(Me,false,false);
                Game.AddPersonToConversation(HLStyleConversationIndex,Game.GetHero());
                Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_B10_SISTER_CALLS_OVER_10",false,Me,Game.GetHero());
                long RandAnim=rand()%pGlobalScriptDef->SickChildsSisterAttentionAnims.size();
                seh_me.PlayAnimation(pGlobalScriptDef->SickChildsSisterAttentionAnims[RandAnim],false,false,false,true,NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations,false,false);
                while (seh_me.IsPerformingScriptTask()) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                GetAttentionTimer=10;
                seh_me.FollowThing(Game.GetHero(),1.0f,true);
            }
            if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL,"SCRIPT_NAME_HERO"))) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                seh_me.StopFollowingThing(Game.GetHero());
                CScriptGameResourceObjectMovieBase speech_movie;
                Game.StartMovieSequence("",speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game.GetHero(),"TEXT_QST_B10_SISTER_ON_HIT",GROUP_SELECT_RANDOM_NO_REPEAT,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                seh_me.FollowThing(Game.GetHero(),1.0f,true);
            }
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        seh_me.StopFollowingThing(Game.GetHero());
        if (!ParentClass.MotherIntroDone) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            {
                Game.FixMovieSequenceCamera(true);
                Game.CameraUseCameraPoint("CAM_SC_GIRL",Me,-1.0f,0,-1);
                CScriptGameResourceObjectMovieBase speech_movie;
                Game.StartMovieSequence("",speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game.GetHero(),"TEXT_QST_B10_SISTER_TOLD_TO_FOLLOW_20",GROUP_SELECT_NONE,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                Game.FixMovieSequenceCamera(false);
            }
            CScriptThing Mother=Game.GetThingWithScriptName("SickChildsMother");
            Game.SetThingHasInformation(Me,false,false,false);
            {
                enum ELeaderState { LS_RUNNING=0, LS_WALKING=1, LS_STOPPED=2 };
                bool bLeadHeroToPointLatch=false;
                bool MeIsNearerThanThingToPoint=false;
                ELeaderState current_state=LS_RUNNING;
                CTimer CallToHeroTimer;
                seh_me.ClearCommands();
                seh_me.MoveToPosition(Mother.GetPos(),1.0f,ENTITY_MOVE_RUN,false,true);
                while (!IsDistanceBetweenThingsUnder(Me,Mother,2.0f) ) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    MeIsNearerThanThingToPoint=GetDistanceBetweenThings(Me,Mother)<GetDistanceBetweenThings(Game.GetHero(),Mother);
                    if (IsDistanceBetweenThingsUnder(Me,Game.GetHero(),15.0f) || !MeIsNearerThanThingToPoint) {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        if (IsDistanceBetweenThingsUnder(Me,Game.GetHero(),11.0f) || !MeIsNearerThanThingToPoint) {
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            if (current_state!=LS_RUNNING) {
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                                seh_me.ClearCommands();
                                seh_me.MoveToPosition(Mother.GetPos(),1.0f,ENTITY_MOVE_RUN,false,true);
                                current_state=LS_RUNNING;
                            }
                        } else {
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            if (current_state!=LS_WALKING) {
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                                seh_me.ClearCommands();
                                seh_me.MoveToPosition(Mother.GetPos(),1.0f,ENTITY_MOVE_WALK,false,true);
                                current_state=LS_WALKING;
                            }
                        }
                        bLeadHeroToPointLatch=false;
                        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            seh_me.ClearCommands();
                            CScriptGameResourceObjectMovieBase speech_movie;
                            Game.StartMovieSequence("",speech_movie);
                            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                            if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                                seh_me.Speak(Game.GetHero(),"TEXT_QST_B10_SISTER_THIS_WAY",GROUP_SELECT_NONE,false,true,false);
                                while (seh_me.IsPerformingScriptTask()) {
                                    Game.NewScriptFrame();
                                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                                }
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            current_state=LS_STOPPED;
                        }
                        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL,"SCRIPT_NAME_HERO"))) {
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            seh_me.ClearCommands();
                            CScriptGameResourceObjectMovieBase speech_movie;
                            Game.StartMovieSequence("",speech_movie);
                            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                            if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                                seh_me.Speak(Game.GetHero(),"TEXT_QST_B10_SISTER_ON_HIT",GROUP_SELECT_RANDOM_NO_REPEAT,false,true,false);
                                while (seh_me.IsPerformingScriptTask()) {
                                    Game.NewScriptFrame();
                                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                                }
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            current_state=LS_STOPPED;
                        }
                    } else {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        seh_me.ClearCommands();
                        seh_me.ClearAllActions();
                        if (!bLeadHeroToPointLatch) {
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            Game.EntitySetFacingAngleTowardsThing(Me,Game.GetHero(),false);
                            long HLStyleConversationIndex=Game.AddNewConversation(Me,false,false);
                            Game.AddPersonToConversation(HLStyleConversationIndex,Game.GetHero());
                            Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_B10_SISTER_THIS_WAY",false,Me,Game.GetHero());
                            while (seh_me.IsPerformingScriptTask()) {
                                Game.NewScriptFrame();
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            Game.Pause(0.8f);
                            seh_me.PlayAnimation("STANDARD_WAVE",false,false,false,true,NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations,false,false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game.NewScriptFrame();
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            bLeadHeroToPointLatch=true;
                            current_state=LS_STOPPED;
                            CallToHeroTimer=5;
                        }
                    }
                    if (bLeadHeroToPointLatch && CallToHeroTimer==0) {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        bLeadHeroToPointLatch=false;
                    }
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            LeadHeroToSickChild=true;
        }
    }
    Game.ClearThingHasInformation(Me);
    long LineNumber=10;
    for (; !false; Game.NewScriptFrame()) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (ParentClass.FinishedQuest) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                CScriptGameResourceObjectMovieBase speech_movie;
                Game.StartMovieSequence("",speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game.GetHero(),"TEXT_QST_B10_SISTER_THANKS",GROUP_SELECT_NONE,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
            }
            else if (!ParentClass.MotherIntroDone) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                CScriptGameResourceObjectMovieBase speech_movie;
                Game.StartMovieSequence("",speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game.GetHero(),"TEXT_QST_B10_SISTER_REMINDER",GROUP_SELECT_NONE,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
            }
            else {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                CCharString FullText;
                FullText="TEXT_QST_B10_SISTER_REMINDER_POSTINTRO_"+GFIntToCharString(LineNumber);
                if (!Game.TextEntryExists(FullText)) {
                    LineNumber=10;
                    FullText="TEXT_QST_B10_SISTER_REMINDER_POSTINTRO_"+GFIntToCharString(LineNumber);
                }
                LineNumber+=10;
                CScriptGameResourceObjectMovieBase speech_movie;
                Game.StartMovieSequence("",speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game.GetHero(),FullText,GROUP_SELECT_NONE,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
            }
        }
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL,"SCRIPT_NAME_HERO"))) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            CScriptGameResourceObjectMovieBase speech_movie;
            Game.StartMovieSequence("",speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
            if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                seh_me.Speak(Game.GetHero(),"TEXT_QST_B10_SISTER_ON_HIT",GROUP_SELECT_NONE,false,true,false);
                while (seh_me.IsPerformingScriptTask()) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
        }
    }
}