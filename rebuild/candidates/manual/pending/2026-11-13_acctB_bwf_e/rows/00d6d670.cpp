// NScript::CQ_HangingTreeEvilScript::Conclusion() -- Hanging Tree (evil) quest conclusion: re-acquires (or respawns) the
//   prisoner and executioner, spawns the two guards and a freedom fighter, plays the OUTRO cut-scene, then waits until the
//   executioner is interrupted (hit / killed / hero close) or the mission timer runs out; the execution path plays
//   CS_HANGINGTREE_EVIL_EXECUTION and fails the quest, the escape path kills the executioner and plays CS_HANGINGTREE_EVIL_ESCAPE.
// Leaf: ?Conclusion@CQ_HangingTreeEvilScript@NScript@@QAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D6D670, 5786 bytes, thiscall, plain `ret`; frame 0xFC, no SEH, ebx = 0 throughout.
// Identity (proven): Ego_r.pdb and FableWin.pdb ?Conclusion@CQ_HangingTreeEvilScript@NScript@@QAEXXZ; the only caller is the landed
//   DoMission 0x00D6BFA0 (0x00D6C37C); Ego_r method order Introduction 0x00D6D080 -> Conclusion -> Alloc 0x00D6ED10. The manifest
//   label `_global::CQ_HangingTreeEvil_Execute` is made up.
// Callees: CCharString 0x0099EBF0 / ~ 0x0099EAE0 / operator=(const char*) 0x0099EFE0; IsActiveThreadTerminating 0x00CB7940;
//   CCountedPointer<CScriptThing>::ShareData 0x007044D0 (out of line under the implicit CScriptThing::operator=); ~CScriptThing
//   0x004AA840 (COMDAT for the six function-scope locals, expanded on temporaries: ~CBaseClass 0x0099A2E0, operator delete
//   0x00BFE9BC); CBaseObject 0x0099A380 / 0x0099A430 / 0x0099A3B0; CSGRO ctor 0x007E72A0, dtor 0x007E74D0, operator= 0x008ABD10,
//   IsValid 0x00CD23B9, Release 0x00CD2770, MoveToPosition 0x007E72F0, ClearCommands 0x007E7360; MovieBase ctor 0x006E7B60 / dtor
//   0x006E7B80, CCountedPointer<MovieBase>::~ 0x006E7AB0; map<CCharString,CSGRO> ctor 0x00CDBF70 / dtor 0x00CDBFB0 / operator[]
//   0x00CD3D2E / _M_erase 0x00CD344A, malloc 0x00BFEA0E / free 0x00BFEA14; RunCutsceneMacro 0x00CBFB7D, IsDistanceBetweenThingsUnder
//   0x00CBE2FF (__fastcall); CGameScriptInterfaceBase slots 7, 8, 70, 72, 74, 91, 92, 265, 296, 321, 325, 338, 350, 364, 366, 370,
//   371, 376, 379, 451, 597, 601, 622, 655; pDSTGame 0x0143E8F8 slot 90 GetTimer (inline CTimer::GetTime); CScriptThing slots 6,
//   18, 21, 75, 76. CScriptThing::GetPos / IsAlive bodies are visible: retail expands both inline (the null test on PImp.Data
//   then slot 6 / slot 75 of the pointee; no call to the out-of-line 0x004AA980 or 0x004AB130 anywhere in the 0x16AA bytes); the
//   bodies copy the landed rows CScriptThing_GetPos_004aa980.cpp (NullCoord when empty) and CScriptThing_IsAlive_004ab130.cpp.
// Shape: the inline-v-COMDAT split of the CSGRO / MovieBase / cast-map / ~CScriptThing members (expanded early, called late) is
//   VC7.1's own inline budget in this 5.8 KB body once every body is visible; the map needs STLport's separate no-argument
//   constructor (FABLE_RETAIL_STLP_MAP: retail passes only `this` to 0x00CDBF70); the wait loop is a top-tested do/while (retail
//   tests ExecutionerInterrupted at the head and loops on !timeUp at the bottom); the escape cut-scene's locals sit in an inner
//   scope (MissionSucceeded is stored after their destructors). Each of the three cut-scene blocks copies Game into a local
//   (`CGameScriptInterfaceBase* game = Game;`) because retail loads it once into edi and reuses it for both slot-379
//   PauseAllNonScriptedEntities calls without reloading [this+0x40] after RunCutsceneMacro: mov edi,[esi+0x40] at 0x00D6E333,
//   calls 0x00D6E33C / 0x00D6E372; 0x00D6E99F, calls 0x00D6E9A8 / 0x00D6E9DE; 0x00D6EBC3, calls 0x00D6EBCC / 0x00D6EC1B. The same
//   spelling is in the landed PlayCutscene rows (e.g. NScript::CQ_HeroSoulsMotherScript::PlayCutscene 0x00D848D0). Header overlay: FABLE_RETAIL_CQ_HANGINGTREEEVIL_CONCLUSION declares
//   Conclusion() in engine/CQ_HangingTreeEvilScript.h. Local names are descriptive (unusedHandle: a fifth handle retail builds and
//   destroys but never uses).
#include "fable_compiler.h"
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_ASSIGNMENT
#define FABLE_RETAIL_CQ_HANGINGTREEEVIL_CONCLUSION
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/C3DVector.h"
#include "engine/CRGBColour.h"
#include "stlport/fable_map.h"
#include "engine/CQ_HangingTreeEvilScript.h"
#include "engine/CBaseObject.h"
#include "engine/CScriptGameResourceObjectBase.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CGameScriptInterfaceBase.h"
template <> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();   // 0x006E7AB0
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST = 0, SCRIPT_AI_PRIORITY_LOW = 1, SCRIPT_AI_PRIORITY_MEDIUM = 2,
                         SCRIPT_AI_PRIORITY_HIGH = 3, SCRIPT_AI_PRIORITY_HIGHEST = 4 };
