// NScript::CQ_EndGameFocalSitesScript::Main() -- the end-game focal sites quest main: make sure the guild master quest runs, wait for the guild complex, switch its apprentices / guards / doors off, theme and music for the end of the world, wait for Witchwood1, bind the eight entity scripts, then for each of the four focal-site regions wait for its region, set the objective, have the guild master speak, and keep the battle generator teleporting next to the hero and spawning guard / minion families until the region's end cutscene has played; finally wait for the guild complex again, re-enable the creature generators, give the closing guild master line and complete or fail the quest
// Leaf: ?Main@CQ_EndGameFocalSitesScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D2AA40, 5860 bytes (incl. the 4-entry switch table 0x00D2C114), thiscall, plain `ret`.
// Identity (proven): the only reference is .rdata 0x012CBEBC, slot 2 of the level-script vtable 0x012CBEB4 (slot 1 the landed
//   RegisterMain 0x00D2A950, slot 3 the landed Init 0x00D2A8A0, slot 4 the landed OnPersist 0x00D2AA10); Ego_r.pdb and FableWin.pdb
//   name sets both hold ?Main@CQ_EndGameFocalSitesScript@NScript@@UAEXXZ (public virtual).
// Callees: operator new 0x00BFEA1A, CCharString(const char*, long) 0x0099EBF0 / copy 0x0099EC30 / ~ 0x0099EAE0, CWideString(const
//   unsigned short*) 0x0099B6B0 / ~ 0x0099B510, ~CScriptThing 0x004AA840, ~vector<CScriptThing> 0x008AC970 (the exit copies; the
//   natural end of each loop pass expands it), rand 0x00BFEB16, C3DVector::RotateXY 0x00A14040, CScriptBase::AddEntityScriptBinding
//   0x00CB8230 / PostAddScriptedEntities 0x00CB8930 / IsActiveThreadTerminating 0x00CB7940; the eight bound Allocs (CEndGameFocalSite
//   0x00D2C140, CEndGameJackOfBlades 0x00D2C260, CEndGameScythe 0x00D2C650, CEndGameScarletRobe 0x00D2CF80, CEndGameThunder 0x00D2C730,
//   CPermanentBarrier 0x00D2D730 (Ego_r + FableWin ?Alloc@CPermanentBarrier@CQ_EndGameFocalSitesScript@NScript@@SI..; it news 0x34 and
//   runs the landed CPermanentBarrier ctor 0x00D2D7B0), CBattleMinion 0x00D2C340, CBattleGuard 0x00D2C430), each binding name the class
//   name less its `C`; CScriptThing slots 6 GetPos / 75 IsAlive; CGameScriptInterfaceBase slots 7 NewScriptFrame, 12 IsRegionLoaded,
//   64 PostAddScriptedEntities, 70 GetHero, 72 GetThingWithScriptName, 75 GetAllThingsWithScriptName, 81 GetNearestWithDefName,
//   108 RemoveThing, 134 SetGuardsIgnoreCrimes, 274 HeroReceiveMessageFromGuildMaster, 276 ActivateQuest, 281 DeactivateQuestLater,
//   284 IsQuestActive, 288 SetQuestAsCompleted, 289 SetQuestAsFailed, 296 SetQuestCardObjective, 374 FadeScreenIn, 376 Pause,
//   422 GetGameAngleXY, 443 EnableGuards, 444 EnableVillagerDefTypes, 447 SetThingAsUsable, 473 EntityTeleportToPosition,
//   546-548 / 551 the CreatureGenerator calls, 558 SetCreatureGeneratorsEnabled, 655 GetActiveQuestName, 656 TransitionToTheme,
//   657 ResetToDefaultTheme, 662 SetEnvironmentThemeWeightAllChannels, 696 OverrideMusic (MUSIC_SET_BOSS = 23), 697 StopOverrideMusic.
// Layout: engine/CQ_EndGameFocalSitesScript.h (CurrentFocalSiteRegion +0x48, RegionDialogueNames +0x60, MissionFailed +0x80,
//   JackLeaves +0x81, EndCutScenePlayed +0x82, counter +0x84, JackReached +0x90) with chunk 805's JACKOFBLADES opt-in.
// Shape: the landed quest-Main binding model (flag word for the name temporaries; binding flags 0,1,1,1,1,0,0,0 as retail stores
//   them; FORCEINLINE binding ctor, which this long body otherwise calls). Each WAIT is `while (!IsRegionLoaded) { NSF; T }` + T.
//   The battle offset is summed in place (`offset += GetHero().GetPos()`: retail stores X, Y, Z back into the RotateXY'd vector) and
//   the random angle is its own statement (in one expression VC7.1 loads Game before rand and keeps it in ebx: a4/a5 31 lines).
//   The switch on CurrentFocalSiteRegion (cases 1-4) is retail's jump table 0x00D2C114.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CENTITYSCRIPTBINDING_FORCEINLINE_CTOR
#define FABLE_RETAIL_CQ_ENDGAMEFOCALSITESSCRIPT_JACKOFBLADES
extern "C" int __cdecl rand();                                          // 0x00BFEB16 (MSVCR71)
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/C3DVector.h"
#include "engine/CScriptThing.h"
#include "engine/EMusicSetType.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CQ_EndGameFocalSitesScript.h"
#include "engine/CEntityScriptBinding.h"

