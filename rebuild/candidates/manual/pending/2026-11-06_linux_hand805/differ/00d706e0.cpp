// NScript::CQ_HangingTreeGoodScript::CHTG_Guard::Main() -- Hanging Tree (good) prisoner escort guard: the party leader introduces himself when talked to, walks the waypoints (complaining when the hero hits him, waiting at the exit when the waypoints run out), runs back to the prisoner and rallies the party when attacked; the other guards follow the prisoner, speak in support when talked to and fight the nearest freedom fighter; at the end every guard walks to the audience watch point.
// Grade: DIFFER(4755v4755) -- the SetPredicate(CIsThingAlive(Me)) temporary and what follows from it: retail expands ~CIsThingAlive (CIsThingAlive vtable 0x012C32F0 held in ebp for the ctor store and the dtor's vptr restore) around an out-of-line ~CScriptThing 0x004AA840, so ebp is pushed in the prologue and the first terminate check jumps to the shared exit; ours calls ~CIsThingAlive out of line, pushes ebp later (0x00D707F3) and gives that first check its own exit copy. The body after 0x00D70800 matches (each exit's pop ebp moves by 2)
// Next idea: the CIsThingAlive copy-inline case of docs/pipeline/INLINE_DEPTH_REVISIT.md: retail expands ~CScriptThing at the local sites but not inside ~CIsThingAlive (a4/a8 __forceinline ~CIsThingAlive 4803 bytes, inner dtor expands; a7 without the row-local ~CScriptThing 4675, local dtors become calls)
// Leaf: ?Main@CHTG_Guard@CQ_HangingTreeGoodScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D706E0, 4755 bytes. Name: slot 1 of the entity vtable 0x012D27A8 that the landed Alloc
//   0x00D70520 stores (0 0x00D706C0, 2 Init 0x00D705D0, 3 GetParentScript 0x00D705C0, 5 OnPredicateFail 0x00D70650, 6 OnInterrupted
//   0x00D706B0); Ego_r.pdb and FableWin.pdb ?Main@CHTG_Guard@CQ_HangingTreeGoodScript@NScript@@EAEXXZ. Layout: engine/CQ_HangingTreeGoodScript.h
//   opt-in FABLE_RETAIL_CQ_HANGINGTREEGOODSCRIPT_GUARD (hdr overlay).
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
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
enum ETextGroupSelectionMethod { GROUP_SELECT_NONE=0, GROUP_SELECT_RANDOM=1, GROUP_SELECT_RANDOM_NO_REPEAT=2, GROUP_SELECT_SEQUENTIAL=3, GROUP_SELECT_FIRST_ONLY=4 };
enum EScriptEntityMoveType { ENTITY_MOVE_WALK=0, ENTITY_MOVE_RUN=1, ENTITY_MOVE_SNEAK=2 };
enum EHeroAbility { HERO_ABILITY_NULL=0, HERO_ABILITY_FORCE_PUSH=1, HERO_ABILITY_TIME_SPELL=2, HERO_ABILITY_ENFLAME_SPELL=3, HERO_ABILITY_PHYSICAL_SHIELD_SPELL=4, HERO_ABILITY_TURNCOAT_SPELL=5, HERO_ABILITY_DRAIN_LIFE_SPELL=6, HERO_ABILITY_RAISE_DEAD_SPELL=7, HERO_ABILITY_BERSERK=8, HERO_ABILITY_DOUBLE_STRIKE=9, HERO_ABILITY_SUMMON_SPELL=10, HERO_ABILITY_LIGHTNING_SPELL=11, HERO_ABILITY_BATTLE_CHARGE=12, HERO_ABILITY_ASSASSIN_RUSH=13, HERO_ABILITY_HEAL_LIFE_SPELL=14, HERO_ABILITY_GHOST_SWORD_SPELL=15, HERO_ABILITY_FIREBALL_SPELL=16, HERO_ABILITY_MULTI_ARROW=17, HERO_ABILITY_DIVINE_WRATH_SPELL=18, HERO_ABILITY_UNHOLY_POWER_SPELL=19, MAX_NUMBER_OF_HERO_ABILITIES=20 };
template<> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing):Thing(thing) {}
inline NScript::CIsThingAlive::~CIsThingAlive() {}
inline CScriptThing::~CScriptThing() { }
inline bool CScriptThing::IsAlive() const { return PImp.Data && PImp.Data->IsAlive(); }
inline bool CScriptThing::operator==(const CScriptThing& other) const { return PImp->IsEqualTo(*other.PImp); }
__forceinline const C3DVector& CScriptThing::GetPos() const { if (!PImp.Data) return NullCoord; return PImp.Data->GetPos(); }
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& thing_a, const CScriptThing& thing_b, float distance);   // 0x00CBE2FF
bool __fastcall IsDistanceBetweenThingsOver(const CScriptThing& thing_a, const CScriptThing& thing_b, float distance);    // 0x00CBE3EA
bool __fastcall IsDistanceFromThingToPositionOver(const CScriptThing& thing, const C3DVector& pos, float distance);       // 0x00CBE45C
void NScript::CQ_HangingTreeGoodScript::CHTG_Guard::Main()
{
    TRetVoid _Fail_Val;
    SetPredicate(CIsThingAlive(Me));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    CScriptGameResourceObjectScriptedThingBase seh_me;
    if (seh_me.IsValid()) seh_me.Release();
    while (!Game->StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGH)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    while (!ParentClass.PartySuccessfullySetUp) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    while (ParentClass.HTG_State!=7) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        seh_me.ClearCommands();
        if (ParentClass.Leader.IsAlive() && ParentClass.Leader==Me) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            while (ParentClass.HTG_State==1) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    {
                        CScriptGameResourceObjectMovieBase speech_movie;
                        Game->StartMovieSequence("",speech_movie);
                        CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                        if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                            seh_me.Speak(Game->GetHero(),"TEXT_QST_030_GUARD_INTRODUCTION",GROUP_SELECT_NONE,false,true,false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game->NewScriptFrame();
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
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                ParentClass.BanditYelledOut=false;
                if (ParentClass.LeaderSpokenTo) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (ParentClass.ActiveWayPoint.IsAlive()) {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        seh_me.MoveToPosition(ParentClass.ActiveWayPoint.GetPos(),0.0f,ENTITY_MOVE_WALK,false,true);
                        while (ParentClass.HTG_State==2 && !IsDistanceBetweenThingsUnder(Me,ParentClass.ActiveWayPoint,5.0f)) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL,"SCRIPT_NAME_HERO"))) {
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                                long HLStyleConversationIndex=Game->AddNewConversation(Me,false,false);
                                Game->AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_030_GUARD_HIT_BY_HERO",false,Me,Game->GetHero());
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
                            CScriptThing ExitPoint=Game->GetThingWithScriptName("HTG_ExitPoint");
                            long HLStyleConversationIndex=Game->AddNewConversation(Me,false,false);
                            Game->AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_030_GUARD_WAITING",false,Me,Game->GetHero());
                            while (!ParentClass.ExitCutsceneStart) {
                                Game->NewScriptFrame();
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            seh_me.MoveToPosition(ExitPoint.GetPos(),2.0f,ENTITY_MOVE_WALK,true,false);
                            if (seh_me.IsValid()) seh_me.Release();
                            while (!false) {
                                Game->NewScriptFrame();
                                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            }
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                    }
                }
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            while (ParentClass.HTG_State==5) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (seh_me.IsValid()) seh_me.Release();
                while (!Game->StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGH)) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                seh_me.ClearCommands();
                if (!ParentClass.BanditYelledOut) {
                    long HLStyleConversationIndex=Game->AddNewConversation(Me,false,false);
                    Game->AddPersonToConversation(HLStyleConversationIndex,Game->GetHero());
                    Game->AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_030_GUARD_ATTACKED",false,Me,Game->GetHero());
                    ParentClass.BanditYelledOut=true;
                }
                C3DVector PrisonerPos=ParentClass.Prisoner.GetPos();
                while (IsDistanceFromThingToPositionOver(seh_me.GetScriptThing(),PrisonerPos,2.0f)) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    seh_me.MoveToPosition(PrisonerPos,0.0f,ENTITY_MOVE_RUN,false,true);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                while (ParentClass.HTG_State!=2) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                long HLStyleConversationIndex=Game->AddNewConversation(Me,false,false);
                Game->AddPersonToConversation(HLStyleConversationIndex,Game->GetHero());
                Game->AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_030_GUARD_LETS_MOVE",false,Me,Game->GetHero());
                if (seh_me.IsValid()) seh_me.Release();
                while (!Game->StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGH)) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        else {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            while (ParentClass.HTG_State==1) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    CScriptGameResourceObjectMovieBase speech_movie;
                    Game->StartMovieSequence("",speech_movie);
                    CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                    if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) {
                        seh_me.Speak(Game->GetHero(),"TEXT_QST_030_GUARD_SUPPORT",GROUP_SELECT_NONE,false,true,false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                }
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            while (ParentClass.HTG_State==2) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (ParentClass.Prisoner.IsAlive()) seh_me.FollowThing(ParentClass.Prisoner,1.0f,true);
                while (ParentClass.HTG_State==2 && !(ParentClass.Leader==Me) && !ParentClass.Leader.IsDead()) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL,"SCRIPT_NAME_HERO"))) {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        long HLStyleConversationIndex=Game->AddNewConversation(Me,false,false);
                        Game->AddLineToConversation(HLStyleConversationIndex,"TEXT_QST_030_GUARD_HIT_BY_HERO",false,Me,Game->GetHero());
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
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            while (ParentClass.HTG_State==5) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (seh_me.IsValid()) seh_me.Release();
                CScriptThing FreedomFighter=Game->GetNearestWithScriptName(Me,"HTG_FreedomFighter");
                if (FreedomFighter.IsAlive()) Game->GiveThingBestEnemyTarget(Me,FreedomFighter);
                while (ParentClass.HTG_State==5) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (seh_me.IsValid()) seh_me.Release();
                while (!Game->StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGH)) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (ParentClass.HTG_State==3) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                while (!false) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
        }
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    CScriptThing AudienceWatchPoint=Game->GetThingWithScriptName("HTG_AudienceWatchPoint");
    while (IsDistanceBetweenThingsOver(Me,AudienceWatchPoint,2.1f)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (!seh_me.IsPerformingScriptTask()) seh_me.MoveToPosition(AudienceWatchPoint.GetPos(),2.0f,ENTITY_MOVE_WALK,false,false);
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    while (!false) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
}
