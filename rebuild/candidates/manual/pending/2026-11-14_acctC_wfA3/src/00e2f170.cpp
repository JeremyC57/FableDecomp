// NScript::CV_AmbushScamScript::SpringAmbush() -- wait (per script frame) until the hero comes within the ambush trigger proximity of the
//   trickster's run-away point while the trickster is alive, then spring the ambush: six bandits (good variant) or six guards (evil
//   variant) around the two spawn points, the first two opening a conversation with the ambush line, the all-dead watcher thread;
//   wait until all ambushers are dead or the trickster dies (TricksterKilledAfterAmbush); a trickster dead before the ambush sets
//   TricksterKilledBeforeAmbush
// Leaf: ?SpringAmbush@CV_AmbushScamScript@NScript@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E2F170, 2781 bytes, plain `ret`.
// Identity: Ego_r.pdb + FableWin.pdb name sets ?SpringAmbush@CV_AmbushScamScript@NScript@@QAEXXZ; its only caller is the landed
//   CV_AmbushScamScript::Main 0x00E2B090 (call at 0x00E2B793).
// Shape: generated script -- `TRetVoid _Fail_Val` returns, `for (;; Game->NewScriptFrame())` loops with the terminate check at the
//   top (the CNOVI_Guard::Main / generator model; c2 then expands ~CScriptThing exactly at retail's sites: both loop-top exits and the
//   post-loop path, called elsewhere), and the spawn written `AddSpawnedFunction(new CSpawnedFunc<..>(..), CCharString(""))` as in the
//   341 landed spawn sites (the explicit temporary is built after the new-expression, as retail 0x00E2F93C..0x00E2F990).
//   The trigger distance is measured from run_away (retail 0x00E2F218 `lea ecx,[esp+24h]` = the run-away point, not the trickster).
// Callees: CCharString (const char*, long) 0x0099EBF0 / ~ 0x0099EAE0, GetTrickster 0x00E2EC30, IsActiveThreadTerminating
//   0x00CB7940, IsDistanceBetweenThingsUnder 0x00CBE2FF (fastcall), ~CScriptThing 0x004AA840, ~CBaseClass 0x0099A2E0,
//   operator new 0x00BFEA1A / delete 0x00BFE9BC, CSpawnedFuncBase(const CCharString&, unsigned long) 0x00CDD450,
//   AddSpawnedFunction 0x00CB7E50; interface slots 7 NewScriptFrame, 70 GetHero, 72 GetThingWithScriptName, 92
//   CreateCreatureNearby, 264 GetHealth, 364..366 AddNewConversation / AddPersonToConversation / AddLineToConversation;
//   thing slots 6 GetPos, 75 IsAlive; pGlobalScriptDef 0x0143E90C (AmbushScamAmbushTriggerProximity +0x290); 0.0001f
//   0x0129BA3C.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_ASSIGNMENT
#include "stlport/fable_containers.h"
#include "engine/CV_AmbushScamScript.h"
#include "engine/CScriptThing.h"
#include "engine/TRetVoid.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CSpawnedFunc.h"
#include "engine/fable_script_distance.h"
#include "engine/fable_script_def_global.h"
inline CScriptThing::~CScriptThing() { }
inline bool CScriptThing::IsAlive() const { return PImp.Data && PImp.Data->IsAlive(); }
inline const C3DVector& CScriptThing::GetPos() const { if (!PImp.Data) return NullCoord; return PImp.Data->GetPos(); }

