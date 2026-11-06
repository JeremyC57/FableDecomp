// NScript::CQ_FireHeartScript::Main() -- the Fire Heart quest main: bind the ten entity scripts, set the first objective, wait for Lookout Point, make its exit persistent and active, spawn the theme / villager-comment / game-ending threads, play the intro once the prophets are scripted, run the puzzle rounds until the mission ends, reward the hero, return him to Lookout Point, close the demon door, play the exit cutscene and complete the quest
// Leaf: ?Main@CQ_FireHeartScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D35090, 3641 bytes; RELOCATION_MATCH, every relocation bound-same, 43 string literals equal.
// Identity: CQ_FireHeartScript vtable slot (.rdata 0x012CCB98) -> ?Main@CQ_FireHeartScript@NScript@@UAEXXZ (Ego_r.pdb + FableWin.pdb).
// Callees: CScriptBase IsActiveThreadTerminating 0x00CB7940 / AddSpawnedFunction 0x00CB7E50 / AddEntityScriptBinding 0x00CB8230 /
//   PostAddScriptedEntities 0x00CB8930; CSpawnedFuncBase ctor 0x00CDD450; CCountedPointer<CScriptThing>::operator= 0x00704580 (called,
//   not expanded: declared-only explicit specialization); ten nested Alloc statics 0x00D362B0..0x00D37C30 (Ego_r names); own
//   RunThemeOverriding 0x00D35ED0, CheckGameEnding 0x00D360E0, SetupPuzzleBlocks 0x00D36140, RestoreFollowers 0x00D361F0,
//   RunVillagerComments 0x00D38D20, PlayCutscene 0x00D39330, StartRound 0x00D39960, DisableFollowers 0x00D3A050; operator new 0x00BFEA1A;
//   vtables CSpawnedFunc<CQ_FireHeartScript> 0x012CCBA4, CEntityScriptBinding<CQ_FireHeartScript> 0x012CCE90; pGlobalScriptDef 0x0143E90C.
// Layout: engine/CQ_FireHeartScript.h (new, 0x1A4 = Ego_r 0x1B8 less 0x14, minus 4 per STLport container after it).
// Shape: AT-Main binding model with binding flag 0 at +0x18; the if/else on JackBossBattleResult with two PlayCutscene calls is
//   tail-merged as retail (a ternary gives mov/je).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CEntityScriptBase.h"
#include "stlport/fable_map.h"
#include "engine/CQ_FireHeartScript.h"
#include "engine/CEntityScriptBinding.h"
#include "engine/CSpawnedFuncBase.h"
#include "engine/CSpawnedFunc.h"
#include "engine/C3DVector.h"
#include "engine/fable_script_def_global.h"

namespace NScript {
#define FABLE_FH_ENTITY(Name) \
    class CQ_FireHeartScript::Name : public CEntityScriptBase { \
    public: \
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_FireHeartScript&, CScriptDataBase*, const CScriptThing&); \
    };
FABLE_FH_ENTITY(CDF_Fireheart)               // Alloc 0x00D362B0
FABLE_FH_ENTITY(CPuzzleBlockMoon)            // Alloc 0x00D363A0
FABLE_FH_ENTITY(CPuzzleBlockSun)             // Alloc 0x00D36470
FABLE_FH_ENTITY(CProphetCage)                // Alloc 0x00D36680
FABLE_FH_ENTITY(CFH_Prisoner)                // Alloc 0x00D36540
FABLE_FH_ENTITY(CFH_BriarRose)               // Alloc 0x00D37240
FABLE_FH_ENTITY(CFH_GuildMaster)             // Alloc 0x00D36790
FABLE_FH_ENTITY(CFH_Scythe)                  // Alloc 0x00D37140
FABLE_FH_ENTITY(CFH_Villager)                // Alloc 0x00D373D0
FABLE_FH_ENTITY(CFH_MemorialCutsceneMarker)  // Alloc 0x00D37C30
#undef FABLE_FH_ENTITY
}
inline CScriptThing::~CScriptThing() { }
template<> CCountedPointer<CScriptThing>& CCountedPointer<CScriptThing>::operator=(const CCountedPointer<CScriptThing>& other);   // 0x00704580
inline CScriptThing& CScriptThing::operator=(const CScriptThing& other) { PImp = other.PImp; return *this; }