namespace NScript {
#define FABLE_EGFS_ENTITY(Name) \
    class CQ_EndGameFocalSitesScript::Name : public CEntityScriptBase { \
    public: \
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_EndGameFocalSitesScript&, CScriptDataBase*, const CScriptThing&); \
    };
FABLE_EGFS_ENTITY(CEndGameFocalSite)        // Alloc 0x00D2C140
FABLE_EGFS_ENTITY(CEndGameScarletRobe)      // Alloc 0x00D2CF80
FABLE_EGFS_ENTITY(CEndGameThunder)          // Alloc 0x00D2C730
FABLE_EGFS_ENTITY(CPermanentBarrier)        // Alloc 0x00D2D730
FABLE_EGFS_ENTITY(CBattleMinion)            // Alloc 0x00D2C340
FABLE_EGFS_ENTITY(CBattleGuard)             // Alloc 0x00D2C430
#undef FABLE_EGFS_ENTITY
}

void NScript::CQ_EndGameFocalSitesScript::Main()
{
    if (!Game->IsQuestActive("V_GuildMaster")) {
        if (IsActiveThreadTerminating())
            return;
        Game->ActivateQuest("V_GuildMaster");
    }
    while (!Game->IsRegionLoaded("HeroGuildComplexInside")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    CScriptThing guildComplex = Game->GetNearestWithDefName(Game->GetHero(), "VILLAGE_GUILD_COMPLEX_INSIDE");
    Game->EnableVillagerDefTypes(guildComplex, false, "CREATURE_GUILD_APPRENTICE_MALE");
    Game->EnableVillagerDefTypes(guildComplex, false, "CREATURE_GUILD_APPRENTICE_FEMALE");
    Game->EnableGuards(guildComplex, false);
    Game->SetThingAsUsable(Game->GetThingWithScriptName("GuildDoors"), false);
    Game->TransitionToTheme("ENVIRONMENT_EOW_04", 5.0f);
    while (!Game->IsRegionLoaded("Witchwood1")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->OverrideMusic(MUSIC_SET_BOSS, false, false);
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_EndGameFocalSitesScript>("EndGameFocalSite", this, &CEndGameFocalSite::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_EndGameFocalSitesScript>("EndGameJackOfBlades", this, &CEndGameJackOfBlades::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_EndGameFocalSitesScript>("EndGameScythe", this, &CEndGameScythe::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_EndGameFocalSitesScript>("EndGameScarletRobe", this, &CEndGameScarletRobe::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_EndGameFocalSitesScript>("EndGameThunder", this, &CEndGameThunder::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_EndGameFocalSitesScript>("PermanentBarrier", this, &CPermanentBarrier::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_EndGameFocalSitesScript>("BattleMinion", this, &CBattleMinion::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_EndGameFocalSitesScript>("BattleGuard", this, &CBattleGuard::Alloc, 0));
    PostAddScriptedEntities();
    Game->PostAddScriptedEntities();
    CScriptThing generator = Game->GetThingWithScriptName("BattleGenerator");
    Game->CreatureGeneratorSetAlwaysCreateCreaturesOnTrigger(generator, true);
    CurrentFocalSiteRegion = 1;
    MissionFailed = false;
    do {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        if (CurrentFocalSiteRegion == 2) {
            if (IsActiveThreadTerminating())
                return;
            while (!Game->IsRegionLoaded("OrchardFarm")) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
            Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_ACTIVATE_FOCAL_SITES_OBJECTIVE_02", "GreatwoodLake", "Greatwood");
            Game->FadeScreenIn();
        } else if (CurrentFocalSiteRegion == 3) {
            if (IsActiveThreadTerminating())
                return;
            while (!Game->IsRegionLoaded("HobbeCaveEntranceTunnel")) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
            Game->SetEnvironmentThemeWeightAllChannels("ENVIRONMENT_EOW_04", 0.2f);
            Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_ACTIVATE_FOCAL_SITES_OBJECTIVE_03", "HobbeCaveNymphFocal", "Greatwood");
            Game->FadeScreenIn();
        } else if (CurrentFocalSiteRegion == 4) {
            if (IsActiveThreadTerminating())
                return;
            while (!Game->IsRegionLoaded("BowerstoneJail")) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
            Game->SetEnvironmentThemeWeightAllChannels("ENVIRONMENT_EOW_04", 1.0f);
            Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_ACTIVATE_FOCAL_SITES_OBJECTIVE_04", "GibbetWoods", "BowerstoneSlums");
            Game->FadeScreenIn();
        }
        if (CurrentFocalSiteRegion != 1) {
            if (IsActiveThreadTerminating())
                return;
            Game->HeroReceiveMessageFromGuildMaster(RegionDialogueNames[CurrentFocalSiteRegion], "", true, true);
        }
        CScriptThing jack = Game->GetThingWithScriptName("EndGameJackOfBlades");
        EndCutScenePlayed = false;
        JackLeaves = false;
        counter = 0;
        JackReached = false;
        do {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
            Game->Pause(0.3f);
            if (EndCutScenePlayed)
                break;
            if (IsActiveThreadTerminating())
                return;
            std::vector<CScriptThing> minions;
            Game->GetAllThingsWithScriptName("BattleMinion", minions);
            long numMinions = minions.size();
            std::vector<CScriptThing> guards;
            Game->GetAllThingsWithScriptName("BattleGuard", guards);
            long numGuards = guards.size();
            if (numGuards < 3) {
                if (IsActiveThreadTerminating())
                    return;
                C3DVector offset(0.0f, 15.0f, 0.0f);
                float angle = (rand() % 60 - 30) * 0.01f;
                offset.RotateXY(angle + Game->GetGameAngleXY());
                offset += Game->GetHero().GetPos();
                Game->EntityTeleportToPosition(generator, offset, 0.0f, false, false);
                if (rand() % 2 == 0) {
                    if (IsActiveThreadTerminating())
                        return;
                    Game->CreatureGeneratorSetFamily(generator, "END_BATTLE_APPRENTICE_FAMILY");
                } else {
                    if (IsActiveThreadTerminating())
                        return;
                    Game->CreatureGeneratorSetFamily(generator, "END_BATTLE_GUARD_FAMILY");
                }
                Game->CreatureGeneratorSetGeneratedCreatureScriptName(generator, "BattleGuard");
                Game->CreatureGeneratorTrigger(generator);
            }
            if (numMinions < 3) {
                if (IsActiveThreadTerminating())
                    return;
                C3DVector offset(0.0f, 15.0f, 0.0f);
                float angle = (rand() % 60 - 30) * 0.01f;
                offset.RotateXY(angle + Game->GetGameAngleXY());
                offset += Game->GetHero().GetPos();
                Game->EntityTeleportToPosition(generator, offset, 0.0f, false, false);
                switch (CurrentFocalSiteRegion) {
                case 1: {
                    long family = rand() % 4;
                    if (family == 0) {
                        if (IsActiveThreadTerminating())
                            return;
                        Game->CreatureGeneratorSetFamily(generator, "END_BATTLE_UNDEAD_LIEUTENANT_SWORD_FAMILY");
                    } else if (family == 1) {
                        if (IsActiveThreadTerminating())
                            return;
                        Game->CreatureGeneratorSetFamily(generator, "END_BATTLE_MINION_FAMILY");
                    } else if (family == 2) {
                        if (IsActiveThreadTerminating())
                            return;
                        Game->CreatureGeneratorSetFamily(generator, "END_BATTLE_UNDEAD_SWORD_FAMILY");
                    } else {
                        if (IsActiveThreadTerminating())
                            return;
                        Game->CreatureGeneratorSetFamily(generator, "END_BATTLE_UNDEAD_AXE_FAMILY");
                    }
                    Game->CreatureGeneratorSetGeneratedCreatureScriptName(generator, "BattleMinion");
                    Game->CreatureGeneratorTrigger(generator);
                    break;
                }
                case 2: {
                    long family = rand() % 4;
                    if (family == 0) {
                        if (IsActiveThreadTerminating())
                            return;
                        Game->CreatureGeneratorSetFamily(generator, "END_BATTLE_SCREAMER_FAMILY");
                    } else if (family == 1) {
                        if (IsActiveThreadTerminating())
                            return;
                        Game->CreatureGeneratorSetFamily(generator, "END_BATTLE_MINION_FAMILY");
                    } else if (family == 2) {
                        if (IsActiveThreadTerminating())
                            return;
                        Game->CreatureGeneratorSetFamily(generator, "END_BATTLE_UNDEAD_LIEUTENANT_AXE_FAMILY");
                    } else {
                        if (IsActiveThreadTerminating())
                            return;
                        Game->CreatureGeneratorSetFamily(generator, "END_BATTLE_UNDEAD_AXE_FAMILY");
                    }
                    Game->CreatureGeneratorSetGeneratedCreatureScriptName(generator, "BattleMinion");
                    Game->CreatureGeneratorTrigger(generator);
                    break;
                }
                case 3: {
                    long family = rand() % 3;
                    if (family == 0) {
                        if (IsActiveThreadTerminating())
                            return;
                        Game->CreatureGeneratorSetFamily(generator, "END_BATTLE_SCREAMER_FAMILY");
                    } else if (family == 1) {
                        if (IsActiveThreadTerminating())
                            return;
                        Game->CreatureGeneratorSetFamily(generator, "END_BATTLE_MINION_FAMILY");
                    } else if (family == 2) {
                        if (IsActiveThreadTerminating())
                            return;
                        Game->CreatureGeneratorSetFamily(generator, "END_BATTLE_UNDEAD_LIEUTENANT_AXE_FAMILY");
                    }
                    Game->CreatureGeneratorSetGeneratedCreatureScriptName(generator, "BattleMinion");
                    Game->CreatureGeneratorTrigger(generator);
                    break;
                }
                case 4:
                    Game->CreatureGeneratorSetFamily(generator, "END_BATTLE_MINION_FAMILY");
                    Game->CreatureGeneratorSetGeneratedCreatureScriptName(generator, "BattleMinion");
                    Game->CreatureGeneratorTrigger(generator);
                    break;
                }
            }
            if (++counter >= 3) {
                if (IsActiveThreadTerminating())
                    return;
                counter = 0;
            }
        } while (!EndCutScenePlayed);
        if (IsActiveThreadTerminating())
            return;
        if (Game->GetThingWithScriptName("EndGameJackOfBlades").IsAlive()) {
            if (IsActiveThreadTerminating())
                return;
            Game->RemoveThing(Game->GetThingWithScriptName("EndGameJackOfBlades"), false, true);
        }
        ++CurrentFocalSiteRegion;
    } while (CurrentFocalSiteRegion <= 4);
    if (IsActiveThreadTerminating())
        return;
    while (!Game->IsRegionLoaded("HeroGuildComplexInside")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->SetCreatureGeneratorsEnabled("", true);
    Game->HeroReceiveMessageFromGuildMaster("TEXT_QST_001_GUILDMASTER_FINISHED_SITES", "", true, true);
    if (MissionFailed) {
        if (IsActiveThreadTerminating())
            return;
        Game->SetQuestAsFailed(Game->GetActiveQuestName(), true, L"", true);
    } else {
        if (IsActiveThreadTerminating())
            return;
        Game->SetQuestAsCompleted(Game->GetActiveQuestName(), true, false, false);
    }
    Game->DeactivateQuestLater(Game->GetActiveQuestName(), 0);
    Game->ResetToDefaultTheme(5.0f);
    Game->StopOverrideMusic(false);
    Game->SetGuardsIgnoreCrimes(false);
}