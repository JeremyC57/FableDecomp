// NScript::CQ_HangingTreeGoodScript::CHTG_Guard::Main() -- Hanging Tree (good) prisoner escort guard: the party leader introduces himself when talked to, walks the waypoints (complaining when the hero hits him, waiting at the exit when the waypoints run out), runs back to the prisoner and rallies the party when attacked; the other guards follow the prisoner, speak in support when talked to and fight the nearest freedom fighter; at the end every guard walks to the audience watch point.
// Grade: RELOCATION_MATCH -- actual COFF operands checked against their native targets.
// Inline-budget: predicate-only depth region retains nested thing copy and cleanup calls; prior hand805 plain and inline-budget attempts failed.
// Leaf: ?Main@CHTG_Guard@CQ_HangingTreeGoodScript@NScript@@EAEXXZ
// Retail: 0x00D706E0, 4755 bytes, thiscall.
//   0x00D70520 stores (0 0x00D706C0, 2 Init 0x00D705D0, 3 GetParentScript 0x00D705C0, 5 OnPredicateFail 0x00D70650, 6 OnInterrupted
//   0x00D706B0); Ego_r.pdb and FableWin.pdb ?Main@CHTG_Guard@CQ_HangingTreeGoodScript@NScript@@EAEXXZ. Layout: engine/CQ_HangingTreeGoodScript.h
//   opt-in FABLE_RETAIL_CQ_HANGINGTREEGOODSCRIPT_GUARD (hdr overlay).
#define FABLE_RETAIL_CLEVELSCRIPTBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_CISTHINGALIVE_ORIGINAL_ACCESS
#define FABLE_RETAIL_CENTITYSCRIPTBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_CPREDICATEBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_CPREDICATEBASE_NOVTABLE
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CSCRIPTTHING_NULLCOORD
#define FABLE_RETAIL_CQ_HANGINGTREEGOODSCRIPT
#define FABLE_RETAIL_CQ_HANGINGTREEGOODSCRIPT_GUARD
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/C3DVector.h"
#include "engine/CScriptThing.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CQ_HangingTreeGoodScript.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/TRetVoid.h"
#include "engine/CIsThingAlive.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
#include "engine/ETextGroupSelectionMethod.h"
#include "engine/EScriptEntityMoveType.h"
#include "engine/EHeroAbility.h"
template<> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing):Thing(thing) {}
inline NScript::CIsThingAlive::~CIsThingAlive() {}
#ifdef _MSC_VER
#pragma inline_depth()
#endif
inline CScriptThing::~CScriptThing() { }
inline bool CScriptThing::IsAlive() const { return PImp.Data && PImp.Data->IsAlive(); }
inline bool CScriptThing::operator==(const CScriptThing& other) const { return PImp->IsEqualTo(*other.PImp); }
FABLE_FORCEINLINE const C3DVector& CScriptThing::GetPos() const { if (!PImp.Data) return NullCoord; return PImp.Data->GetPos(); }
bool FABLE_FASTCALL IsDistanceBetweenThingsUnder(const CScriptThing& thing_a, const CScriptThing& thing_b, float distance);   // 0x00CBE2FF
bool FABLE_FASTCALL IsDistanceBetweenThingsOver(const CScriptThing& thing_a, const CScriptThing& thing_b, float distance);    // 0x00CBE3EA
bool FABLE_FASTCALL IsDistanceFromThingToPositionOver(const CScriptThing& thing, const C3DVector& pos, float distance);       // 0x00CBE45C
void NScript::CQ_HangingTreeGoodScript::CHTG_Guard::Main()
{
    TRetVoid _Fail_Val;
    SetPredicate(CIsThingAlive(Me));
    Game.NewScriptFrame();
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    CScriptGameResourceObjectScriptedThingBase seh_me;
    if (seh_me.IsValid()) seh_me.Release();
    while (!Game.StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGH)) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    while (!ParentClass.PartySuccessfullySetUp) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    while (ParentClass.HTG_State!=7) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        seh_me.ClearCommands();
        if (ParentClass.Leader.IsAlive() && ParentClass.Leader==Me) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            while (ParentClass.HTG_State==1) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    {
                        CScriptGameResourceObjectMovieBase speech_movie;
                        Game.StartMovieSequence("",speech_movie);
                        CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                        if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                            seh_me.Speak(Game.GetHero(),"TEXT_QST_030_GUARD_INTRODUCTION",GROUP_SELECT_NONE,false,true,false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game.NewScriptFrame();
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                    }
                    ParentClass.LeaderSpokenTo=true;
                }
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            while (ParentClass.HTG_State==2) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                ParentClass.BanditYelledOut=false;
                if (ParentClass.LeaderSpokenTo) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (ParentClass.ActiveWayPoint.IsAlive()) {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        seh_me.MoveToPosition(ParentClass.ActiveWayPoint.GetPos(),0.0f,ENTITY_MOVE_WALK,false,true);
                        while (ParentClass.HTG_State==2 && !IsDistanceBetweenThingsUnder(Me,ParentClass.ActiveWayPoint,5.0f)) {
                            Game.NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL,"SCRIPT_NAME_HERO"))) {
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                                long HLStyleConversationIndex=Game.AddNewConversation(Me,false,false);
                                Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_030_GUARD_HIT_BY_HERO",false,Me,Game.GetHero());
                            }
                            if (!seh_me.IsPerformingScriptTask()) {
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                                seh_me.MoveToPosition(ParentClass.ActiveWayPoint.GetPos(),0.0f,ENTITY_MOVE_WALK,false,true);
                            }
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        if (ParentClass.HTG_State==2) ParentClass.AtWayPoint=true;
                    }
                    else {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        if (ParentClass.CurrentWayPoint<=14) {
                            ParentClass.HTG_State=3;
                            CScriptThing ExitPoint=Game.GetThingWithScriptName("HTG_ExitPoint");
                            long HLStyleConversationIndex=Game.AddNewConversation(Me,false,false);
                            Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_030_GUARD_WAITING",false,Me,Game.GetHero());
                            while (!ParentClass.ExitCutsceneStart) {
                                Game.NewScriptFrame();
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            seh_me.MoveToPosition(ExitPoint.GetPos(),2.0f,ENTITY_MOVE_WALK,true,false);
                            if (seh_me.IsValid()) seh_me.Release();
                            while (!false) {
                                Game.NewScriptFrame();
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                    }
                }
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            while (ParentClass.HTG_State==5) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (seh_me.IsValid()) seh_me.Release();
                while (!Game.StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGH)) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                seh_me.ClearCommands();
                if (!ParentClass.BanditYelledOut) {
                    long HLStyleConversationIndex=Game.AddNewConversation(Me,false,false);
                    Game.AddPersonToConversation(HLStyleConversationIndex,Game.GetHero());
                    Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_030_GUARD_ATTACKED",false,Me,Game.GetHero());
                    ParentClass.BanditYelledOut=true;
                }
                C3DVector PrisonerPos=ParentClass.Prisoner.GetPos();
                while (IsDistanceFromThingToPositionOver(seh_me.GetScriptThing(),PrisonerPos,2.0f)) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    seh_me.MoveToPosition(PrisonerPos,0.0f,ENTITY_MOVE_RUN,false,true);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                while (ParentClass.HTG_State!=2) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                long HLStyleConversationIndex=Game.AddNewConversation(Me,false,false);
                Game.AddPersonToConversation(HLStyleConversationIndex,Game.GetHero());
                Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_030_GUARD_LETS_MOVE",false,Me,Game.GetHero());
                if (seh_me.IsValid()) seh_me.Release();
                while (!Game.StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGH)) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        else {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            while (ParentClass.HTG_State==1) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    CScriptGameResourceObjectMovieBase speech_movie;
                    Game.StartMovieSequence("",speech_movie);
                    CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
                    if (Game.GetHealth(seh_me.GetScriptThing())>0.0f) {
                        seh_me.Speak(Game.GetHero(),"TEXT_QST_030_GUARD_SUPPORT",GROUP_SELECT_NONE,false,true,false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game.NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                }
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            while (ParentClass.HTG_State==2) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (ParentClass.Prisoner.IsAlive()) seh_me.FollowThing(ParentClass.Prisoner,1.0f,true);
                while (ParentClass.HTG_State==2 && !(ParentClass.Leader==Me) && !ParentClass.Leader.IsDead()) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL,"SCRIPT_NAME_HERO"))) {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        long HLStyleConversationIndex=Game.AddNewConversation(Me,false,false);
                        Game.AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_030_GUARD_HIT_BY_HERO",false,Me,Game.GetHero());
                    }
                    if (!seh_me.IsPerformingScriptTask()) {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        seh_me.FollowThing(ParentClass.Prisoner,1.0f,true);
                    }
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                seh_me.ClearCommands();
                if (ParentClass.Leader.IsDead()) ParentClass.ReassignLeaderNow=true;
                else if (ParentClass.HTG_State!=7 && ParentClass.HTG_State!=3 && ParentClass.HTG_State!=4) ParentClass.HTG_State=5;
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            while (ParentClass.HTG_State==5) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (seh_me.IsValid()) seh_me.Release();
                CScriptThing FreedomFighter=Game.GetNearestWithScriptName(Me,"HTG_FreedomFighter");
                if (FreedomFighter.IsAlive()) Game.GiveThingBestEnemyTarget(Me,FreedomFighter);
                while (ParentClass.HTG_State==5) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (seh_me.IsValid()) seh_me.Release();
                while (!Game.StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGH)) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (ParentClass.HTG_State==3) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                while (!false) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
        }
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    CScriptThing AudienceWatchPoint=Game.GetThingWithScriptName("HTG_AudienceWatchPoint");
    while (IsDistanceBetweenThingsOver(Me,AudienceWatchPoint,2.1f)) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (!seh_me.IsPerformingScriptTask()) seh_me.MoveToPosition(AudienceWatchPoint.GetPos(),2.0f,ENTITY_MOVE_WALK,false,false);
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    while (!false) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
}