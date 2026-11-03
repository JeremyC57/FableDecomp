// NScript::CQ_ArenaScript::CArenaCellDoorGuard2::Main() -- guard the cell door, pause for arena events and respond to the hero.
// Leaf: ?Main@CArenaCellDoorGuard2@CQ_ArenaScript@NScript@@EAEXXZ
#define FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_ASSIGNMENT
#define FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_COPY
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#include "engine/C3DVector.h"
#include "engine/CQ_ArenaScript.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/TRetVoid.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum ETextGroupSelectionMethod { GROUP_SELECT_NONE=0, GROUP_SELECT_RANDOM=1, GROUP_SELECT_RANDOM_NO_REPEAT=2, GROUP_SELECT_SEQUENTIAL=3, GROUP_SELECT_FIRST_ONLY=4 };
enum EHeroAbility { HERO_ABILITY_NULL=0, HERO_ABILITY_FORCE_PUSH=1, HERO_ABILITY_TIME_SPELL=2, HERO_ABILITY_ENFLAME_SPELL=3, HERO_ABILITY_PHYSICAL_SHIELD_SPELL=4, HERO_ABILITY_TURNCOAT_SPELL=5, HERO_ABILITY_DRAIN_LIFE_SPELL=6, HERO_ABILITY_RAISE_DEAD_SPELL=7, HERO_ABILITY_BERSERK=8, HERO_ABILITY_DOUBLE_STRIKE=9, HERO_ABILITY_SUMMON_SPELL=10, HERO_ABILITY_LIGHTNING_SPELL=11, HERO_ABILITY_BATTLE_CHARGE=12, HERO_ABILITY_ASSASSIN_RUSH=13, HERO_ABILITY_HEAL_LIFE_SPELL=14, HERO_ABILITY_GHOST_SWORD_SPELL=15, HERO_ABILITY_FIREBALL_SPELL=16, HERO_ABILITY_MULTI_ARROW=17, HERO_ABILITY_DIVINE_WRATH_SPELL=18, HERO_ABILITY_UNHOLY_POWER_SPELL=19, MAX_NUMBER_OF_HERO_ABILITIES=20 };
template<> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();
inline CScriptThing::~CScriptThing() {}
enum EScriptingStateGroups { ESSG_NONE=0, ESSG_PERFORM_ACTION_PHYSICAL=1, ESSG_PERFORM_ACTION_VERBAL=2, ESSG_PERFORM_ACTION_AURAL=3, ESSG_WANDER_NEAR=4, ESSG_FOLLOW_PATH=5, ESSG_FOLLOW_RANDOM=6, ESSG_FOLLOW_NEAREST=7, ESSG_WALK_TO_RANDOM=8, ESSG_WALK_TO_NEAREST_DIFFERENT=9, ESSG_RUN_AT_HERO_AND_ATTACK_UNTIL_DEAD=10 };
enum EScriptEntityMoveType { ENTITY_MOVE_WALK=0, ENTITY_MOVE_RUN=1, ENTITY_MOVE_SNEAK=2 };
bool __fastcall IsDistanceBetweenThingsOver(const CScriptThing&,const CScriptThing&,float);
void NScript::CQ_ArenaScript::CArenaCellDoorGuard2::Main()
{
    TRetVoid _Fail_Val;
    CScriptGameResourceObjectScriptedThingBase seh_me;
    ParentClass.CellsVillage=Game->GetNearestWithDefName(Me,"VILLAGE_ARENA_CELLS");
    Game->EntitySetOpinionReactionsEnabled(Me,false);
    Game->EntitySetInFaction(Me,"FACTION_HERO");
    Game->EntitySetAsKillable(Me,false,false);
    Game->EntitySetAsToAddToComboMultiplierWhenHit(Me,false);
    while (ParentClass.ArenaState==MAS_CELLS_PRE_ARENA_INTRO) {
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
    bool Turned=false;
    for (; !false; Game->NewScriptFrame()) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (ParentClass.ArenaState==MAS_CELLS_PRE_ARENA_CHAM_CUTSCENE) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game->ClearHeroEnemyOfGuards(ParentClass.CellsVillage);
            seh_me.ClearCommands();
            seh_me.Release();
            while (ParentClass.ArenaState==MAS_CELLS_PRE_ARENA_CHAM_CUTSCENE) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game->ClearHeroEnemyOfGuards(ParentClass.CellsVillage);
            if (seh_me.IsValid()) seh_me.Release();
            while (!Game->StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            seh_me.ClearCommands();
            seh_me.MoveToPosition(Game->GetThingWithScriptName("GuardingDoorMarkerLeft").GetPos(),0.0f,ENTITY_MOVE_RUN,false,true);
            Turned=false;
        }
        if (ParentClass.NeedBertForSpeech) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            Game->ClearHeroEnemyOfGuards(ParentClass.CellsVillage);
            seh_me.ClearCommands();
            seh_me.Release();
            while (ParentClass.NeedBertForSpeech) {
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
            if (seh_me.IsPerformingScriptTask()) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                seh_me.ClearCommands();
            }
            CScriptGameResourceObjectMovieBase speech_movie;
            Game->StartMovieSequence("",speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            if (ParentClass.ArenaState==MAS_CELLS_PRE_ARENA_NORMAL && !EarlyTalk) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game->GetHero(),"TEXT_QST_005_V2_ARENA_MAIN_CELL_GUARD2_EARLY_FIRST",GROUP_SELECT_NONE,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                EarlyTalk=true;
            } else if (ParentClass.ArenaState==MAS_CELLS_PRE_ARENA_NORMAL_GO && !ChamTalk) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game->GetHero(),"TEXT_QST_005_V2_ARENA_MAIN_CELL_GUARD2_LATE",GROUP_SELECT_NONE,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                ChamTalk=true;
            } else {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game->GetHero(),"TEXT_QST_005_V2_ARENA_MAIN_CELL_GUARD2_VERY_LATE",GROUP_SELECT_NONE,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
            }
        } else if (IsDistanceBetweenThingsOver(Me,Game->GetThingWithScriptName("GuardingDoorMarkerLeft"),1.0f) && !seh_me.IsPerformingScriptTask()) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            seh_me.MoveToPosition(Game->GetThingWithScriptName("GuardingDoorMarkerLeft").GetPos(),0.0f,ENTITY_MOVE_WALK,false,true);
            Turned=false;
        } else if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL,"SCRIPT_NAME_HERO"))) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (seh_me.IsPerformingScriptTask()) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                seh_me.ClearCommands();
            }
            if (!ParentClass.InHitCutsceneAlready) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                ParentClass.InHitCutsceneAlready=true;
                CScriptGameResourceObjectMovieBase speech_movie;
                Game->StartMovieSequence("",speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                    seh_me.Speak(Game->GetHero(),"TEXT_QST_005_V2_ARENA_MAIN_CELL_GUARD2_ATTACKED",GROUP_SELECT_NONE,false,true,false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                Game->ModifyThingHealth(Me,10000.0f,false);
                Game->EntitySetThingAsAllyOfThing(Me,Game->GetHero());
                Game->EntitySetThingAsAllyOfThing(Game->GetHero(),Me);
                ParentClass.InHitCutsceneAlready=false;
            }
        } else if (!Turned) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (!seh_me.IsPerformingScriptTask()) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Turned=true;
                Game->EntitySetFacingAngle(Me,Game->GetThingWithScriptName("GuardingDoorMarkerLeft").GetAngleXY(),false);
            }
        }
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
}