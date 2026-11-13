// NScript::CQ_BreakSiegeScript::CImprisonedBanditLeader::Main() -- the captured bandit leader in Break the Siege: live while alive; take scripted control, make him an evil opinion source and pick a random RestlessMarker; until he is freed or the end cut-scene is done: curse at the hero when talked to or hit (the guard answers TEXT_QST_018_GUARD_STOP_THAT), and every 10 s walk to a new random marker; when freed run (horn KNG_HORN if the horn marker is off-screen and far) to MB_OutsideBanditCamp as a monster, or wait out the end cut-scene and go home; then fail the mission (reason 3) or remove himself
// Grade: DIFFER(2742v2758) -- 40 masked difflines, one cause: the CScriptThing copy split (GOTCHAS inline revisit 3). Retail expands the by-value DestMarker copy (one CScriptThing vtable store, PImp copy, inc) but calls the copy 0x004ABE90 inside the expanded CIsThingAlive ctor; with the copy declared (this source) the predicate matches and the DestMarker copy is a call (+ condition-mask / Guard frame slots 0xc lower, the reassignment's `+4` folded into the operator= argument).
// Next idea: none in plain C++ known; chunk 836 h2 (FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_COPY / _IMPLICIT_ASSIGNMENT + declared-only CCountedPointer<CScriptThing>::operator= 0x00704580) makes DestMarker and its reassignment byte-exact but expands the predicate's nested copy too (md63); the chunk 822 / /d2inlT55-65 family; a named filter block (h3) and a row-local plain-inline copy ctor (h4) are identical to h2
// Leaf: ?Main@CImprisonedBanditLeader@CQ_BreakSiegeScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D21040, 2758 bytes, thiscall, plain `ret`, frame 0x84.
// Identity (proven): Ego_r.pdb ?Main@CImprisonedBanditLeader@CQ_BreakSiegeScript@NScript@@EAEXXZ; slot 1 of the CImprisonedBanditLeader
//   entity vtable.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#include <stdlib.h>
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/C3DVector.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CQ_BreakSiegeScript.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CIsThingAlive.h"
#include "engine/CTimer.h"
#include "engine/TRetVoid.h"
#include "engine/fable_script_distance.h"
#include "stlport/fable_containers.h"
template<> std::vector<CScriptThing>::~vector();              // 0x008AC970
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum EScriptEntityMoveType { ENTITY_MOVE_WALK=0, ENTITY_MOVE_RUN=1, ENTITY_MOVE_SNEAK=2 };
enum EHeroAbility { HERO_ABILITY_NULL=0, HERO_ABILITY_FORCE_PUSH=1, HERO_ABILITY_TIME_SPELL=2, HERO_ABILITY_ENFLAME_SPELL=3, HERO_ABILITY_PHYSICAL_SHIELD_SPELL=4, HERO_ABILITY_TURNCOAT_SPELL=5, HERO_ABILITY_DRAIN_LIFE_SPELL=6, HERO_ABILITY_RAISE_DEAD_SPELL=7, HERO_ABILITY_BERSERK=8, HERO_ABILITY_DOUBLE_STRIKE=9, HERO_ABILITY_SUMMON_SPELL=10, HERO_ABILITY_LIGHTNING_SPELL=11, HERO_ABILITY_BATTLE_CHARGE=12, HERO_ABILITY_ASSASSIN_RUSH=13, HERO_ABILITY_HEAL_LIFE_SPELL=14, HERO_ABILITY_GHOST_SWORD_SPELL=15, HERO_ABILITY_FIREBALL_SPELL=16, HERO_ABILITY_MULTI_ARROW=17, HERO_ABILITY_DIVINE_WRATH_SPELL=18, HERO_ABILITY_UNHOLY_POWER_SPELL=19, MAX_NUMBER_OF_HERO_ABILITIES=20 };
extern CGameScriptInterfaceBase* pDSTGame;               // 0x0143E8F8
inline CTimer::CTimer() : TimerIndex(pDSTGame->RegisterTimer()) { }
inline CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }
__forceinline CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex, time); return *this; }
__forceinline long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing) : Thing(thing) { }
inline NScript::CIsThingAlive::~CIsThingAlive() { }
void NScript::CQ_BreakSiegeScript::CImprisonedBanditLeader::Main()
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
    Game->EntitySetAsOpinionSource(Me,"OPINION_SOURCE_INANIMATE_EVIL_HIGH");
    std::vector<CScriptThing> RestlessMarkers;
    Game->GetAllThingsWithScriptName("RestlessMarker",RestlessMarkers);
    CScriptThing DestMarker=RestlessMarkers[rand()%RestlessMarkers.size()];
    CTimer RestlessFrequency;
    RestlessFrequency=5;
    while (!ParentClass.BanditLeaderFree && !ParentClass.EndCutsceneDone) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game->EntitySetFacingAngleTowardsThing(Me,Game->GetHero(),false);
            long HLStyleConversationIndex=Game->AddNewConversation(Me,false,false);
            CScriptThing Guard=Game->GetNearestWithScriptName(Me,"GuardHoldingBandit");
            Game->AddPersonToConversation(HLStyleConversationIndex,Guard);
            Game->AddPersonToConversation(HLStyleConversationIndex,Game->GetHero());
            Game->AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_018_PRISONER_CURSES_AT_YOU",false,Me,Game->GetHero());
            Game->AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_018_GUARD_STOP_THAT",false,Guard,Game->GetHero());
            while (!Game->IsConversationActive(HLStyleConversationIndex)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL,"SCRIPT_NAME_HERO"))) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game->EntitySetFacingAngleTowardsThing(Me,Game->GetHero(),false);
            long HLStyleConversationIndex=Game->AddNewConversation(Me,false,false);
            CScriptThing Guard=Game->GetNearestWithScriptName(Me,"GuardHoldingBandit");
            Game->AddPersonToConversation(HLStyleConversationIndex,Guard);
            Game->AddPersonToConversation(HLStyleConversationIndex,Game->GetHero());
            Game->AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_018_PRISONER_CURSES_AT_YOU_ON_HIT",false,Me,Game->GetHero());
            Game->AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_018_GUARD_STOP_THAT",false,Guard,Game->GetHero());
            while (!Game->IsConversationActive(HLStyleConversationIndex)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (!seh_me.IsPerformingScriptTask() && RestlessFrequency.GetTime()==0) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            seh_me.MoveToThing(DestMarker,1.0f,ENTITY_MOVE_WALK,false,true,false,true);
            DestMarker=RestlessMarkers[rand()%RestlessMarkers.size()];
            RestlessFrequency=10;
        }
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    Game->MiniMapRemoveMarker(Me);
    if (!ParentClass.EndCutsceneDone) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game->MiniMapAddMarker(Me,"HUD_ORB_QUEST_OPTIONAL");
        if (seh_me.IsValid()) seh_me.Release();
        while (!Game->StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game->EntitySetInFaction(Me,"FACTION_MONSTERS");
        CScriptThing MB_OutsideCamp=Game->GetThingWithScriptName("MB_OutsideBanditCamp");
        if (IsDistanceBetweenThingsOver(Game->GetThingWithScriptName("HornBlowMarker"),Game->GetHero(),10.0f) && !Game->IsCameraPosOnScreen(Game->GetThingWithScriptName("HornBlowMarker").GetPos())) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game->PlayCriteriaSoundOnThing(Me,"KNG_HORN");
        }
        while (IsDistanceBetweenThingsOver(Me,MB_OutsideCamp,2.0f)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (!ParentClass.EndCutsceneStarts) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (!seh_me.IsPerformingScriptTask()) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    seh_me.MoveToThing(MB_OutsideCamp,0.0f,ENTITY_MOVE_RUN,false,true,false,true);
                }
            } else {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                seh_me.ClearCommands();
                Game->EntitySetInFaction(Me,"FACTION_NEUTRALS");
                while (!ParentClass.EndCutsceneDone) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game->EntityTeleportToPosition(Me,Me.GetHomePos(),0.0f,false,false);
                break;
            }
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (!ParentClass.EndCutsceneStarts) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        ParentClass.ReasonForFailure=3;
        if (!ParentClass.MissionSucceeded) ParentClass.MissionFailed=true;
    } else {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game->MiniMapRemoveMarker(Me);
        Game->RemoveThing(Me,false,true);
    }
}