void NScript::CQ_FireHeartScript::Main()
{
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_FireHeartScript>("DF_Fireheart", this, &CDF_Fireheart::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_FireHeartScript>("PuzzleBlockMoon", this, &CPuzzleBlockMoon::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_FireHeartScript>("PuzzleBlockSun", this, &CPuzzleBlockSun::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_FireHeartScript>("ProphetCage", this, &CProphetCage::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_FireHeartScript>("FH_Prisoner", this, &CFH_Prisoner::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_FireHeartScript>("FH_BriarRose", this, &CFH_BriarRose::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_FireHeartScript>("FH_GuildMaster", this, &CFH_GuildMaster::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_FireHeartScript>("FH_Scythe", this, &CFH_Scythe::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_FireHeartScript>("FH_Villager", this, &CFH_Villager::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_FireHeartScript>("FH_MemorialCutsceneMarker", this, &CFH_MemorialCutsceneMarker::Alloc, 0));
    PostAddScriptedEntities();
    Game->PostAddScriptedEntities();
    Game->SetBoastingEnabled(false);
    Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_FIRE_HEART_OBJECTIVE_01", "LookoutPoint", "HeroGuildComplexInside");
    while (!Game->IsLevelLoaded("LookoutPoint")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->SetThingPersistent(Game->GetThingWithScriptName("FireHeartExit"), true);
    Game->SetRegionExitAsActive(Game->GetThingWithScriptName("FireHeartExit"), true);
    Game->FadeScreenOutUntilNextCallToFadeScreenIn(0.5f, 0.0f);
    AddSpawnedFunction(new CSpawnedFunc<CQ_FireHeartScript>("RunThemeOverriding", 0, this, &CQ_FireHeartScript::RunThemeOverriding), CCharString(""));
    VillagerEscapeMarker = Game->GetThingWithScriptName("FH_VillagerEscapeMarker");
    Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_FIRE_HEART_OBJECTIVE_02", "LookoutPoint", "HeroGuildComplexInside");
    AddSpawnedFunction(new CSpawnedFunc<CQ_FireHeartScript>("RunVillagerComments", 0, this, &CQ_FireHeartScript::RunVillagerComments), CCharString(""));
    AddSpawnedFunction(new CSpawnedFunc<CQ_FireHeartScript>("CheckGameEnding", 0, this, &CQ_FireHeartScript::CheckGameEnding), CCharString(""));
    if (!Game->GetThingWithScriptName("FH_SCYTHE").IsAlive()) {
        if (IsActiveThreadTerminating())
            return;
        Game->FadeScreenIn();
    }
    while (!Game->IsLevelLoaded("DemonDoor_LookoutPoint")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->FadeScreenOutUntilNextCallToFadeScreenIn(0.5f, 0.0f);
    SetupPuzzleBlocks();
    DisableFollowers();
    Game->MiniMapSetAsEnabled(false);
    while (ProphetsScripted < 5) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    PlayCutscene("CS_FIREHEART_INTRO");
    if (!QuestStartScreenShown) {
        if (IsActiveThreadTerminating())
            return;
        Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_FIRE_HEART_OBJECTIVE_03", "", "HeroGuildComplexInside");
        Game->KickOffQuestStartScreen(Game->GetActiveQuestName(), true, false);
    }
    Game->ConfiscateAllHeroWeapons();
    Game->SetHeroWillAsUsable(false);
    Game->SetTeleportingAsActive(false);
    StartRound();
    while (!MissionSucceeded && !MissionFailed) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    if (MissionSucceeded) {
        if (IsActiveThreadTerminating())
            return;
        Game->GiveHeroObject("OBJECT_LPDD_FIREHEART_01", -1, false);
        Game->ReturnAllConfiscatedItemsToHero();
        Game->SetHeroWillAsUsable(true);
        Game->SetTeleportingAsActive(true);
        RestoreFollowers();
        Game->SetBoastingEnabled(false);
    }
    Game->AddQuestRegion(Game->GetActiveQuestName(), "LookoutPoint");
    Game->EntityTeleportToPosition(Game->GetHero(), Game->GetThingWithScriptName("FH_QuestExit").GetPos(), 0.0f, false, false);
    while (!Game->IsLevelLoaded("LookoutPoint")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->RemoveThing(Game->GetThingWithScriptName("FH_GuildMaster"), false, true);
    Game->FadeScreenIn();
    CScriptThing door = Game->GetThingWithScriptName("DD_Fireheart");
    Game->SetThingAsUsable(door, false);
    Game->CloseDoor(door);
    if (MasterScriptData->JackBossBattleResult == 1)
        PlayCutscene("CS_EXIT_DEMONDOOR_HAVESWORD");
    else
        PlayCutscene("CS_EXIT_DEMONDOOR_NOSWORD");
    if (KilledAllPrisoners) {
        if (IsActiveThreadTerminating())
            return;
        Game->GiveHeroMorality(pGlobalScriptDef->FHKillAllPrisonersMoralityChange);
        Game->GiveHeroRenownPoints(pGlobalScriptDef->FHKillAllPrisonersRenownBoost);
    } else if (FreedAllPrisoners) {
        if (IsActiveThreadTerminating())
            return;
        Game->GiveHeroMorality(pGlobalScriptDef->FHFreeAllPrisonersMoralityChange);
        Game->GiveHeroRenownPoints(pGlobalScriptDef->FHFreeAllPrisonersRenownBoost);
    }
    Game->SetBoastingEnabled(true);
    Game->MiniMapRemoveMarker(door);
    Game->MiniMapSetAsEnabled(true);
    Game->SetQuestAsCompleted(Game->GetActiveQuestName(), true, false, false);
    Game->DeactivateQuestLater(Game->GetActiveQuestName(), 0);
}