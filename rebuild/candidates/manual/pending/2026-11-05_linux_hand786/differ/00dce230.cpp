// NScript::CQ_OrchardFarmRaidScript::CCrateTeamMember::Main() -- a crate-raid team member: wait for the introduction (removed if the hero came in the wrong way), then patrol, comment on AI-state changes, fetch, defend, chase and carry crates until Whisper spawns, then walk to the team exit marker and fade out
// Grade: DIFFER(2605v2589) -- the prologue CIsThingAlive(Me) expands the CScriptThing copy inline where retail calls 0x004ABE90 (16 bytes; the INLINE_DEPTH_REVISIT "copy inside inline-expanded special members" family), which moves &Me out of edi and shifts register choice downstream: the two RemoveThing-path decrements keep `dec [mem]` where retail splits load/dec/store, and the DEFENDING block (0x00DCE7F2..0x00DCE82F) swaps eax/edx; every call, literal and branch is in retail order
// Next idea: find the CScriptThing copy-ctor spelling whose c2 size sits between this one (inlined at the f=1 prologue) and the `PImp() { PImp = other.PImp; }` form (probe a7: prologue exact, but every copy out of line); then recheck the decrement splits and the DEFENDING registers
// Leaf: ?Main@CCrateTeamMember@CQ_OrchardFarmRaidScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DCE230, 2589 bytes, thiscall, plain `ret`, no EH frame.
// Identity (proven): Ego_r.pdb / FableWin.pdb name set ?Main@CCrateTeamMember@CQ_OrchardFarmRaidScript@NScript@@EAEXXZ; it calls the
//   landed CCrateTeamMember members GoOnPatrol 0x00DCEC70, IsThingCarryingCrate 0x00DCED10, GetNearestCrateToMe 0x00DCEDF0 and the
//   parent's MakeTeamMemberComment 0x00DCDA80, and SetState 0x00DCEC50 (?SetState@CCrateTeamMember@CQ_OrchardFarmRaidScript@NScript@@AAEXJ@Z:
//   `--MyTeam->StateCounter[MemberState]; MemberState = s; ++MyTeam->StateCounter[s]`, ret 4, all eight callers in this function;
//   the manifest label CHandleBasedMemoryPool::GetPointerFromHandle is wrong).
// Callees: CScriptThing copy 0x004ABE90 / ~ 0x004AA840 / operator= 0x008AB980 / CBaseClass() 0x0099A2D0, CCharString 0x0099EBF0 / ~ 0x0099EAE0,
//   IsDistanceBetweenThingsUnder 0x00CBE2FF, IsDistanceBetweenThingsOver 0x00CBE3EA, FadeOutAndKillEntity 0x00CBE716 (all __fastcall),
//   scripted-thing ctor 0x007E72A0 / dtor 0x007E74D0 / IsValid / Release / IsPerformingScriptTask / MoveToPosition 0x007E72F0;
//   CScriptThing slots 13 GetCurrentStateGroupType, 63 IsBeingCarriedBy, 75 IsAlive, 76 IsDead, 78 IsEqualTo; game slots 7, 8, 77
//   GetNearestWithScriptName, 108 RemoveThing, 229 EntityFollowThing, 230 EntityStopFollowing, 386 IsInCutscene, 817
//   SetCombatNearbyBreakOffRange, 819 ResetCombatNearbyBreakOffRange, 820 SetStealStealableItems, 823 SetRecoverStealableItems.
// Layout: Ego_r.pdb CCrateTeamMember (entity offsets equal retail); CQ_OrchardFarmRaidScript retail = Ego_r - 0x14 up to CrateList,
//   - 0x18 after it (STLport vector 12 bytes): DoneIntroduction +0x50, HeroAtWrongEntrance +0x51, WhisperSpawned +0x52, HeroTeam +0x78,
//   Teams[2] +0x8c; CCrateTeamManager (Ego_r 64 bytes): TeamCrateCarrier +0x30, CrateDropPos +0x24, EnemyTeam +0x3c.
// Descriptive names: the locals fetching, near_crate, crate, carrier, exit_marker; the AI-state (1, 2) and member-state (0..5) numbers.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_CTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_DTOR
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CIsThingAlive.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CLevelScriptBase.h"
#include "engine/CTimer.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing) : Thing(thing) { }
inline NScript::CIsThingAlive::~CIsThingAlive() { }
inline CScriptThing::CScriptThing() { }
inline CScriptThing::CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) { }
inline bool CScriptThing::IsAlive() const { return PImp.Data && PImp.Data->IsAlive(); }
__forceinline bool CScriptThing::IsBeingCarriedBy(const CCharString& carrier) const { return PImp.Data && PImp.Data->IsBeingCarriedBy(carrier); }
__forceinline const C3DVector& CScriptThing::GetPos() const { return !PImp.Data ? NullCoord : PImp.Data->GetPos(); }
inline bool CScriptThing::operator==(const CScriptThing& other) const { return PImp.Data->IsEqualTo(*other.PImp.Data); }
inline bool CScriptThing::operator!=(const CScriptThing& other) const { return !(*this == other); }
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& thing1, const CScriptThing& thing2, float distance);   // 0x00CBE2FF
bool __fastcall IsDistanceBetweenThingsOver(const CScriptThing& thing1, const CScriptThing& thing2, float distance);    // 0x00CBE3EA
void __fastcall FadeOutAndKillEntity(const CScriptThing& thing, float time, bool kill, bool fade);                      // 0x00CBE716
namespace NScript {
struct CCrateTeamManager {                                  // Ego_r.pdb 64 bytes
    long         StateCounter[6];                           // +0x00
    long         MemberCount;                               // +0x18
    CTimer       TeamReinforcementsTimer;                   // +0x1c
    long         MaxTeamMembers;                            // +0x20
    CScriptThing CrateDropPos;                              // +0x24
    CScriptThing TeamCrateCarrier;                          // +0x30
    CCrateTeamManager* EnemyTeam;                           // +0x3c
};
class CQ_OrchardFarmRaidScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    long              CrateCount;                           // +0x48
    CTimer            CommentTimer;                         // +0x4c
    bool              DoneIntroduction;                     // +0x50
    bool              HeroAtWrongEntrance;                  // +0x51
    bool              WhisperSpawned;                       // +0x52
    CScriptThing*     CrateList[3];                         // +0x54 std::vector<CScriptThing> (STLport, 12 bytes)
    bool              MissionSucceeded;                     // +0x60
    long              MissionFailed;                        // +0x64
    CWideString       FailReasons[4];                       // +0x68
    long              HeroTeam;                             // +0x78
    long              BanditWavesSpawned;                   // +0x7c
    CTimer            RemindHeroOfObjectivesTimer;          // +0x80
    CCharString       TextSystemScriptCode;                 // +0x84
    bool              HeroMetWhisperBeforeFarm;             // +0x88
    bool              ShownCombatMultiplierTutorial;        // +0x89
    bool              ShownCombatFlourishTutorial;          // +0x8a
    bool              WhisperInCutscene;                    // +0x8b
    CCrateTeamManager Teams[2];                             // +0x8c
    bool MakeTeamMemberComment(const CCharString& line, const CScriptThing& speaker, bool unused);   // 0x00DCDA80
    class CCrateTeamMember : public CEntityScriptBase {
    public:
        CQ_OrchardFarmRaidScript* ParentClass;              // +0x14
        CQ_SunnyvaleMasterData*   PMasterData;              // +0x18
        long                      CurrentAIState;           // +0x1c
        long                      PreviousAIState;          // +0x20
        long                      TeamID;                   // +0x24
        long                      MemberState;              // +0x28
        CCrateTeamManager*        MyTeam;                   // +0x2c
        CScriptThing              ThingToPatrolTo;          // +0x30
    private:
        virtual void Main();
        void GoOnPatrol();                                  // 0x00DCEC70
        bool IsThingCarryingCrate(const CScriptThing& thing);   // 0x00DCED10
        CScriptThing GetNearestCrateToMe();                 // 0x00DCEDF0
        void SetState(long state);                          // 0x00DCEC50
    };
};
}
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
        ParentClass->MakeTeamMemberComment("FETCHING_03",Me,false);
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
                    ParentClass->MakeTeamMemberComment("DEFENDING",Me,false);
                } else {
                    if (IsActiveThreadTerminating()) return;
                    ParentClass->MakeTeamMemberComment("FOLLOWING",Me,false);
                }
            } else if (CurrentAIState==1) {
                if (IsActiveThreadTerminating()) return;
                ParentClass->MakeTeamMemberComment("ATTACKING",Me,false);
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
                    ParentClass->MakeTeamMemberComment("FETCHING",Me,false);
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
            ParentClass->MakeTeamMemberComment("DEFENDING",Me,false);
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
            ParentClass->MakeTeamMemberComment("FETCHING",Me,false);
        }
        if (MemberState==4 && (MyTeam->EnemyTeam->TeamCrateCarrier.IsDead() || !IsThingCarryingCrate(MyTeam->EnemyTeam->TeamCrateCarrier))) {
            if (IsActiveThreadTerminating()) return;
            SetState(0);
        }
        if (MemberState==2 && IsThingCarryingCrate(Me)) {
            if (IsActiveThreadTerminating()) return;
            SetState(5);
            MyTeam->TeamCrateCarrier=Me;
            ParentClass->MakeTeamMemberComment("REQUEST_PROTECTION",Me,false);
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
