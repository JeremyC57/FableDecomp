// NScript::CQ_OrchardFarmRaidScript::CCrateTeamMember::Main() -- a crate-raid team member: wait for the introduction (removed if the hero came in the wrong way), then patrol, comment on AI-state changes, fetch, defend, chase and carry crates until Whisper spawns, then walk to the team exit marker and fade out
// Grade: DIFFER(2589v2589) -- 6 hunks, all in the DEFENDING block 0x00DCE7F2..0x00DCE82F: retail vtable regs eax (EntityStopFollowing) / edx (NewScriptFrame) / eax+edx=MyTeam (EntityFollowThing), ours edx / eax / edx+eax; every call, literal, branch and frame slot equal
// Inline-budget (would be needed on match): FABLE_FORCEINLINE on the row-local CScriptThing::IsBeingCarriedBy / GetPos wrapper bodies -- retail expands both (no call to 0x004AAFE0 / 0x004AA980 in the row); prior lane: plain inline leaves calls.
// Inline-budget: #pragma inline_depth(0) round the row-local inline CIsThingAlive(const CScriptThing&) definition + FABLE_RETAIL_CPREDICATEBASE_NOVTABLE -- retail calls the CScriptThing copy 0x004ABE90 inside the expanded predicate ctor (0x00DCE250) yet expands the by-value Me copies (landed CBeggarBully::Main 0x00E5B3B0 idiom)
// Leaf: ?Main@CCrateTeamMember@CQ_OrchardFarmRaidScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DCE230, 2589 bytes, thiscall, plain `ret`, no EH frame.
#include "fable_compiler.h"
#define FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_COPY
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CPREDICATEBASE_NOVTABLE
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_CTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_CQ_ORCHARDFARMRAIDSCRIPT_CCRATETEAMMEMBER_MAIN
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "stlport/fable_containers.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CQ_OrchardFarmRaidScript.h"
#include "engine/CIsThingAlive.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CTimer.h"
#include "engine/fable_script_distance.h"
#include "engine/fable_script_fade.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
#pragma inline_depth(0)
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing) : Thing(thing) { }
#pragma inline_depth()
inline NScript::CIsThingAlive::~CIsThingAlive() { }
inline CScriptThing::CScriptThing() { }
inline bool CScriptThing::IsAlive() const { return PImp.Data && PImp.Data->IsAlive(); }
FABLE_FORCEINLINE bool CScriptThing::IsBeingCarriedBy(const CCharString& carrier) const { return PImp.Data && PImp.Data->IsBeingCarriedBy(carrier); }
FABLE_FORCEINLINE const C3DVector& CScriptThing::GetPos() const { return !PImp.Data ? NullCoord : PImp.Data->GetPos(); }
inline bool CScriptThing::operator==(const CScriptThing& other) const { return PImp.Data->IsEqualTo(*other.PImp.Data); }
inline bool CScriptThing::operator!=(const CScriptThing& other) const { return !(*this == other); }
void NScript::CQ_OrchardFarmRaidScript::CCrateTeamMember::Main()
{
    SetPredicate(CIsThingAlive(Me));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating()) return;
    while (!ParentClass->DoneIntroduction) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
        if (ParentClass->HeroAtWrongEntrance) {
            if (IsActiveThreadTerminating()) return;
            Game->RemoveThing(Me,false,false);
            if (ParentClass->Teams[TeamID].MemberCount>0) {
                if (IsActiveThreadTerminating()) return;
                ParentClass->Teams[TeamID].MemberCount--;
            }
            ParentClass->Teams[TeamID].StateCounter[MemberState]--;
            return;
        }
    }
    if (IsActiveThreadTerminating()) return;
    if (TeamID==1 && TeamID==ParentClass->HeroTeam) {
        if (IsActiveThreadTerminating()) return;
        while (Game->IsInCutscene()) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        ParentClass->MakeTeamMemberComment("FETCHING_03",Me,(ERaidComment)0);
    }
    bool fetching=false;
    while (!ParentClass->WhisperSpawned) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
        GoOnPatrol();
        CurrentAIState=Me.GetCurrentStateGroupType();
        if (CurrentAIState!=PreviousAIState) {
            if (IsActiveThreadTerminating()) return;
            if (CurrentAIState==2) {
                if (IsActiveThreadTerminating()) return;
                if (MemberState==3) {
                    if (IsActiveThreadTerminating()) return;
                    ParentClass->MakeTeamMemberComment("DEFENDING",Me,(ERaidComment)0);
                } else {
                    if (IsActiveThreadTerminating()) return;
                    ParentClass->MakeTeamMemberComment("FOLLOWING",Me,(ERaidComment)0);
                }
            } else if (CurrentAIState==1) {
                if (IsActiveThreadTerminating()) return;
                ParentClass->MakeTeamMemberComment("ATTACKING",Me,(ERaidComment)0);
            }
        }
        PreviousAIState=CurrentAIState;
        if (MemberState==1 && MyTeam->TeamCrateCarrier.IsDead() && MyTeam->StateCounter[2]==0) {
            if (IsActiveThreadTerminating()) return;
            CScriptThing crate=GetNearestCrateToMe();
            if (crate.IsAlive() && !crate.IsBeingCarriedBy("")) {
                if (IsActiveThreadTerminating()) return;
                bool near_crate=IsDistanceBetweenThingsUnder(Me,crate,10.0f);
                if (ParentClass->HeroTeam==0 && TeamID==1) {
                    if (IsActiveThreadTerminating()) return;
                    near_crate=true;
                }
                if (IsDistanceBetweenThingsOver(crate,MyTeam->CrateDropPos,5.0f) && near_crate) {
                    if (IsActiveThreadTerminating()) return;
                    if (TeamID==1) {
                        if (IsActiveThreadTerminating()) return;
                        Game->EntityStopFollowing(Me);
                        Game->SetCombatNearbyBreakOffRange(Me,4.0f);
                        Game->SetStealStealableItems(Me,true);
                        Game->SetRecoverStealableItems(Me,false);
                    } else {
                        if (IsActiveThreadTerminating()) return;
                        Game->SetCombatNearbyBreakOffRange(Me,4.0f);
                        Game->SetStealStealableItems(Me,false);
                        Game->SetRecoverStealableItems(Me,true);
                    }
                    fetching=true;
                    SetState(2);
                    ParentClass->MakeTeamMemberComment("FETCHING",Me,(ERaidComment)0);
                } else if (fetching) {
                    if (IsActiveThreadTerminating()) return;
                    fetching=false;
                    Game->ResetCombatNearbyBreakOffRange(Me);
                    Game->SetStealStealableItems(Me,false);
                    Game->SetRecoverStealableItems(Me,false);
                    SetState(0);
                }
            }
        }
        if (MemberState==1 && MyTeam->TeamCrateCarrier.IsAlive() && MyTeam->TeamCrateCarrier!=Me) {
            if (IsActiveThreadTerminating()) return;
            SetState(3);
            Game->EntityStopFollowing(Me);
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
            const CScriptThing& carrier=MyTeam->TeamCrateCarrier;
            Game->EntityFollowThing(Me,carrier,1.0f,true);
            ParentClass->MakeTeamMemberComment("DEFENDING",Me,(ERaidComment)0);
        }
        if (MemberState==3 && (MyTeam->TeamCrateCarrier.IsDead() || !IsThingCarryingCrate(MyTeam->TeamCrateCarrier))) {
            if (IsActiveThreadTerminating()) return;
            Game->EntityStopFollowing(Me);
            SetState(0);
        }
        if (MemberState==1 && MyTeam->EnemyTeam->TeamCrateCarrier.IsAlive()) {
            if (IsActiveThreadTerminating()) return;
            SetState(4);
            Game->EntityStopFollowing(Me);
            Game->EntityFollowThing(Me,MyTeam->EnemyTeam->TeamCrateCarrier,1.0f,true);
            ParentClass->MakeTeamMemberComment("FETCHING",Me,(ERaidComment)0);
        }
        if (MemberState==4 && (MyTeam->EnemyTeam->TeamCrateCarrier.IsDead() || !IsThingCarryingCrate(MyTeam->EnemyTeam->TeamCrateCarrier))) {
            if (IsActiveThreadTerminating()) return;
            SetState(0);
        }
        if (MemberState==2 && IsThingCarryingCrate(Me)) {
            if (IsActiveThreadTerminating()) return;
            SetState(5);
            MyTeam->TeamCrateCarrier=Me;
            ParentClass->MakeTeamMemberComment("REQUEST_PROTECTION",Me,(ERaidComment)0);
            Game->SetCombatNearbyBreakOffRange(Me,0.0f);
        }
        if (MemberState==5 && !IsThingCarryingCrate(Me)) {
            if (IsActiveThreadTerminating()) return;
            MyTeam->TeamCrateCarrier=CScriptThing();
            SetState(0);
            Game->SetCombatNearbyBreakOffRange(Me,4.0f);
        }
    }
    if (IsActiveThreadTerminating()) return;
    Game->EntityStopFollowing(Me);
    CScriptThing exit_marker=Game->GetNearestWithScriptName(Me,"TeamExitMarker");
    CScriptGameResourceObjectScriptedThingBase seh_me;
    if (seh_me.IsValid()) seh_me.Release();
    while (!Game->StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    if (IsActiveThreadTerminating()) return;
    for (;;) {
        if (!seh_me.IsPerformingScriptTask()) {
            if (IsActiveThreadTerminating()) return;
            seh_me.MoveToPosition(exit_marker.GetPos(),0.5f,(EScriptEntityMoveType)1,false,true);
        }
        if (IsDistanceBetweenThingsUnder(Me,exit_marker,2.0f)) break;
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    FadeOutAndKillEntity(Me,3.0f,true,true);
    for (;;) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
}