void NScript::CV_AmbushScamScript::SpringAmbush()
{
    TRetVoid _Fail_Val;
    CScriptThing run_away = Game->GetThingWithScriptName("M_AS_TricksterRunAway");
    CScriptThing trickster = GetTrickster();
    for (;; Game->NewScriptFrame()) {
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
        if (!trickster.IsAlive() || Game->GetHealth(trickster) <= 0.0001f)
            break;
        if (IsDistanceBetweenThingsUnder(run_away, Game->GetHero(), pGlobalScriptDef->AmbushScamAmbushTriggerProximity)) {
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            DoAmbush = true;
            HitCount = 0;
            CScriptThing spawn_point1 = Game->GetThingWithScriptName("M_AS_AmbusherSpawnPoint1");
            CScriptThing spawn_point2 = Game->GetThingWithScriptName("M_AS_AmbusherSpawnPoint2");
            if (MoralitySwitch) {
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
                CScriptThing ambusher1 = Game->CreateCreatureNearby("CREATURE_BANDIT_GRUNT_LEVEL3_A", spawn_point1.GetPos(), 1.0f, "AS_Ambusher", false);
                CScriptThing ambusher2 = Game->CreateCreatureNearby("CREATURE_BANDIT_GRUNT_LEVEL3_B", spawn_point1.GetPos(), 1.0f, "AS_Ambusher", false);
                Game->CreateCreatureNearby("CREATURE_BANDIT_LEADER_LEVEL3", spawn_point1.GetPos(), 1.0f, "AS_Ambusher", false);
                Game->CreateCreatureNearby("CREATURE_BANDIT_LEADER_LEVEL3", spawn_point2.GetPos(), 1.0f, "AS_Ambusher", false);
                Game->CreateCreatureNearby("CREATURE_BANDIT_GRUNT_LEVEL3_C", spawn_point2.GetPos(), 1.0f, "AS_Ambusher", false);
                Game->CreateCreatureNearby("CREATURE_BANDIT_GRUNT_LEVEL3_D", spawn_point2.GetPos(), 1.0f, "AS_Ambusher", false);
                long conversation = Game->AddNewConversation(ambusher1, false, false);
                Game->AddPersonToConversation(conversation, ambusher2);
                Game->AddLineToConversation(conversation, "TEXT_QST_002_AMBUSH_GOOD", false, ambusher1, ambusher2);
            } else {
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
                CScriptThing ambusher1 = Game->CreateCreatureNearby("CREATURE_BS_GUARD_RED", spawn_point1.GetPos(), 1.0f, "AS_Ambusher", false);
                CScriptThing ambusher2 = Game->CreateCreatureNearby("CREATURE_BS_GUARD", spawn_point1.GetPos(), 1.0f, "AS_Ambusher", false);
                Game->CreateCreatureNearby("CREATURE_BS_GUARD", spawn_point1.GetPos(), 1.0f, "AS_Ambusher", false);
                Game->CreateCreatureNearby("CREATURE_BS_GUARD_RED", spawn_point2.GetPos(), 1.0f, "AS_Ambusher", false);
                Game->CreateCreatureNearby("CREATURE_BS_GUARD", spawn_point2.GetPos(), 1.0f, "AS_Ambusher", false);
                Game->CreateCreatureNearby("CREATURE_BS_GUARD", spawn_point2.GetPos(), 1.0f, "AS_Ambusher", false);
                long conversation = Game->AddNewConversation(ambusher1, false, false);
                Game->AddPersonToConversation(conversation, ambusher2);
                Game->AddLineToConversation(conversation, "TEXT_QST_002_AMBUSH_EVIL", false, ambusher1, ambusher2);
            }
            AddSpawnedFunction(new CSpawnedFunc<CV_AmbushScamScript>("WatchForAllAmbushersDead", 0, this, &CV_AmbushScamScript::WatchForAllAmbushersDead), CCharString(""));
            for (;; Game->NewScriptFrame()) {
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
                if (AllAmbushersDead) {
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                    break;
                }
                if (!trickster.IsAlive() || Game->GetHealth(trickster) <= 0.0001f) {
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                    TricksterKilledAfterAmbush = true;
                    break;
                }
            }
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            return _Fail_Val.Return();
        }
    }
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
    TricksterKilledBeforeAmbush = true;
}
