// NScript::CV_SwordInTheStoneScript::CSwordCrowd::Main() -- a villager in the sword-in-the-stone crowd: faces the sword, complains once when the hero attacks him, tells the hero the sword legend (cutscene and quest card) or that he is trying, disperses to the exit once the hero has the sword, and on the crowd timer takes his own turn pulling at the sword.
// Grade: DIFFER(4971v5024) -- chunk 826 (7 attempts, md 381 -> 22): the region name an explicit CCharString("") (built after the new-expression, the landed RegisterMain rows' spelling) and each rand() loop count its own statement before its PlayLoopingAnimation (retail calls rand, builds the name, then pushes; chunk 812's rand lesson) fixed every frame slot; the one residue is the function-final ~CScriptThing (Sword) that retail expands at 0x00ED71C8 where this build calls 0x004AA840; a row-level inline ~CScriptThing (g5) expands it there as retail but also expands the by-value Par1 temporary's dtor inside the forced-inline CSpawnedFunc1 ctor (retail calls 0x004AA840 at 0x00ED6AA1): the documented unexplained CSpawnedFunc1 case (INLINE_DEPTH_REVISIT, 2026-11-03 section)
// Next idea: the CScriptThing-holding special-member case (CSpawnedFunc1 / CIsThingAlive) once the family has an answer; a plain-inline CSpawnedFunc1 ctor stops being inlined at all (g6, opt-in dropped)
// Leaf: ?Main@CSwordCrowd@CV_SwordInTheStoneScript@NScript@@EAEXXZ
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_ASSIGN
#define FABLE_RETAIL_CV_SWORDINTHESTONESCRIPT_ENTITIES
#include "engine/C3DVector.h"
#include "engine/CV_SwordInTheStoneScript.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/fable_script_cutscene.h"
#include "engine/fable_script_text.h"
#include "engine/CSpawnedFunc1.h"
#include "engine/CIsThingAlive.h"
#include "engine/CTimer.h"
#include "engine/TRetVoid.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum ETextGroupSelectionMethod { GROUP_SELECT_NONE=0, GROUP_SELECT_RANDOM=1, GROUP_SELECT_RANDOM_NO_REPEAT=2, GROUP_SELECT_SEQUENTIAL=3, GROUP_SELECT_FIRST_ONLY=4 };
enum EHeroAbility { HERO_ABILITY_NULL=0, HERO_ABILITY_FORCE_PUSH=1, HERO_ABILITY_TIME_SPELL=2, HERO_ABILITY_ENFLAME_SPELL=3, HERO_ABILITY_PHYSICAL_SHIELD_SPELL=4, HERO_ABILITY_TURNCOAT_SPELL=5, HERO_ABILITY_DRAIN_LIFE_SPELL=6, HERO_ABILITY_RAISE_DEAD_SPELL=7, HERO_ABILITY_BERSERK=8, HERO_ABILITY_DOUBLE_STRIKE=9, HERO_ABILITY_SUMMON_SPELL=10, HERO_ABILITY_LIGHTNING_SPELL=11, HERO_ABILITY_BATTLE_CHARGE=12, HERO_ABILITY_ASSASSIN_RUSH=13, HERO_ABILITY_HEAL_LIFE_SPELL=14, HERO_ABILITY_GHOST_SWORD_SPELL=15, HERO_ABILITY_FIREBALL_SPELL=16, HERO_ABILITY_MULTI_ARROW=17, HERO_ABILITY_DIVINE_WRATH_SPELL=18, HERO_ABILITY_UNHOLY_POWER_SPELL=19, MAX_NUMBER_OF_HERO_ABILITIES=20 };
enum EScriptEntityMoveType { ENTITY_MOVE_WALK=0, ENTITY_MOVE_RUN=1, ENTITY_MOVE_SNEAK=2 };
namespace NGlobalConsole { extern bool GDefaultToUsingPhysicsInCutsceneAnimations; }   // 0x01375748
extern "C" int __cdecl rand();
extern CGameScriptInterfaceBase* pDSTGame;
inline CTimer::CTimer() { TimerIndex = pDSTGame->RegisterTimer(); }
inline CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }
__forceinline CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex, time); return *this; }
__forceinline long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
__forceinline const C3DVector& CScriptThing::GetPos() const { if (!PImp.Data) return NullCoord; return PImp.Data->GetPos(); }
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& thing_a, const CScriptThing& thing_b, float distance);   // 0x00CBE2FF
bool __fastcall IsDistanceFromThingToPositionOver(const CScriptThing& thing, const C3DVector& pos, float distance);   // 0x00CBE45C
void __fastcall FadeOutAndKillEntity(const CScriptThing& thing, bool kill, float seconds, bool removeOnFade);   // 0x00CBE716
template<> std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>::map();
template<> std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>::~map();
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing):Thing(thing) {}
void NScript::CV_SwordInTheStoneScript::CSwordCrowd::Main()
{
    TRetVoid _Fail_Val;
    SetPredicate(CIsThingAlive(Me));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    CScriptGameResourceObjectScriptedThingBase seh_me;
    Game->SetThingHasInformation(Me,true,false,false);
    CScriptThing Sword=Game->GetThingWithScriptName("SwordOfStrength");
    Game->EntitySetFacingAngleTowardsThing(Me,Sword,true);
    CTimer AttackedTimer;
    AttackedTimer=0;
    for (; !false; Game->NewScriptFrame()) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if ((Me.MsgIsHitBy("SCRIPT_NAME_HERO") || Me.MsgIsHitByAnyAggressiveSpecialAbilityFrom("SCRIPT_NAME_HERO")) && Game->GetHealth(Me)>0.0001f) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (AttackedTimer.GetTime()==0 && !ParentClass.AttackedByHero) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                ParentClass.AttackedByHero=true;
                long conversation=Game->AddNewConversation(Me,false,false);
                Game->AddPersonToConversation(conversation,Game->GetHero());
                Game->AddLineToConversation(conversation,"TEXT_QST_062_ATTACK",false,Me,Game->GetHero());
                AttackedTimer=2;
                while (Game->IsConversationActive(conversation)) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                ParentClass.AttackedByHero=false;
            }
        }
        else if (ParentClass.SwordCutscene) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            seh_me.Release();
            while (ParentClass.SwordCutscene) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (seh_me.IsValid()) seh_me.Release();
            while (!Game->StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (!ParentClass.HeroHasBeenToldSwordLegend) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                CScriptGameResourceObjectScriptedThingBase seh_hero;
                Game->StartScriptingEntity(Game->GetHero(),seh_hero,SCRIPT_AI_PRIORITY_HIGHEST);
                if (seh_me.IsValid()) seh_me.Release();
                while (!Game->StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                CScriptGameResourceObjectMovieBase speech_movie;
                Game->StartMovieSequence("",speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap;
                mymap["HERO"]=seh_hero;
                mymap["ME"]=seh_me;
                RunCutsceneMacro("CS_SWORDINSTONE_INTRO",mymap,0,0,false,true);
                Game->GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_SWORD_IN_THE_STONE",Game->GetActiveQuestName(),true);
                ParentClass.HeroHasBeenToldSwordLegend=true;
            }
            else {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (seh_me.IsValid()) seh_me.Release();
                while (!Game->StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                CScriptGameResourceObjectMovieBase speech_movie;
                Game->StartMovieSequence("",speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game->GetHero(),"TEXT_QST_062_TRYING",GROUP_SELECT_RANDOM_NO_REPEAT,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
            }
            if (seh_me.IsValid()) seh_me.Release();
        }
        if (ParentClass.HeroHasTakenSword && !ParentClass.HeroCongratulated) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (seh_me.IsValid()) seh_me.Release();
            while (!Game->StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            ParentClass.HeroCongratulated=true;
            ParentClass.CrowdDisperse=true;
            if (seh_me.IsValid()) seh_me.Release();
        }
        if (ParentClass.CrowdDisperse) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game->ClearThingHasInformation(Me);
            CScriptThing ExitPoint=Game->GetThingWithScriptName("M_SIS_ExitPoint");
            if (seh_me.IsValid()) seh_me.Release();
            while (!Game->StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (ExitPoint.IsAlive()) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (seh_me.IsValid()) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    seh_me.MoveToPosition(ExitPoint.GetPos(),1.0f,ENTITY_MOVE_RUN,false,true);
                }
                while (!IsDistanceBetweenThingsUnder(Me,ExitPoint,2.0f)) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                FadeOutAndKillEntity(Me,true,1.0f,true);
            }
        }
        if (ParentClass.CrowdSwordTimer.GetTime()==0 && !ParentClass.CrowdSwordPullActive) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            CScriptThing SwordThing=Game->GetThingWithScriptName("SwordOfStrength");
            if (!ParentClass.CrowdAttemptingToPullSword && rand()%2==1) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                ParentClass.CrowdSwordPullActive=true;
                ParentClass.AddSpawnedFunction(new CSpawnedFunc1<CV_SwordInTheStoneScript,CScriptThing>(CCharString("ParentClass.")+CCharString("WatchForDeathDuringSwordPull"),62,&ParentClass,&CV_SwordInTheStoneScript::WatchForDeathDuringSwordPull,Me),CCharString(""));
                ParentClass.CrowdAttemptingToPullSword=true;
                Game->SetThingAsUsable(SwordThing,false);
                ParentClass.CrowdMemberPullingCurrent=Me;
                if (seh_me.IsValid()) seh_me.Release();
                while (!Game->StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                C3DVector PullPos=Game->GetThingWithScriptName("MK_SIS_HERO").GetPos();
                while (ParentClass.CrowdAttemptingToPullSword && IsDistanceFromThingToPositionOver(seh_me.GetScriptThing(),PullPos,0.1f)) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    seh_me.MoveToPosition(PullPos,0.0f,ENTITY_MOVE_WALK,false,true);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (ParentClass.CrowdAttemptingToPullSword) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    Game->EntitySetFacingAngleTowardsThing(Me,SwordThing,true);
                    Game->EntitySetAsOpinionSource(Me,"OPINION_SOURCE_SWORD_IN_THE_STONE_VILLAGER");
                    Game->SetIsPushableByHero(Me,false);
                    seh_me.ClearCommands();
                    seh_me.PlayAnimation("ST_LIFT_SWORD_TRY_INTO",false,false,true,false,NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations,false,false);
                    long count1=rand()%2+1;
                    seh_me.PlayLoopingAnimation("ST_LIFT_SWORD_TRY_LOOP",count1,false,false,true,false,NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations,false,false);
                    long count2=rand()%5+2;
                    seh_me.PlayLoopingAnimation("ST_LIFT_SWORD_TRY_REST1",count2,false,false,true,false,NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations,false,false);
                    long count3=rand()%2+1;
                    seh_me.PlayLoopingAnimation("ST_LIFT_SWORD_TRY_LOOP",count3,false,false,true,false,NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations,false,false);
                    seh_me.PlayAnimation("ST_LIFT_SWORD_TRY_REST_INTO",false,false,true,false,NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations,false,false);
                    long count4=rand()%3+3;
                    seh_me.PlayLoopingAnimation("ST_LIFT_SWORD_TRY_REST2",count4,false,false,true,false,NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations,false,false);
                    seh_me.PlayAnimation("ST_LIFT_SWORD_TRY_REST_INTO_GIVEUP",false,false,true,true,NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations,false,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    C3DVector HomePos=Me.GetHomePos();
                    while (IsDistanceFromThingToPositionOver(seh_me.GetScriptThing(),HomePos,2.1f)) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        seh_me.MoveToPosition(HomePos,0.1f,ENTITY_MOVE_WALK,false,true);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                ParentClass.ResetCrowdSwordTimer();
                if (seh_me.IsValid()) seh_me.Release();
                Game->SetIsPushableByHero(Me,true);
                Game->EntityUnsetAsOpinionSource(Me,true);
                ParentClass.KillSpawnedFunction("ParentClass.WatchForDeathDuringSwordPull",62);
            }
            ParentClass.CrowdAttemptingToPullSword=false;
            Game->SetThingAsUsable(SwordThing,true);
            ParentClass.CrowdSwordPullActive=false;
            ParentClass.CrowdMemberPullingCurrent=ParentClass.CrowdMemberPullingNull;
        }
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
}
