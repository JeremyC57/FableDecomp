// NScript::CV_AmbushScamScript::SpringAmbush() -- wait until the living trickster nears the hero, then spring the ambush: six bandits (good variant) or six guards (evil variant) around the two spawn points, the first two opening a conversation with the ambush line, the all-dead watcher; record a trickster killed before or after the ambush
// Leaf: ?SpringAmbush@CV_AmbushScamScript@NScript@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E2F170, 2781 bytes, plain `ret`.
// Grade: DIFFER(2163v2781) -- mixed ~CScriptThing: retail expands the dtor at the top-of-loop exits of both rotated loops and on the two post-loop paths, and calls 0x004AA840 at the ambush exits and for the creature temporaries; plain inline gives all calls, __forceinline all expansions (3743 bytes); inline_depth family
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
#include "stlport/fable_containers.h"
#include "engine/CV_AmbushScamScript.h"
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CSpawnedFunc.h"
#include "engine/fable_script_distance.h"
#include "engine/fable_script_def_global.h"
inline CScriptThing::~CScriptThing() { }
inline bool CScriptThing::IsAlive() const { if (!PImp.Data) return false; return PImp.Data->IsAlive(); }
inline const C3DVector& CScriptThing::GetPos() const { if (PImp.IsNull()) return NullCoord; return PImp->GetPos(); }

void NScript::CV_AmbushScamScript::SpringAmbush()
{
    CScriptThing run_away = Game->GetThingWithScriptName("M_AS_TricksterRunAway");
    CScriptThing trickster = GetTrickster();
    for (;;) {
        do { if (IsActiveThreadTerminating()) return; } while (0);
        if (!trickster.IsAlive() || Game->GetHealth(trickster) <= 0.0001f)
            break;
        if (IsDistanceBetweenThingsUnder(trickster, Game->GetHero(), pGlobalScriptDef->AmbushScamAmbushTriggerProximity)) {
            do { if (IsActiveThreadTerminating()) return; } while (0);
            do { if (IsActiveThreadTerminating()) return; } while (0);
            DoAmbush = true;
            HitCount = 0;
            CScriptThing spawn_point1 = Game->GetThingWithScriptName("M_AS_AmbusherSpawnPoint1");
            CScriptThing spawn_point2 = Game->GetThingWithScriptName("M_AS_AmbusherSpawnPoint2");
            if (MoralitySwitch) {
                do { if (IsActiveThreadTerminating()) return; } while (0);
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
                do { if (IsActiveThreadTerminating()) return; } while (0);
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
            AddSpawnedFunction(new CSpawnedFunc<CV_AmbushScamScript>("WatchForAllAmbushersDead", 0, this, &CV_AmbushScamScript::WatchForAllAmbushersDead), "");
            for (;;) {
                do { if (IsActiveThreadTerminating()) return; } while (0);
                if (AllAmbushersDead) {
                    do { if (IsActiveThreadTerminating()) return; } while (0);
                    break;
                }
                if (!trickster.IsAlive() || Game->GetHealth(trickster) <= 0.0001f) {
                    do { if (IsActiveThreadTerminating()) return; } while (0);
                    TricksterKilledAfterAmbush = true;
                    break;
                }
                Game->NewScriptFrame();
            }
            do { if (IsActiveThreadTerminating()) return; } while (0);
            return;
        }
        Game->NewScriptFrame();
    }
    do { if (IsActiveThreadTerminating()) return; } while (0);
    TricksterKilledBeforeAmbush = true;
}
