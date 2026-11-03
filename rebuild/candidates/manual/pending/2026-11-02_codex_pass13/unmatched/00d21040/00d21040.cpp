// NScript::CQ_BreakSiegeScript::CImprisonedBanditLeader::Main() -- pace in custody, respond to the hero and attempt escape.
// Leaf: ?Main@CImprisonedBanditLeader@CQ_BreakSiegeScript@NScript@@EAEXXZ
// Grade: DIFFER(2742v2758) -- DestMarker copy remains called; condition mask slot and one loop jump differ.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CENTITYSCRIPTBASE
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CSCRIPTTHING
#define FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_ASSIGNMENT
#define FABLE_RETAIL_CISTHINGALIVE
#define FABLE_RETAIL_CTIMER
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_DECLARED_DTOR
#include <stdlib.h>
#include "engine/CCountedPointer.h"
class CScriptThing;
template<> __declspec(noinline) CCountedPointer<CScriptThing>& CCountedPointer<CScriptThing>::operator=(const CCountedPointer<CScriptThing>& other) { ShareData(other.Data,other.Info); return *this; }
#include "engine/CQ_BreakSiegeScript.h"
#include "engine/CIsThingAlive.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/C3DVector.h"
#include "engine/TRetVoid.h"
#include "stlport/fable_containers.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum EScriptEntityMoveType { ENTITY_MOVE_WALK=0, ENTITY_MOVE_RUN=1, ENTITY_MOVE_SNEAK=2 };
enum EHeroAbility { HERO_ABILITY_NULL=0, HERO_ABILITY_FORCE_PUSH=1, HERO_ABILITY_TIME_SPELL=2, HERO_ABILITY_ENFLAME_SPELL=3, HERO_ABILITY_PHYSICAL_SHIELD_SPELL=4, HERO_ABILITY_TURNCOAT_SPELL=5, HERO_ABILITY_DRAIN_LIFE_SPELL=6, HERO_ABILITY_RAISE_DEAD_SPELL=7, HERO_ABILITY_BERSERK=8, HERO_ABILITY_DOUBLE_STRIKE=9, HERO_ABILITY_SUMMON_SPELL=10, HERO_ABILITY_LIGHTNING_SPELL=11, HERO_ABILITY_BATTLE_CHARGE=12, HERO_ABILITY_ASSASSIN_RUSH=13, HERO_ABILITY_HEAL_LIFE_SPELL=14, HERO_ABILITY_GHOST_SWORD_SPELL=15, HERO_ABILITY_FIREBALL_SPELL=16, HERO_ABILITY_MULTI_ARROW=17, HERO_ABILITY_DIVINE_WRATH_SPELL=18, HERO_ABILITY_UNHOLY_POWER_SPELL=19, MAX_NUMBER_OF_HERO_ABILITIES=20 };
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing):Thing(thing) {}
inline NScript::CIsThingAlive::~CIsThingAlive() {}
extern CGameScriptInterfaceBase* pDSTGame;
bool FABLE_FASTCALL IsDistanceBetweenThingsOver(const CScriptThing&,const CScriptThing&,float);
template<> std::vector<CScriptThing>::~vector();
inline CTimer::CTimer():TimerIndex(pDSTGame->RegisterTimer()) {}
inline CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }
inline CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex,time); return *this; }
inline long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
inline bool CTimer::operator==(long time) const { return GetTime()==time; }
void NScript::CQ_BreakSiegeScript::CImprisonedBanditLeader::Main()
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
    Game.EntitySetAsOpinionSource(Me,"OPINION_SOURCE_INANIMATE_EVIL_HIGH");
    std::vector<CScriptThing> RestlessMarkers;
    Game.GetAllThingsWithScriptName("RestlessMarker",RestlessMarkers);
    CScriptThing DestMarker=RestlessMarkers[rand()%RestlessMarkers.size()];
    CTimer RestlessFrequency;
    RestlessFrequency=5;
    while (!ParentClass.BanditLeaderFree && !ParentClass.EndCutsceneDone) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game.EntitySetFacingAngleTowardsThing(Me,Game.GetHero(),false);
            long HLStyleConversationIndex=Game.AddNewConversation(Me,false,false);
            CScriptThing Guard=Game.GetNearestWithScriptName(Me,"GuardHoldingBandit");
            Game.AddPersonToConversation(HLStyleConversationIndex,Guard);
            Game.AddPersonToConversation(HLStyleConversationIndex,Game.GetHero());
            Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_018_PRISONER_CURSES_AT_YOU",false,Me,Game.GetHero());
            Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_018_GUARD_STOP_THAT",false,Guard,Game.GetHero());
            while (!Game.IsConversationActive(HLStyleConversationIndex)) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL,"SCRIPT_NAME_HERO"))) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game.EntitySetFacingAngleTowardsThing(Me,Game.GetHero(),false);
            long HLStyleConversationIndex=Game.AddNewConversation(Me,false,false);
            CScriptThing Guard=Game.GetNearestWithScriptName(Me,"GuardHoldingBandit");
            Game.AddPersonToConversation(HLStyleConversationIndex,Guard);
            Game.AddPersonToConversation(HLStyleConversationIndex,Game.GetHero());
            Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_018_PRISONER_CURSES_AT_YOU_ON_HIT",false,Me,Game.GetHero());
            Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_018_GUARD_STOP_THAT",false,Guard,Game.GetHero());
            while (!Game.IsConversationActive(HLStyleConversationIndex)) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (!seh_me.IsPerformingScriptTask() && RestlessFrequency==0) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            seh_me.MoveToThing(DestMarker,1.0f,ENTITY_MOVE_WALK,false,true,false,true);
            DestMarker=RestlessMarkers[rand()%RestlessMarkers.size()];
            RestlessFrequency=10;
        }
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    Game.MiniMapRemoveMarker(Me);
    if (!ParentClass.EndCutsceneDone) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game.MiniMapAddMarker(Me,"HUD_ORB_QUEST_OPTIONAL");
        if (seh_me.IsValid()) seh_me.Release();
        while (!Game.StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
            Game.NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game.EntitySetInFaction(Me,"FACTION_MONSTERS");
        CScriptThing MB_OutsideCamp=Game.GetThingWithScriptName("MB_OutsideBanditCamp");
        if (IsDistanceBetweenThingsOver(Game.GetThingWithScriptName("HornBlowMarker"),Game.GetHero(),10.0f) && !Game.IsCameraPosOnScreen(Game.GetThingWithScriptName("HornBlowMarker").GetPos())) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game.PlayCriteriaSoundOnThing(Me,"KNG_HORN");
        }
        while (IsDistanceBetweenThingsOver(Me,MB_OutsideCamp,2.0f)) {
            Game.NewScriptFrame();
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
                Game.EntitySetInFaction(Me,"FACTION_NEUTRALS");
                while (!ParentClass.EndCutsceneDone) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game.EntityTeleportToPosition(Me,Me.GetHomePos(),0.0f,false,false);
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
        Game.MiniMapRemoveMarker(Me);
        Game.RemoveThing(Me,false,true);
    }
}
