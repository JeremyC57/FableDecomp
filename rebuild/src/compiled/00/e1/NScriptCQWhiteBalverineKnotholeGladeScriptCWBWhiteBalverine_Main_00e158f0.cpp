// NScript::CQ_WhiteBalverineKnotholeGladeScript::CWB_WhiteBalverine::Main() -- the white balverine: at stages 2 and 4 it must be hit 10 times (a quest counter), then runs to its escape point, howls, jumps into the grass and vanishes (stage 3 / 5); at stage 6 five hits, then it runs for WB_RunPoint2 and fades out once off screen or arrived (stage 7).
// Leaf: ?Main@CWB_WhiteBalverine@CQ_WhiteBalverineKnotholeGladeScript@NScript@@EAEXXZ
#define FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_ASSIGNMENT
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CQ_WHITEBALVERINEKNOTHOLEGLADESCRIPT_ENTITIES
#include "engine/C3DVector.h"
#include "engine/CQ_WhiteBalverineKnotholeGladeScript.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CIsThingConscious.h"
#include "engine/CTimer.h"
#include "engine/TRetVoid.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum EHeroAbility { HERO_ABILITY_NULL=0, HERO_ABILITY_FORCE_PUSH=1, HERO_ABILITY_TIME_SPELL=2, HERO_ABILITY_ENFLAME_SPELL=3, HERO_ABILITY_PHYSICAL_SHIELD_SPELL=4, HERO_ABILITY_TURNCOAT_SPELL=5, HERO_ABILITY_DRAIN_LIFE_SPELL=6, HERO_ABILITY_RAISE_DEAD_SPELL=7, HERO_ABILITY_BERSERK=8, HERO_ABILITY_DOUBLE_STRIKE=9, HERO_ABILITY_SUMMON_SPELL=10, HERO_ABILITY_LIGHTNING_SPELL=11, HERO_ABILITY_BATTLE_CHARGE=12, HERO_ABILITY_ASSASSIN_RUSH=13, HERO_ABILITY_HEAL_LIFE_SPELL=14, HERO_ABILITY_GHOST_SWORD_SPELL=15, HERO_ABILITY_FIREBALL_SPELL=16, HERO_ABILITY_MULTI_ARROW=17, HERO_ABILITY_DIVINE_WRATH_SPELL=18, HERO_ABILITY_UNHOLY_POWER_SPELL=19, MAX_NUMBER_OF_HERO_ABILITIES=20 };
enum EScriptEntityMoveType { ENTITY_MOVE_WALK=0, ENTITY_MOVE_RUN=1, ENTITY_MOVE_SNEAK=2 };
namespace NGlobalConsole { extern bool GDefaultToUsingPhysicsInCutsceneAnimations; }   // 0x01375748
extern CGameScriptInterfaceBase* pDSTGame;
#pragma auto_inline(off)
__declspec(noinline) inline CTimer::CTimer() { TimerIndex = pDSTGame->RegisterTimer(); }
__declspec(noinline) inline CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }
#pragma auto_inline(on)
__forceinline CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex, time); return *this; }
__forceinline long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
bool __fastcall IsDistanceFromThingToPositionOver(const CScriptThing& thing, const C3DVector& pos, float distance);   // 0x00CBE45C
void __fastcall FadeOutAndKillEntity(const CScriptThing& thing, bool kill, float seconds, bool removeOnFade);   // 0x00CBE716
inline NScript::CIsThingConscious::CIsThingConscious(const CScriptThing& thing):Thing(thing) {}
inline NScript::CIsThingConscious::~CIsThingConscious() {}
void NScript::CQ_WhiteBalverineKnotholeGladeScript::CWB_WhiteBalverine::Main()
{
    TRetVoid _Fail_Val;
    SetPredicate(CIsThingConscious(Me));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    CScriptGameResourceObjectScriptedThingBase seh_me;
    Game->MiniMapAddMarker(Me,"HUD_ORB_QUEST_CORE");
    Game->EntitySetAsKillable(Me,false,true);
    Game->EntitySetAsDamageable(Me,false);
    Game->OpinionSourceSetAsExclusive(Me,true);
    Game->EntitySetTargetable(Me,true);
    for (; !false; Game->NewScriptFrame()) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (ParentClass.BalverineState==2) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game->SetThingPersistent(Me,true);
            Game->GiveThingBestEnemyTarget(Me,Game->GetHero());
            long hits=0;
            WBCounter=Game->AddQuestInfoCounter("HUD_QUEST_ICON_WHITE_BALVERINE",10,1.0f);
            Game->DisplayQuestInfo(true);
            do {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL,"SCRIPT_NAME_HERO"))) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    hits=hits+1;
                    Game->UpdateQuestInfoCounter(WBCounter,hits,-1);
                }
            } while (hits<10);
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game->RemoveQuestInfoElement(WBCounter);
            Game->DisplayQuestInfo(false);
            Game->EntitySetTargetable(Me,false);
            Game->EntitySetCombatType(Me,"WHITE_BALVERINE_BLOCK_ALWAYS_ATTACK_STYLE");
            if (seh_me.IsValid()) seh_me.Release();
            while (!Game->StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            C3DVector EscapePos=Game->GetThingWithScriptName("WB_EscapePoint1").GetPos();
            while (IsDistanceFromThingToPositionOver(seh_me.GetScriptThing(),EscapePos,2.5f)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                seh_me.MoveToPosition(EscapePos,0.5f,ENTITY_MOVE_RUN,false,false);
                while (seh_me.IsPerformingScriptTask()) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            seh_me.PlayAnimation("HOWL",false,false,false,true,NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations,false,false);
            while (seh_me.IsPerformingScriptTask()) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            seh_me.PlayAnimation("ST_RUN_JUMP",false,false,false,true,NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations,false,false);
            Game->Pause(1.0f);
            Game->CreateEffect("EFFECT_GRASS_CLUMPS",Me.GetPos(),"WB_GrassClump",0.0f,false,false);
            Game->RemoveThing(Me,false,true);
            ParentClass.BalverineState=3;
        }
        if (ParentClass.BalverineState==4) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game->SetThingPersistent(Me,true);
            Game->GiveThingBestEnemyTarget(Me,Game->GetHero());
            Game->EntitySetCombatType(Me,"WHITE_BALVERINE_ATTACK_STYLE");
            long hits=0;
            WBCounter=Game->AddQuestInfoCounter("HUD_QUEST_ICON_WHITE_BALVERINE",10,1.0f);
            Game->DisplayQuestInfo(true);
            do {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL,"SCRIPT_NAME_HERO"))) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    hits=hits+1;
                    Game->UpdateQuestInfoCounter(WBCounter,hits,-1);
                }
            } while (hits<10);
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game->RemoveQuestInfoElement(WBCounter);
            Game->DisplayQuestInfo(false);
            Game->EntitySetTargetable(Me,false);
            Game->EntitySetCombatType(Me,"WHITE_BALVERINE_BLOCK_ALWAYS_ATTACK_STYLE");
            if (seh_me.IsValid()) seh_me.Release();
            while (!Game->StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            C3DVector EscapePos=Game->GetThingWithScriptName("WB_EscapePoint2").GetPos();
            while (IsDistanceFromThingToPositionOver(seh_me.GetScriptThing(),EscapePos,2.5f)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                seh_me.MoveToPosition(EscapePos,0.5f,ENTITY_MOVE_RUN,false,false);
                while (seh_me.IsPerformingScriptTask()) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            seh_me.PlayAnimation("HOWL",false,false,false,true,NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations,false,false);
            while (seh_me.IsPerformingScriptTask()) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            seh_me.PlayAnimation("ST_RUN_JUMP",false,false,false,true,NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations,false,false);
            Game->Pause(1.0f);
            Game->CreateEffect("EFFECT_GRASS_CLUMPS",Me.GetPos(),"WB_GrassClump",0.0f,false,false);
            Game->RemoveThing(Me,false,true);
            ParentClass.BalverineState=5;
        }
        if (ParentClass.BalverineState==6) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            CScriptThing Guard=Game->GetThingWithScriptName("WB_Guard1");
            if (seh_me.IsValid()) seh_me.Release();
            while (!Game->StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game->EntitySetCombatType(Me,"WHITE_BALVERINE_ATTACK_STYLE");
            if (seh_me.IsValid()) seh_me.Release();
            Game->SetThingPersistent(Me,true);
            Game->GiveThingBestEnemyTarget(Me,Game->GetHero());
            long hits=0;
            WBCounter=Game->AddQuestInfoCounter("HUD_QUEST_ICON_WHITE_BALVERINE",5,1.0f);
            Game->DisplayQuestInfo(true);
            do {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL,"SCRIPT_NAME_HERO"))) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    hits=hits+1;
                    Game->UpdateQuestInfoCounter(WBCounter,hits,-1);
                }
            } while (hits<5);
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game->RemoveQuestInfoElement(WBCounter);
            Game->DisplayQuestInfo(false);
            if (seh_me.IsValid()) seh_me.Release();
            while (!Game->StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            CScriptThing RunPoint=Game->GetThingWithScriptName("WB_RunPoint2");
            CTimer RunTimer;
            RunTimer=5;
            while (RunTimer.GetTime()>0) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (!seh_me.IsPerformingScriptTask()) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    seh_me.MoveToThing(RunPoint,0.0f,ENTITY_MOVE_RUN,false,false,false,true);
                }
                if (!Game->IsCameraPosOnScreen(Me.GetPos())) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    ParentClass.BalverineState=7;
                    FadeOutAndKillEntity(Me,true,3.0f,true);
                } else if (!seh_me.IsPerformingScriptTask()) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    ParentClass.BalverineState=7;
                    FadeOutAndKillEntity(Me,true,3.0f,true);
                }
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            ParentClass.BalverineState=7;
            FadeOutAndKillEntity(Me,true,3.0f,true);
        }
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
}