// NScript::CQ_HangingTreeGoodScript::DoMission() -- start the region, mission-end and hero-proximity watchers, wait for BowerstoneJail, play the opening cutscene, then run the escort state machine (1 spawn the party, 2 walk the waypoints, 5 wait for the ambush to die, 3/4 hand over at the jail) until state 7, play the final round and, unless failed, the closing cutscene
// Leaf: ?DoMission@CQ_HangingTreeGoodScript@NScript@@QAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D74430, 2216 bytes, plain `ret`. Identity: Main 0x00D6F4B0 spawns it as "DoMission"
//   (CSpawnedFunc member store 0x00D6F9A3); both PDB name sets hold ?DoMission@CQ_HangingTreeGoodScript@NScript@@QAEXXZ.
// Callees: spawned MonitorRegionPresence 0x00D773E0, WatchForMissionEnd 0x00D74CE0, MonitorHeroProximity 0x00D74D30, DoConversations
//   0x00D767F0 (thread-name strings); DoCutscene(long) 0x00D77690, UpdateWayPoint 0x00D74E80, ReassignLeader 0x00D74F80, SpawnParty
//   0x00D76030, SetGuardsHealth 0x00D766C0, GetGuardsHealth 0x00D77340 (PDB name), RemoveParty(bool)
//   0x00D732F0, FinalRound 0x00D75060; GFCharStringToInt 0x0099E7F0, AreAllThingsInVectorDead 0x00CBED00, IsDistanceBetweenThingsUnder
//   0x00CBE2FF; ~CScriptThing 0x004AA840; interface slots IsLevelLoaded 11, NewScriptFrame 7, GetHero 70, GetThingWithScriptName 72,
//   SetQuestCardObjective 296, GetActiveQuestName 655.
// Layout (engine/CQ_HangingTreeGoodScript.h, retail = Ego_r - 0x14): SwitchingRegions +0x4c, ReassignLeaderNow +0x4d, PartyDefeated
//   +0x4e, SpawnHere +0x4f, AtWayPoint +0x50, PartyReadyToTransition +0x53, HTG_State +0x5c, CurrentWayPoint +0x60, Leader +0x68,
//   PartySpawnPoint +0x80, CurrentEnemyVector +0xa0.
// Shape: generated script code (a terminate check after every statement; an empty `else` keeps only its check). The top-level
//   PartySpawnPoint assignment expands CScriptThing::operator= but calls ~CScriptThing; the in-loop one expands both. Plain `inline` and
//   the implicit operator= leave the top-level site a call to ??4CScriptThing (0x008AB980 exists out of line): this row needs the
//   `__forceinline` TU definition (owner 2026-11-01 rule: recorded). ~CScriptThing stays plain inline.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_CQ_HANGINGTREEGOODSCRIPT
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CSpawnedFunc.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CQ_HangingTreeGoodScript.h"
#include "engine/fable_script_distance.h"
inline CScriptThing::~CScriptThing() { }
__forceinline CScriptThing& CScriptThing::operator=(const CScriptThing& other) { PImp = other.PImp; return *this; }
long FABLE_FASTCALL GFCharStringToInt(const CCharString&);                                                      // 0x0099E7F0
bool FABLE_FASTCALL AreAllThingsInVectorDead(const std::vector<CScriptThing,std::allocator<CScriptThing> >&);  // 0x00CBED00
void NScript::CQ_HangingTreeGoodScript::DoMission()
{
    AddSpawnedFunction(new CSpawnedFunc<CQ_HangingTreeGoodScript>("MonitorRegionPresence", 0, this, &CQ_HangingTreeGoodScript::MonitorRegionPresence), CCharString(""));
    AddSpawnedFunction(new CSpawnedFunc<CQ_HangingTreeGoodScript>("WatchForMissionEnd", 0, this, &CQ_HangingTreeGoodScript::WatchForMissionEnd), CCharString(""));
    AddSpawnedFunction(new CSpawnedFunc<CQ_HangingTreeGoodScript>("MonitorHeroProximity", 0, this, &CQ_HangingTreeGoodScript::MonitorHeroProximity), CCharString(""));
    while (!Game->IsLevelLoaded("BowerstoneJail")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    DoCutscene(0);
    PartySpawnPoint = Game->GetThingWithScriptName("HTG_PartySpawnPoint");
    HTG_State = 1;
    Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_HANGING_TREE_GOOD_OBJECTIVE_01", "ExecutionTree", "BowerstoneSlums");
    UpdateWayPoint();
    while (HTG_State != 7) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
        while (HTG_State == 1) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
            while (SwitchingRegions) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return;
            }
            if (IsActiveThreadTerminating()) return;
            if (!PartyDefeated) {
                if (IsActiveThreadTerminating()) return;
                if (SpawnHere) {
                    PartySpawnPoint = Game->GetThingWithScriptName("HTG_PartySpawnPoint");
                    CurrentWayPoint = GFCharStringToInt(PartySpawnPoint.GetDataString());
                    SpawnParty(CurrentGuards, PartySpawnPoint);
                    SetGuardsHealth();
                    AddSpawnedFunction(new CSpawnedFunc<CQ_HangingTreeGoodScript>("DoConversations", 0, this, &CQ_HangingTreeGoodScript::DoConversations), CCharString(""));
                }
                HTG_State = 2;
            } else {
                if (IsActiveThreadTerminating()) return;
            }
        }
        if (IsActiveThreadTerminating()) return;
        while (HTG_State == 2) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
            if (ReassignLeaderNow) {
                if (IsActiveThreadTerminating()) return;
                PartySuccessfullySetUp = false;
                if (SwitchingRegions) {
                    if (IsActiveThreadTerminating()) return;
                    HTG_State = 1;
                } else {
                    if (IsActiveThreadTerminating()) return;
                    ReassignLeader();
                }
            } else if (AtWayPoint) {
                if (IsActiveThreadTerminating()) return;
                ++CurrentWayPoint;
                if (!UpdateWayPoint()) {
                    if (IsActiveThreadTerminating()) return;
                    if (CurrentWayPoint > 14) {
                        if (IsActiveThreadTerminating()) return;
                        HTG_State = 7;
                    } else {
                        if (IsActiveThreadTerminating()) return;
                        PartyReadyToTransition = true;
                    }
                }
                AtWayPoint = false;
            }
            if (SwitchingRegions) {
                if (IsActiveThreadTerminating()) return;
                HTG_State = 1;
            }
        }
        if (IsActiveThreadTerminating()) return;
        while (HTG_State == 5) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
            PartyReadyToTransition = false;
            while (!AreAllThingsInVectorDead(CurrentEnemyVector)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return;
            }
            if (IsActiveThreadTerminating()) return;
            HTG_State = 2;
        }
        if (IsActiveThreadTerminating()) return;
        while (HTG_State == 3 || HTG_State == 4) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
            if (HTG_State == 4) {
                if (IsActiveThreadTerminating()) return;
            }
            if (HTG_State == 3) {
                if (IsActiveThreadTerminating()) return;
                while (!IsDistanceBetweenThingsUnder(Game->GetHero(), Leader, 8.0f)) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return;
                }
                if (IsActiveThreadTerminating()) return;
                GetGuardsHealth();
                RemoveParty(true);
                PartyReadyToTransition = true;
                while (!SwitchingRegions) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return;
                }
                if (IsActiveThreadTerminating()) return;
            } else {
                if (IsActiveThreadTerminating()) return;
                PartyReadyToTransition = false;
            }
            if (SwitchingRegions) {
                if (IsActiveThreadTerminating()) return;
                HTG_State = 1;
            }
        }
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    FinalRound();
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating()) return;
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating()) return;
    if (MissionFailed) return;
    DoCutscene(1);
    MissionSucceeded = true;
}