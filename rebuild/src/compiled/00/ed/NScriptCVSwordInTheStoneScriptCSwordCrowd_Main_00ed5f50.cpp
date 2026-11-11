// NScript::CV_SwordInTheStoneScript::CSwordCrowd::Main() -- a villager in the sword-in-the-stone crowd: faces the sword, complains once when the hero attacks him, tells the hero the sword legend (cutscene and quest card) or that he is trying, disperses to the exit once the hero has the sword, and on the crowd timer takes his own turn pulling at the sword.
// Grade: RELOCATION_MATCH -- actual COFF operands checked against their native targets.
// Inline-budget: explicit spawned-thread constructor specialization keeps nested argument cleanup called at 0x00ED6AA1 while the final Sword cleanup expands at 0x00ED71C8; prior hand826 plain and budget forms failed.
// Leaf: ?Main@CSwordCrowd@CV_SwordInTheStoneScript@NScript@@EAEXXZ
// Retail: 0x00ED5F50, 5024 bytes, thiscall.
#define FABLE_RETAIL_CLEVELSCRIPTBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_CISTHINGALIVE_ORIGINAL_ACCESS
#define FABLE_RETAIL_CENTITYSCRIPTBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_CPREDICATEBASE_ORIGINAL_MEMBERS
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
// Original data symbol and native fcomp operand0x0129BA3C; ESTABLISHED_SPELLINGS.
extern const float DELTA_NOTIONAL_ZERO;
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
#include "engine/ETextGroupSelectionMethod.h"
#include "engine/EHeroAbility.h"
enum EScriptEntityMoveType { ENTITY_MOVE_WALK=0, ENTITY_MOVE_RUN=1, ENTITY_MOVE_SNEAK=2 };
namespace NGlobalConsole { extern bool GDefaultToUsingPhysicsInCutsceneAnimations; }   // 0x01375748
extern "C" int FABLE_CDECL rand();
extern CGameScriptInterfaceBase* pDSTGame;
inline CTimer::CTimer() { TimerIndex = pDSTGame->RegisterTimer(); }
inline CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }
FABLE_FORCEINLINE CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex, time); return *this; }
FABLE_FORCEINLINE long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
FABLE_FORCEINLINE const C3DVector& CScriptThing::GetPos() const { if (!PImp.Data) return NullCoord; return PImp.Data->GetPos(); }
bool FABLE_FASTCALL IsDistanceBetweenThingsUnder(const CScriptThing& thing_a, const CScriptThing& thing_b, float distance);   // 0x00CBE2FF
bool FABLE_FASTCALL IsDistanceFromThingToPositionOver(const CScriptThing& thing, const C3DVector& pos, float distance);   // 0x00CBE45C
#include "engine/fable_script_fade.h"
template<> std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>::map();
template<> std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>::~map();
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing):Thing(thing) {}
inline CScriptThing::~CScriptThing() { }
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
template<> FABLE_FORCEINLINE CSpawnedFunc1<NScript::CV_SwordInTheStoneScript,CScriptThing>::CSpawnedFunc1(const CCharString& name, unsigned long id, NScript::CV_SwordInTheStoneScript* pclass, void (NScript::CV_SwordInTheStoneScript::*func)(CScriptThing), CScriptThing par1)
    : CSpawnedFuncBase(name,id), Func(func), PClass(pclass), Par1(par1) { }
