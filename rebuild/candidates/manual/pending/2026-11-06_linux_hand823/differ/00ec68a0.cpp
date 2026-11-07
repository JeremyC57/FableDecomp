// NScript::CV_SickChildScript::CSickChildsSister::Main() -- the sick child's sister: until she has led the hero (and the mother intro is not done) she follows the hero, calls him over every ten seconds with an attention animation and complains when hit; talked to, she asks the hero to follow and runs to her mother, waving and waiting whenever the hero falls behind; afterwards she thanks the hero, reminds him of the quest (numbered post-intro reminders) and complains when hit.
// Grade: DIFFER(5969v5962) -- 67 difflines: retail places the rotated test of the run-to-mother loop plus the loop exit after the far-from-hero branch and moves the near branch after the exit code, ours lays near before the test (+7 bytes); plus one GetTimer register swap. (The ~WaveTimer / LeadHeroToSickChild store order is fixed by the inner scope block.)
// Next idea: a loop spelling whose back edge belongs to the far branch; chunk 823 tried arm order (s1-s3), plain-inline CTimer members (g1-g3, out-of-line calls), do-while heat (k2/k3), inline operator== (k4-k6), for/break, bool-done and trailing-continue loops (L1-L3): identical or worse
// Leaf: ?Main@CSickChildsSister@CV_SickChildScript@NScript@@EAEXXZ
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
enum EHeroAbility { HERO_ABILITY_NULL=0, HERO_ABILITY_FORCE_PUSH=1, HERO_ABILITY_TIME_SPELL=2, HERO_ABILITY_ENFLAME_SPELL=3, HERO_ABILITY_PHYSICAL_SHIELD_SPELL=4, HERO_ABILITY_TURNCOAT_SPELL=5, HERO_ABILITY_DRAIN_LIFE_SPELL=6, HERO_ABILITY_RAISE_DEAD_SPELL=7, HERO_ABILITY_BERSERK=8, HERO_ABILITY_DOUBLE_STRIKE=9, HERO_ABILITY_SUMMON_SPELL=10, HERO_ABILITY_LIGHTNING_SPELL=11, HERO_ABILITY_BATTLE_CHARGE=12, HERO_ABILITY_ASSASSIN_RUSH=13, HERO_ABILITY_HEAL_LIFE_SPELL=14, HERO_ABILITY_GHOST_SWORD_SPELL=15, HERO_ABILITY_FIREBALL_SPELL=16, HERO_ABILITY_MULTI_ARROW=17, HERO_ABILITY_DIVINE_WRATH_SPELL=18, HERO_ABILITY_UNHOLY_POWER_SPELL=19, MAX_NUMBER_OF_HERO_ABILITIES=20 };
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
extern "C" int __cdecl rand();
namespace NGlobalConsole { extern bool GDefaultToUsingPhysicsInCutsceneAnimations; }   // 0x01375748
enum EScriptEntityMoveType { ENTITY_MOVE_WALK=0, ENTITY_MOVE_RUN=1, ENTITY_MOVE_SNEAK=2 };
__forceinline long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
__forceinline CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex, time); return *this; }
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& thing_a, const CScriptThing& thing_b, float distance);   // 0x00CBE2FF
float __fastcall GetDistanceBetweenThings(const CScriptThing& thing_a, const CScriptThing& thing_b);   // 0x00CBE512
inline const C3DVector& CScriptThing::GetPos() const { if (PImp.IsNull()) return NullCoord; return PImp->GetPos(); }
void NScript::CV_SickChildScript::CSickChildsSister::Main()
{
    TRetVoid _Fail_Val;
    SetPredicate(CIsThingAlive(Me));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    CScriptGameResourceObjectScriptedThingBase seh_me;
    if (seh_me.IsValid()) seh_me.Release();
    while (!Game->StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    if (!LeadHeroToSickChild && !ParentClass.MotherIntroDone) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        seh_me.FollowThing(Game->GetHero(),1.0f,true);
        CTimer CallOverTimer;
        while (!Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO") && !ParentClass.MotherIntroDone) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (CallOverTimer.GetTime()==0) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                seh_me.StopFollowingThing(Game->GetHero());
                long conversation=Game->AddNewConversation(Me,false,false);
                Game->AddPersonToConversation(conversation,Game->GetHero());
                Game->AddLineToConversation(conversation,"TEXT_QST_B10_SISTER_CALLS_OVER_10",false,Me,Game->GetHero());
                long anim_index=rand()%pGlobalScriptDef->SickChildsSisterAttentionAnims.size();
                seh_me.PlayAnimation(pGlobalScriptDef->SickChildsSisterAttentionAnims[anim_index],false,false,false,true,NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations,false,false);
                while (seh_me.IsPerformingScriptTask()) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                CallOverTimer=10;
                seh_me.FollowThing(Game->GetHero(),1.0f,true);
            }
            if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL,"SCRIPT_NAME_HERO"))) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                seh_me.StopFollowingThing(Game->GetHero());
                CScriptGameResourceObjectMovieBase speech_movie;
                Game->StartMovieSequence("",speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game->GetHero(),"TEXT_QST_B10_SISTER_ON_HIT",GROUP_SELECT_RANDOM_NO_REPEAT,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                seh_me.FollowThing(Game->GetHero(),1.0f,true);
            }
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        seh_me.StopFollowingThing(Game->GetHero());
        if (!ParentClass.MotherIntroDone) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            {
                Game->FixMovieSequenceCamera(true);
                Game->CameraUseCameraPoint("CAM_SC_GIRL",Me,-1.0f,0,-1);
                CScriptGameResourceObjectMovieBase speech_movie;
                Game->StartMovieSequence("",speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game->GetHero(),"TEXT_QST_B10_SISTER_TOLD_TO_FOLLOW_20",GROUP_SELECT_NONE,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                Game->FixMovieSequenceCamera(false);
            }
            CScriptThing Mother=Game->GetThingWithScriptName("SickChildsMother");
            Game->SetThingHasInformation(Me,false,false,false);
            {
                bool WavedAtHero=false;
                long MoveState=0;
                CTimer WaveTimer;
                seh_me.ClearCommands();
                seh_me.MoveToPosition(Mother.GetPos(),1.0f,ENTITY_MOVE_RUN,false,true);
                while (!IsDistanceBetweenThingsUnder(Me,Mother,2.0f)) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    bool HeroFurther=GetDistanceBetweenThings(Me,Mother)<GetDistanceBetweenThings(Game->GetHero(),Mother);
                    if (!IsDistanceBetweenThingsUnder(Me,Game->GetHero(),15.0f) && HeroFurther) {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        seh_me.ClearCommands();
                        seh_me.ClearAllActions();
                        if (!WavedAtHero) {
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            Game->EntitySetFacingAngleTowardsThing(Me,Game->GetHero(),false);
                            long conversation=Game->AddNewConversation(Me,false,false);
                            Game->AddPersonToConversation(conversation,Game->GetHero());
                            Game->AddLineToConversation(conversation,"TEXT_QST_B10_SISTER_THIS_WAY",false,Me,Game->GetHero());
                            while (seh_me.IsPerformingScriptTask()) {
                                Game->NewScriptFrame();
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            Game->Pause(0.8f);
                            seh_me.PlayAnimation("STANDARD_WAVE",false,false,false,true,NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations,false,false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game->NewScriptFrame();
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            WavedAtHero=true;
                            MoveState=2;
                            WaveTimer=5;
                        }
                        if (WaveTimer.GetTime()==0) {
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            WavedAtHero=false;
                        }
                    }
                    else {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        if (!IsDistanceBetweenThingsUnder(Me,Game->GetHero(),11.0f) && HeroFurther) {
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            if (MoveState!=1) {
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                                seh_me.ClearCommands();
                                seh_me.MoveToPosition(Mother.GetPos(),1.0f,ENTITY_MOVE_WALK,false,true);
                                MoveState=1;
                            }
                        }
                        else {
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            if (MoveState!=0) {
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                                seh_me.ClearCommands();
                                seh_me.MoveToPosition(Mother.GetPos(),1.0f,ENTITY_MOVE_RUN,false,true);
                                MoveState=0;
                            }
                        }
                        WavedAtHero=false;
                        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            seh_me.ClearCommands();
                            CScriptGameResourceObjectMovieBase speech_movie;
                            Game->StartMovieSequence("",speech_movie);
                            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                            if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                                seh_me.Speak(Game->GetHero(),"TEXT_QST_B10_SISTER_THIS_WAY",GROUP_SELECT_NONE,false,true,false);
                                while (seh_me.IsPerformingScriptTask()) {
                                    Game->NewScriptFrame();
                                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                                }
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            MoveState=2;
                        }
                        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL,"SCRIPT_NAME_HERO"))) {
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            seh_me.ClearCommands();
                            CScriptGameResourceObjectMovieBase speech_movie;
                            Game->StartMovieSequence("",speech_movie);
                            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                            if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                                seh_me.Speak(Game->GetHero(),"TEXT_QST_B10_SISTER_ON_HIT",GROUP_SELECT_RANDOM_NO_REPEAT,false,true,false);
                                while (seh_me.IsPerformingScriptTask()) {
                                    Game->NewScriptFrame();
                                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                                }
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            MoveState=2;
                        }
                    }
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            LeadHeroToSickChild=true;
        }
    }
    Game->ClearThingHasInformation(Me);
    long ReminderIndex=10;
    for (; !false; Game->NewScriptFrame()) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (ParentClass.FinishedQuest) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                CScriptGameResourceObjectMovieBase speech_movie;
                Game->StartMovieSequence("",speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game->GetHero(),"TEXT_QST_B10_SISTER_THANKS",GROUP_SELECT_NONE,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
            }
            else if (!ParentClass.MotherIntroDone) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                CScriptGameResourceObjectMovieBase speech_movie;
                Game->StartMovieSequence("",speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game->GetHero(),"TEXT_QST_B10_SISTER_REMINDER",GROUP_SELECT_NONE,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
            }
            else {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                CCharString line;
                line="TEXT_QST_B10_SISTER_REMINDER_POSTINTRO_"+GFIntToCharString(ReminderIndex);
                if (!Game->TextEntryExists(line)) {
                    ReminderIndex=10;
                    line="TEXT_QST_B10_SISTER_REMINDER_POSTINTRO_"+GFIntToCharString(ReminderIndex);
                }
                ReminderIndex+=10;
                CScriptGameResourceObjectMovieBase speech_movie;
                Game->StartMovieSequence("",speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game->GetHero(),line,GROUP_SELECT_NONE,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
            }
        }
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL,"SCRIPT_NAME_HERO"))) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            CScriptGameResourceObjectMovieBase speech_movie;
            Game->StartMovieSequence("",speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                seh_me.Speak(Game->GetHero(),"TEXT_QST_B10_SISTER_ON_HIT",GROUP_SELECT_NONE,false,true,false);
                while (seh_me.IsPerformingScriptTask()) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
        }
    }
}
