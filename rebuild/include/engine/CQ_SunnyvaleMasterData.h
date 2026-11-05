#ifndef FABLE_ENGINE_CQ_SUNNYVALEMASTERDATA_H
#define FABLE_ENGINE_CQ_SUNNYVALEMASTERDATA_H
// Original Ego_r TPI 0x11e3a, native constructor 00cdb9e0 / Init 00cdba10 / Alloc 00cdbd20.
// Layout: allocator requests 0x144; vector is 12 bytes at +0x134, final flag +0x140.
// Every original field is retained. Native Main 00d51520 accesses GuildWarningOccuring +0xc0.
// Codex pass 13 typed the pass 12 opaque members (EGameflowPosition, EPlaceInHeroSouls, EHeroNames and ArcheryOpenState from the
// Ego_r / FableWin LF_ENUM records); layout unchanged (BodyGuard reads HenchmanCommentaryState +8, HenchmanOverrideState +0xC).
#include <stddef.h>
#include "engine/CScriptDataBase.h"
#include "fable_string.h"
#include "stlport/fable_containers.h"
enum EGameflowPosition {
    EGP_INTRO = 0,
    EGP_TRAINING = 100,
    EGP_WAITING_FOR_WASP_BOSS = 150,
    EGP_WASP_BOSS = 200,
    EGP_MAZE_MEETING = 300,
    EGP_WAITING_FOR_ORCHARD_FARM = 400,
    EGP_ORCHARD_FARM = 450,
    EGP_WAITING_FOR_TRADER_ESCORT = 500,
    EGP_TRADER_ESCORT = 550,
    EGP_SECOND_MAZE_MEETING = 600,
    EGP_BANDIT_CAMP = 700,
    EGP_WAITING_FOR_BANDIT_CAMP_TWINBLADE = 733,
    EGP_BANDIT_CAMP_TWINBLADE = 766,
    EGP_MAZE_TELEPORT_TO_WW = 800,
    EGP_TROPHY_DEALER = 850,
    EGP_WITCHWOOD_POST_TROPHY_DEALER = 856,
    EGP_WHITE_BALVERINE_KHG = 865,
    EGP_WHITE_BALVERINE_WITCHWOOD = 870,
    EGP_WITCHWOOD_WAITING_FOR_ARENA = 875,
    EGP_ARENA = 900,
    EGP_MEET_SISTER = 1000,
    EGP_WAITING_FOR_MCC = 1050,
    EGP_MINION_CLIFFTOP_CHASE = 1100,
    EGP_GRAVEYARD = 1200,
    EGP_WAITING_FOR_FINALGRAVEYARD = 1216,
    EGP_FINALGRAVEYARD = 1233,
    EGP_PRISON = 1250,
    EGP_HOOK_COAST = 1300,
    EGP_WAITING_FOR_WIZARD_BATTLE = 1450,
    EGP_WIZARD_BATTLE = 1500,
    EGP_WAITING_FOR_FOCAL_SITES = 1550,
    EGP_FOCAL_SITES = 1600,
    EGP_JACK_BOSS_FIGHT = 1700,
    EGP_FIRE_HEART = 1900,
    EGP_THE_SHIP = 2100,
    EGP_THE_ORACLE = 2300,
    EGP_SCYTHE_MEETING = 2400,
    EGP_HERO_SOULS = 2500,
    EGP_DRAGON_FIGHT = 2600,
    EGP_FINISHED_GAME = 2800
};
enum EPlaceInHeroSouls {
    EPIHS_1ST_SOUL_TALK_TO_BRIAR = 0,
    EPIHS_1ST_SOUL_TALK_TO_THUNDER = 1,
    EPIHS_1ST_SOUL_VISIT_THE_ARENA = 2,
    EPIHS_2ND_SOUL_TALK_TO_BRIAR_KILLED_THUNDER = 3,
    EPIHS_2ND_SOUL_TALK_TO_BRIAR_GOT_ARENA_SOUL = 4,
    EPIHS_2ND_SOUL_VISIT_YOUR_MOTHER = 5,
    EPIHS_3RD_SOUL_TALK_TO_BRIAR = 6,
    EPIHS_3RD_SOUL_TALK_TO_SCYTHE = 7
};
enum EHeroNames {
    NAME_NAMELESS = 0,
    NAME_FEASTMASTER = 1,
    NAME_BASILISK = 2,
    NAME_DARKBLADE = 3,
    NAME_WOLFSEYE = 4
};
namespace NScript {
class CQ_SunnyvaleMasterData : public CScriptDataBase {
public:
    enum ArcheryOpenState {
        ARCHERY_STATE_OPEN = 0,
        ARCHERY_STATE_CLOSED = 1,
        ARCHERY_STATE_LATER = 2
    };
    EGameflowPosition PostSavePosition; // +0x4
    long HenchmanCommentaryState; // +0x8
    long HenchmanOverrideState; // +0xc
    bool HenchmanSacrificedBodge; // +0x10
    bool HauntedBarrowFieldsCompleted; // +0x11
    long RandomEntityVillagerMax; // +0x14
    long RandomEntityGuardMax; // +0x18
    long RandomEntityBanditMax; // +0x1c
    long RandomEntityVillagerCount; // +0x20
    long RandomEntityGuardCount; // +0x24
    long RandomEntityBanditCount; // +0x28
    float RandomEntityVillagerPercentage; // +0x2c
    float RandomEntityGuardPercentage; // +0x30
    float RandomEntityBanditPercentage; // +0x34
    bool RandomEntitySheriffAlive; // +0x38
    bool RandomEntityLieutenantAlive; // +0x39
    bool BanditCampTwinbladeKilled; // +0x3a
    ArcheryOpenState ArcheryStateCurrent; // +0x3c
    ArcheryOpenState ArcheryStateRequired; // +0x40
    long ArcheryHighScore; // +0x44
    bool FriendOfForeman; // +0x48
    bool BridgeOpened; // +0x49
    bool GrannyMemoryReturned; // +0x4a
    bool IsLunaHuman; // +0x4b
    bool CondemnedManDead; // +0x4c
    bool CondemnedManForgiven; // +0x4d
    bool CondemnedManMeetsBodyGuard; // +0x4e
    bool CondemnedManMeetsBodyGuardCutSceneStart; // +0x4f
    bool CondemnedManMeetsBodyGuardCutSceneFinished; // +0x50
    CCharString TeddySolution; // +0x54
    long OrchardFarmRaidLastCompleted; // +0x58
    long OrchardFarmTraderEscortCounter; // +0x5c
    bool SeenAbbeyMotherAtGuild; // +0x60
    bool DefeatedThunder; // +0x61
    bool LostToThunder; // +0x62
    bool KilledThunder; // +0x63
    bool CollectedSoulFromArena; // +0x64
    bool KilledBriar; // +0x65
    bool CollectedSoulFromMother; // +0x66
    bool KilledGM; // +0x67
    bool CollectedSoulFromNostro; // +0x68
    long DeliveredSoul; // +0x6c
    EPlaceInHeroSouls CurrentHeroSoulsPosition; // +0x70
    bool WhisperKilledByHero; // +0x74
    bool ArenaFinished; // +0x75
    bool GatesRequireClosing; // +0x76
    bool GatesRequireOpening; // +0x77
    long HeroDrunkness; // +0x78
    bool TrophyDealerHeroSpokenToDemonDoors; // +0x7c
    float VillagerAngryRating; // +0x80
    bool MotherDodgingKraken; // +0x84
    long JackBossBattleResult; // +0x88
    bool JackBossBattleHeroGoodAtEnd; // +0x8c
    bool WifeLeavingYou; // +0x8d
    bool WifeLeftYou; // +0x8e
    long CurrentScene; // +0x90
    bool StartNextScene; // +0x94
    long CurrentChapter; // +0x98
    bool ScorpionsDestroyed; // +0x9c
    bool ScorpionsDestroyedCutscenePlayed; // +0x9d
    bool SkillTrainingStarted; // +0x9e
    bool WillTrainingStarted; // +0x9f
    bool MovingDummiesNeeded; // +0xa0
    long SkillScore; // +0xa4
    long HighestSkillScore; // +0xa8
    long WillScore; // +0xac
    bool MeleeApprenticeNeededForCutscene; // +0xb0
    long GlobalMeleeGrade; // +0xb4
    long GlobalSkillGrade; // +0xb8
    long GlobalWillGrade; // +0xbc
    bool GuildWarningOccuring; // +0xc0
    bool SkillTestOccuring; // +0xc1
    bool WillTestOccuring; // +0xc2
    bool SkillRepeating; // +0xc3
    bool SkillRepeatKnown; // +0xc4
    bool SkillDummyReset; // +0xc5
    bool HeroTakingGuildTest; // +0xc6
    bool SwordInTheStoneComplete; // +0xc7
    bool AmbushTradersAllGuardsDead; // +0xc8
    bool AmbushTradersAllTradersDead; // +0xc9
    long AmbushTradersKillCount; // +0xcc
    long AmbushTradersBanditHireCount; // +0xd0
    bool AmbushTradersSpyDead; // +0xd4
    bool StruckDealWithLadyGrey; // +0xd5
    bool HeroExposedLadyGrey; // +0xd6
    bool HeroMarriedLadyGrey; // +0xd7
    bool BountyHuntWithinTimeLimit; // +0xd8
    bool BountyHuntDecapitation; // +0xd9
    bool BountyHuntTimeLimitExceeded; // +0xda
    unsigned long AchievementsWorthyOfSong; // +0xdc
    bool BreakSiegeFinished; // +0xe0
    bool WhiteBalverineFinished; // +0xe1
    bool MadBomberFinished; // +0xe2
    bool BodyGuardsMustStandAndWait; // +0xe3
    bool BodyGuardsInLimbo; // +0xe4
    bool WifeHeroFlirting; // +0xe5
    long PrisonRaceNumber; // +0xe8
    bool PrisonRaceWonByHero; // +0xec
    bool WasPrisonRaceEverWonByHero; // +0xed
    bool PrisonKeyStolenByHero; // +0xee
    bool BooksPreviouslyOpened[3]; // +0xef
    bool PrisonTorturedBefore; // +0xf2
    bool PrisonGameDoneBefore; // +0xf3
    bool HeroDollsScriptUsingTeacher; // +0xf4
    long MaxChickenKickingScore; // +0xf8
    unsigned long StoryTellerSpecialStories; // +0xfc
    unsigned long StoryTellerToldSpecialStories; // +0x100
    EHeroNames GHeroName; // +0x104
    bool TimeAdvancePointTriggered; // +0x108
    bool OFBRCratesStolen; // +0x109
    bool OFBR_NoCratesWereStolen; // +0x10a
    bool DarkwoodAllTradersAlive; // +0x10b
    bool MadBomberNoBombsExplode; // +0x10c
    bool HobbeCavePerpertratorKilled; // +0x10d
    long HobbeCaveInnocentSacrificed; // +0x110
    bool HobbeCaveBoyUnharmed; // +0x114
    bool BanditCampKillNoBandits; // +0x115
    bool BreakSiegeNoAssistance; // +0x116
    bool BreakSiegeKilledLeader; // +0x117
    bool OrchardFarmBanditKilled; // +0x118
    bool OrchardFarmGuardKilled; // +0x119
    bool BanditCampKillManyBandits; // +0x11a
    bool HobbeContestKillMoreHobbes; // +0x11b
    bool GraveyardKillAllInnocents; // +0x11c
    long DarkwoodPickpocketedAllTraders; // +0x120
    bool HangingTreeBanditKilled; // +0x124
    bool HangingTreeGuardKilled; // +0x125
    bool TCGKillNoBandits; // +0x126
    bool TCGMadeTimeLimit; // +0x127
    bool TCEKeepBanditFollowerAlive; // +0x128
    bool TCEMadeTimeLimit; // +0x129
    bool RansomVictimVictimNoDamage; // +0x12a
    bool RansomVictimKidnappersKilled; // +0x12b
    bool RansomVictimSaveVictim; // +0x12c
    bool RansomVictimHaveVictimKilled; // +0x12d
    bool MinionCampBriarNoDamage; // +0x12e
    bool FireHeartFreeAllPrisoners; // +0x12f
    bool FireHeartKillAllPrisoners; // +0x130
    bool TCETimeLimitBoastTaken; // +0x131
    bool TCGTimeLimitBoastTaken; // +0x132
    bool HeroWoreMask; // +0x133
    std::vector<CCharString,std::allocator<CCharString> > BoastRegions; // +0x134
    bool SingingStonesInSync; // +0x140
    void ModifyVillagerAngryRating(float);
    virtual void OnPersist(CPersistContext&);
    virtual void Init();
#ifdef FABLE_RETAIL_CQ_SUNNYVALEMASTERDATA_RELATION_STATICS
    // Retail opt-in (chunk 780): the three unnamed statics the landed Init 0x00CDBA10 resets and the transfer 0x00CDC890 walks
    // (reloc_bindings ?RelationCounters@CQ_SunnyvaleMasterData@NScript@@2PAJA 0x0143E928; the strings 0x0143E934). Retail
    // CV_FishermanScript::NPCAttitudeProcess 0x00E7C1D6 stores 2 into RelationCounters[2] (0x0143E930).
    static long        RelationCounters[3];                                   // 0x0143E928
    static CCharString RelationSuffixes[3];                                   // 0x0143E934
#endif
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_SunnyvaleMasterData) == 0x144);
FABLE_STATIC_ASSERT(offsetof(NScript::CQ_SunnyvaleMasterData, GuildWarningOccuring) == 0xc0);
FABLE_STATIC_ASSERT(offsetof(NScript::CQ_SunnyvaleMasterData, SingingStonesInSync) == 0x140);
#endif
