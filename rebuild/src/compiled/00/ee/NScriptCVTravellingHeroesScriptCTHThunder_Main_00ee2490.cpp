// NScript::CV_TravellingHeroesScript::CTH_Thunder::Main() -- Thunder (the rival hero) visiting Bowerstone: alive filter on Me, keep Me persistent (removed once the gameflow is past 900), mute and unpushable, weapons away, take scripted control through the quest's rival handle, stand at TH_Trigger_Bowerstone unarmed; when the hero comes within 25 units face him, spawn the quest's ThunderChatter thread, and wait until the hero talks to Me (taunt line 10) or hits Me (taunt line 10B); then play the CS_TRAVHERO_THUNDERTAUNT cutscene, flag the scene done and remove Me
// Inline-budget: #pragma inline_depth(0) round the row-local inline CIsThingAlive(const CScriptThing&) definition + FABLE_RETAIL_CPREDICATEBASE_NOVTABLE (the landed CIsThingAlive predicate idiom, e.g. 0x00DCE230 / 0x00E5B3B0) -- retail calls the implicit CScriptThing copy 0x004ABE90 inside the expanded predicate ctor (0x00EE24AF) yet expands it at the by-value SetIsPushableByHero argument (0x00EE252B..0x00EE254E). agentNF71 tried pragma-free first: /d2inlT62 DIFFER(1352v1334), FABLE_FORCEINLINE predicate ctor DIFFER(1352v1334), user inline copy at /d2inlT40 / T30 / T20 DIFFER(1352 / 1304 / 1304 v1334) (both sites always decided together).
// Leaf: ?Main@CTH_Thunder@CV_TravellingHeroesScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EE2490, 1334 bytes, thiscall, no EH frame.
// Shared headers (NF68 port, finished by agentNF71 round 7; was 18 local NScript classes): engine/CV_TravellingHeroesScript.h (FABLE_RETAIL_NSCRIPT /
//   CLEVELSCRIPTBASE / BASE_OBJECT / CCHARSTRING + SUSPENDABLE_PROCESS; opt-ins THUNDER (PDB ThunderChatter 0x00EE29D0 / PlayCutscene(CCharString)
//   0x00EE2DB0) and THUNDER_GETMASTERDATA (FableWin.pdb ?GetMasterData@CTH_Thunder@..@@QAEPAVCQ_SunnyvaleMasterData@3@XZ, expanded: retail reads
//   PMasterData into eax at 0x00EE24EE)), CIsThingAlive.h, CQ_SunnyvaleMasterData.h, EHeroAbility.h (HERO_ABILITY_HEAL_LIFE_SPELL = 14),
//   CSpawnedFunc.h; CScriptThing's copy is implicit (FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_COPY); CCharString + CCharString 0x0099F570.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CPREDICATEBASE_NOVTABLE
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_CCHARSTRING_CONCAT_STRING
#define FABLE_RETAIL_CV_TRAVELLINGHEROESSCRIPT_THUNDER
#define FABLE_RETAIL_CV_TRAVELLINGHEROESSCRIPT_THUNDER_GETMASTERDATA
#define FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_COPY
// Ego_r.pdb EScriptAIPriority enumerators, defined ahead of the headers as the landed sibling 0x00E13310 does.
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST,SCRIPT_AI_PRIORITY_LOW,SCRIPT_AI_PRIORITY_MEDIUM,SCRIPT_AI_PRIORITY_HIGH,SCRIPT_AI_PRIORITY_HIGHEST };
#include "engine/CV_TravellingHeroesScript.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CIsThingAlive.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/EHeroAbility.h"
#include "engine/CSpawnedFuncBase.h"
#include "engine/CSpawnedFunc.h"

#pragma inline_depth(0)
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing) : Thing(thing) { }
#pragma inline_depth()

bool FABLE_FASTCALL IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF (Ego_r.pdb ?IsDistanceBetweenThingsUnder@@YI_NABVCScriptThing@@0M@Z)

void NScript::CV_TravellingHeroesScript::CTH_Thunder::Main()
{
    {
        CIsThingAlive filter(Me);
        SetPredicate(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating()) return;
    Game->SetThingPersistent(Me, true);
    if (GetMasterData()->PostSavePosition > 900) {
        if (IsActiveThreadTerminating()) return;
        Game->RemoveThing(Me, false, true);
    }
    Game->SetThingHasInformation(Me, false, false, true);
    Game->SetIsPushableByHero(Me, false);
    Game->EntitySheatheWeapons(Me, false);
    {
        CV_TravellingHeroesScript& parent = ParentClass;
        if (parent.seh_Rival.IsValid()) parent.seh_Rival.Release();
    }
    while (!Game->StartScriptingEntity(Me, ParentClass.seh_Rival, SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating()) return;
    Game->EntityTeleportToThing(Me, Game->GetThingWithScriptName("TH_Trigger_Bowerstone"), false);
    Game->AddCreatureScriptedMode(Me, "UNARMED");
    while (!IsDistanceBetweenThingsUnder(Me, Game->GetHero(), 25.0f)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    Game->EntitySetFacingAngle(Me, 0.5f, true);
    ParentClass.AddSpawnedFunction(new CSpawnedFunc<CV_TravellingHeroesScript>(CCharString("ParentClass.") + CCharString("ThunderChatter"), 0, &ParentClass, &CV_TravellingHeroesScript::ThunderChatter), CCharString(""));
    while (!ParentClass.ThunderSceneStarted) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating()) return;
            ParentClass.ThunderSceneStarted = true;
            ParentClass.csargs["$LINE1"] = "TEXT_CS_B14_THUNDERTAUNT_10";
        } else if (Me.MsgIsHitBy("SCRIPT_NAME_HERO")
                   || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO"))) {
            if (IsActiveThreadTerminating()) return;
            ParentClass.ThunderSceneStarted = true;
            ParentClass.csargs["$LINE1"] = "TEXT_CS_B14_THUNDERTAUNT_10B";
        }
    }
    if (IsActiveThreadTerminating()) return;
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating()) return;
    ParentClass.PlayCutscene("CS_TRAVHERO_THUNDERTAUNT");
    ParentClass.ThunderSceneDone = true;
    Game->RemoveThing(Me, false, true);
}