#ifdef _MSC_VER
#pragma inline_depth()
#endif
void NScript::CV_SwordInTheStoneScript::CSwordCrowd::Main()
{
    TRetVoid _Fail_Val;
    SetPredicate(CIsThingAlive(Me));
    Game.NewScriptFrame();
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    CScriptGameResourceObjectScriptedThingBase seh_me;
    Game.SetThingHasInformation(Me,true,false,false);
    CScriptThing Sword=Game.GetThingWithScriptName("SwordOfStrength");
    Game.EntitySetFacingAngleTowardsThing(Me,Sword,true);
    CTimer AttackedTimer;
    AttackedTimer=0;
    for (; !false; Game.NewScriptFrame()) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if ((Me.MsgIsHitBy("SCRIPT_NAME_HERO") || Me.MsgIsHitByAnyAggressiveSpecialAbilityFrom("SCRIPT_NAME_HERO")) && Game.GetHealth(Me)>DELTA_NOTIONAL_ZERO) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (AttackedTimer.GetTime()==0 && !ParentClass.AttackedByHero) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                ParentClass.AttackedByHero=true;
                long conversation=Game.AddNewConversation(Me,false,false);
                Game.AddPersonToConversation(conversation,Game.GetHero());
                Game.AddLineToConversation(conversation,"TEXT_QST_062_ATTACK",false,Me,Game.GetHero());
                AttackedTimer=2;
                while (Game.IsConversationActive(conversation)) {
                    Game.NewScriptFrame();
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
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (seh_me.IsValid()) seh_me.Release();
            while (!Game.StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (!ParentClass.HeroHasBeenToldSwordLegend) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                CScriptGameResourceObjectScriptedThingBase seh_hero;
                Game.StartScriptingEntity(Game.GetHero(),seh_hero,SCRIPT_AI_PRIORITY_HIGHEST);
                if (seh_me.IsValid()) seh_me.Release();
                while (!Game.StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                CScriptGameResourceObjectMovieBase speech_movie;
                Game.StartMovieSequence("",speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap;
                mymap["HERO"]=seh_hero;
                mymap["ME"]=seh_me;
                RunCutsceneMacro("CS_SWORDINSTONE_INTRO",mymap,0,0,false,true);
                Game.GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_SWORD_IN_THE_STONE",Game.GetActiveQuestName(),true);
                ParentClass.HeroHasBeenToldSwordLegend=true;
            }
            else {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (seh_me.IsValid()) seh_me.Release();
                while (!Game.StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                CScriptGameResourceObjectMovieBase speech_movie;
                Game.StartMovieSequence("",speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game.GetHero(),"TEXT_QST_062_TRYING",GROUP_SELECT_RANDOM_NO_REPEAT,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
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
            while (!Game.StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            ParentClass.HeroCongratulated=true;
            ParentClass.CrowdDisperse=true;
            if (seh_me.IsValid()) seh_me.Release();
        }
        if (ParentClass.CrowdDisperse) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game.ClearThingHasInformation(Me);
            CScriptThing ExitPoint=Game.GetThingWithScriptName("M_SIS_ExitPoint");
            if (seh_me.IsValid()) seh_me.Release();
            while (!Game.StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game.NewScriptFrame();
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
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                FadeOutAndKillEntity(Me,1.0f,true,true);
            }
        }
        if (ParentClass.CrowdSwordTimer.GetTime()==0 && !ParentClass.CrowdSwordPullActive) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            CScriptThing SwordThing=Game.GetThingWithScriptName("SwordOfStrength");
            if (!ParentClass.CrowdAttemptingToPullSword && rand()%2==1) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                ParentClass.CrowdSwordPullActive=true;
                ParentClass.AddSpawnedFunction(new CSpawnedFunc1<CV_SwordInTheStoneScript,CScriptThing>(CCharString("ParentClass.")+CCharString("WatchForDeathDuringSwordPull"),62,&ParentClass,&CV_SwordInTheStoneScript::WatchForDeathDuringSwordPull,Me),CCharString(""));
                ParentClass.CrowdAttemptingToPullSword=true;
                Game.SetThingAsUsable(SwordThing,false);
                ParentClass.CrowdMemberPullingCurrent=Me;
                if (seh_me.IsValid()) seh_me.Release();
                while (!Game.StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                C3DVector PullPos=Game.GetThingWithScriptName("MK_SIS_HERO").GetPos();
                while (ParentClass.CrowdAttemptingToPullSword && IsDistanceFromThingToPositionOver(seh_me.GetScriptThing(),PullPos,0.1f)) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    seh_me.MoveToPosition(PullPos,0.0f,ENTITY_MOVE_WALK,false,true);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (ParentClass.CrowdAttemptingToPullSword) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    Game.EntitySetFacingAngleTowardsThing(Me,SwordThing,true);
                    Game.EntitySetAsOpinionSource(Me,"OPINION_SOURCE_SWORD_IN_THE_STONE_VILLAGER");
                    Game.SetIsPushableByHero(Me,false);
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
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    C3DVector HomePos=Me.GetHomePos();
                    while (IsDistanceFromThingToPositionOver(seh_me.GetScriptThing(),HomePos,2.1f)) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        seh_me.MoveToPosition(HomePos,0.1f,ENTITY_MOVE_WALK,false,true);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game.NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                ParentClass.ResetCrowdSwordTimer();
                if (seh_me.IsValid()) seh_me.Release();
                Game.SetIsPushableByHero(Me,true);
                Game.EntityUnsetAsOpinionSource(Me,true);
                ParentClass.KillSpawnedFunction("ParentClass.WatchForDeathDuringSwordPull",62);
            }
            ParentClass.CrowdAttemptingToPullSword=false;
            Game.SetThingAsUsable(SwordThing,true);
            ParentClass.CrowdSwordPullActive=false;
            ParentClass.CrowdMemberPullingCurrent=ParentClass.CrowdMemberPullingNull;
        }
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
}