template <> void CCountedPointer<CScriptThing>::ShareData(CScriptThing* data, CCPPointerInfo* info);   // 0x007044D0
inline CScriptThing::~CScriptThing() { }
FABLE_FORCEINLINE const C3DVector& CScriptThing::GetPos() const { return !PImp.Data ? NullCoord : PImp.Data->GetPos(); }
inline bool CScriptThing::IsAlive() const { return PImp.Data && PImp.Data->IsAlive(); }
extern CGameScriptInterfaceBase* pDSTGame;   // 0x0143E8F8
FABLE_FORCEINLINE long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
bool FABLE_FASTCALL IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF
void FABLE_FASTCALL RunCutsceneMacro(const CCharString& name, std::map<CCharString, CScriptGameResourceObjectScriptedThingBase>& cast,
                                     std::map<CCharString, bool>* flags, std::map<CCharString, CCharString>* args, bool a5, bool a6);   // 0x00CBFB7D

namespace NScript {
void CQ_HangingTreeEvilScript::Conclusion()
{
    Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_HANGING_TREE_EVIL_OBJECTIVE_02", "ExecutionTree", "BowerstoneSlums");
    Game->EntitySetAsOpinionSource(Game->GetHero(), "OPINION_SOURCE_HERO_EVIL_HIGH");
    bool timeUp = false;
    Prisoner = Game->GetThingWithScriptName("HTE_Prisoner");
    if (Prisoner.IsDead()) {
        CScriptThing marker = Game->GetThingWithScriptName("MK_HTE_OUTRO_PRIS1");
        Prisoner = Game->CreateCreature("CREATURE_PRISONER", marker.GetPos(), "HTE_Prisoner", false);
        Game->MiniMapAddMarker(Prisoner, "HUD_ORB_GREEN_SMALL");
    }
    Executioner = Game->GetThingWithScriptName("HTE_Executioner");
    if (Executioner.IsDead()) {
        CScriptThing marker = Game->GetThingWithScriptName("MK_HTE_OUTRO_EXEC");
        Executioner = Game->CreateCreature("CREATURE_EXECUTIONER", marker.GetPos(), "HTE_Executioner", false);
    }
    Game->ModifyThingHealth(Executioner, 150.0f, false);
    CScriptThing guard1 = Game->CreateCreature("CREATURE_BS_GUARD", Game->GetThingWithScriptName("MK_HTE_GUARD1").GetPos(), "GUARD1", false);
    CScriptThing guard2 = Game->CreateCreature("CREATURE_BS_GUARD", Game->GetThingWithScriptName("MK_HTE_GUARD2").GetPos(), "GUARD2", false);
    CScriptThing freedomFighter = Game->GetRandomThingWithScriptName("HTE_FreedomFighter");
    if (!freedomFighter.IsAlive()) {
        freedomFighter = Game->CreateCreatureNearby("CREATURE_BANDIT_GRUNT", Game->GetHero().GetPos(), 3.0f, "HTE_FreedomFighter", false);
        Game->MiniMapAddMarker(freedomFighter, "HUD_ORB_GREEN_SMALL");
        Game->AddQuestInfoBarHealth(freedomFighter, CRGBColour(255, 255, 255, 255), "HUD_QUEST_ICON_BANDIT", 1.0f);
    }
    CScriptThing execMarker = Game->GetThingWithScriptName("MK_HTG_EXEC_EXEC");
    CScriptThing prisMarker = Game->GetThingWithScriptName("MK_HTG_EXEC_PRIS");
    C3DVector execPos = execMarker.GetPos();
    C3DVector prisPos = prisMarker.GetPos();
    CScriptGameResourceObjectScriptedThingBase prisonerH;
    CScriptGameResourceObjectScriptedThingBase executionerH;
    CScriptGameResourceObjectScriptedThingBase guard1H;
    CScriptGameResourceObjectScriptedThingBase guard2H;
    CScriptGameResourceObjectScriptedThingBase unusedHandle;
    Game->EntitySetInFaction(Executioner, "FACTION_VILLAGER");
    Game->EntitySetThingAsEnemyOfThing(Executioner, Game->GetHero());
    Game->StartScriptingEntity(Prisoner, prisonerH, SCRIPT_AI_PRIORITY_HIGHEST);
    Game->StartScriptingEntity(Executioner, executionerH, SCRIPT_AI_PRIORITY_HIGHEST);
    Game->StartScriptingEntity(guard1, guard1H, SCRIPT_AI_PRIORITY_HIGHEST);
    Game->StartScriptingEntity(guard2, guard2H, SCRIPT_AI_PRIORITY_HIGHEST);
    {
        CScriptGameResourceObjectScriptedThingBase hero;
        Game->StartScriptingEntity(Game->GetHero(), hero, SCRIPT_AI_PRIORITY_HIGHEST);
        std::map<CCharString, CScriptGameResourceObjectScriptedThingBase> cast;
        cast["HERO"] = hero;
        cast["PRIS"] = prisonerH;
        cast["EXEC"] = executionerH;
        cast["GUARD1"] = guard1H;
        cast["GUARD2"] = guard2H;
        CScriptGameResourceObjectMovieBase movie;
        Game->StartMovieSequence("", movie);
        CGameScriptInterfaceBase* game = Game;
        game->PauseAllNonScriptedEntities(true);
        RunCutsceneMacro("CS_HANGINGTREE_EVIL_OUTRO", cast, 0, 0, false, true);
        game->PauseAllNonScriptedEntities(false);
    }
    long conversation = Game->AddNewConversation(Executioner, false, false);
    Game->AddLineToConversation(conversation, "TEXT_QST_029_EXECUTIONER_INTRODUCTION", false, Executioner, Executioner);
    CScriptThing spawnPoint = Game->GetThingWithScriptName("HTE_PartySpawnPoint");
    Game->DisplayQuestInfo(true);
    if (!Prisoner.IsAlive()) {
        if (IsActiveThreadTerminating())
            return;
        ReasonForFailing = "TEXT_QST_029_FAILURE_PRISONER_KILLED";
        MissionFailed = true;
    }
    conversation = Game->AddNewConversation(guard1, false, false);
    Game->AddLineToConversation(conversation, "TEXT_QST_029_GUARD_SPOTTED_HERO", false, guard1, guard2);
    if (guard1H.IsValid())
        guard1H.Release();
    if (guard2H.IsValid())
        guard2H.Release();
    Game->GiveThingBestEnemyTarget(guard1, Game->GetHero());
    Game->GiveThingBestEnemyTarget(guard2, Game->GetHero());
    Game->GiveThingBestEnemyTarget(freedomFighter, guard1);
    if (IsActiveThreadTerminating())
        return;
    conversation = Game->AddNewConversation(Executioner, false, false);
    Game->AddLineToConversation(conversation, "TEXT_QST_029_EXECUTIONER_RABBLE_ROUSING", false, Executioner, Executioner);
    prisonerH.MoveToPosition(prisPos, 0.0f, (EScriptEntityMoveType)0, false, true);
    Game->Pause(1.0f);
    executionerH.MoveToPosition(execPos, 0.0f, (EScriptEntityMoveType)0, false, true);
    do {
        if (ExecutionerInterrupted)
            break;
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        if (MissionTimer.GetTime() <= 0) {
            if (IsActiveThreadTerminating())
                return;
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
            Game->Pause(1.0f);
            if (Executioner.IsAlive()) {
                if (IsActiveThreadTerminating())
                    return;
                timeUp = true;
            }
        }
        if (Executioner.MsgIsHitBy("") || Executioner.MsgIsKilledBy("")
            || IsDistanceBetweenThingsUnder(Executioner, Game->GetHero(), 3.0f)) {
            if (IsActiveThreadTerminating())
                return;
            ExecutionerInterrupted = true;
            AudienceLeggit = true;
        }
        if (Prisoner.MsgIsKilledBy("")) {
            if (IsActiveThreadTerminating())
                return;
            ReasonForFailing = "TEXT_QST_029_FAILURE_PRISONER_KILLED";
            MissionFailed = true;
        }
    } while (!timeUp);
    if (IsActiveThreadTerminating())
        return;
    if (!ExecutionerInterrupted && Executioner.IsAlive() && Prisoner.IsAlive()) {
        if (IsActiveThreadTerminating())
            return;
        CScriptGameResourceObjectScriptedThingBase hero;
        if (hero.IsValid())
            hero.Release();
        while (!Game->StartScriptingEntity(Game->GetHero(), hero, SCRIPT_AI_PRIORITY_HIGHEST)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        {
            std::map<CCharString, CScriptGameResourceObjectScriptedThingBase> cast;
            cast["HERO"] = hero;
            cast["PRIS"] = prisonerH;
            cast["EXEC"] = executionerH;
            CScriptGameResourceObjectMovieBase movie;
            Game->StartMovieSequence("", movie);
            CGameScriptInterfaceBase* game = Game;
            game->PauseAllNonScriptedEntities(true);
            RunCutsceneMacro("CS_HANGINGTREE_EVIL_EXECUTION", cast, 0, 0, false, true);
            game->PauseAllNonScriptedEntities(false);
        }
        ReasonForFailing = "TEXT_QST_029_FAILURE_PRISONER_KILLED";
        MissionFailed = true;
    } else {
        if (IsActiveThreadTerminating())
            return;
        Game->RemoveQuestInfoElement(TimerGUIIndex);
        Game->DisplayQuestInfo(false);
        if (executionerH.IsValid())
            executionerH.Release();
        prisonerH.ClearCommands();
        Game->GiveThingBestEnemyTarget(Executioner, Game->GetHero());
        while (!Executioner.IsDead()) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        prisonerH.ClearCommands();
        {
            CScriptGameResourceObjectScriptedThingBase hero;
            Game->StartScriptingEntity(Game->GetHero(), hero, SCRIPT_AI_PRIORITY_HIGHEST);
            std::map<CCharString, CScriptGameResourceObjectScriptedThingBase> cast;
            cast["HERO"] = hero;
            cast["PRIS"] = prisonerH;
            CScriptGameResourceObjectMovieBase movie;
            Game->StartMovieSequence("", movie);
            CGameScriptInterfaceBase* game = Game;
            game->PauseAllNonScriptedEntities(true);
            Game->FixMovieSequenceCamera(true);
            RunCutsceneMacro("CS_HANGINGTREE_EVIL_ESCAPE", cast, 0, 0, false, true);
            Game->FixMovieSequenceCamera(false);
            game->PauseAllNonScriptedEntities(false);
        }
        MissionSucceeded = true;
    }
    if (guard1H.IsValid())
        guard1H.Release();
    if (guard2H.IsValid())
        guard2H.Release();
    Game->DisplayQuestInfo(false);
}
}   // namespace NScript
