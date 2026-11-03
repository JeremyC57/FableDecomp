#ifndef FABLE_ENGINE_CQ_SUNNYVALEMASTERDATA_H
#define FABLE_ENGINE_CQ_SUNNYVALEMASTERDATA_H
// Retail opt-in (Codex pass 12): original Ego_r/FableWin members, donor size328.
// Retail uses12-byte STLport vectors: BoastRegions +0x134..0x13f,
// SingingStonesInSync +0x140; complete object size0x144. BodyGuard independently
// reads HenchmanCommentaryState at +8 and HenchmanOverrideState at +12.
#include "engine/CScriptDataBase.h"
#include "engine/CCharString.h"
#ifdef FABLE_RETAIL_QUEST_SCRIPT_PERSIST
#include "stlport/fable_containers.h"
#else
#include "stlport46/vector_roots.h"
#endif
namespace NScript {
class CQ_SunnyvaleMasterData : public CScriptDataBase {
public:
    unsigned char PostSavePosition[4]; // +0x4 EGameflowPosition
    long HenchmanCommentaryState; // +0x8 long
    long HenchmanOverrideState; // +0xc long
    bool HenchmanSacrificedBodge; // +0x10 bool
    bool HauntedBarrowFieldsCompleted; // +0x11 bool
    long RandomEntityVillagerMax; // +0x14 long
    long RandomEntityGuardMax; // +0x18 long
    long RandomEntityBanditMax; // +0x1c long
    long RandomEntityVillagerCount; // +0x20 long
    long RandomEntityGuardCount; // +0x24 long
    long RandomEntityBanditCount; // +0x28 long
    float RandomEntityVillagerPercentage; // +0x2c float
    float RandomEntityGuardPercentage; // +0x30 float
    float RandomEntityBanditPercentage; // +0x34 float
    bool RandomEntitySheriffAlive; // +0x38 bool
    bool RandomEntityLieutenantAlive; // +0x39 bool
    bool BanditCampTwinbladeKilled; // +0x3a bool
    unsigned char ArcheryStateCurrent[4]; // +0x3c NScript::CQ_SunnyvaleMasterData::ArcheryOpenState
    unsigned char ArcheryStateRequired[4]; // +0x40 NScript::CQ_SunnyvaleMasterData::ArcheryOpenState
    long ArcheryHighScore; // +0x44 long
    bool FriendOfForeman; // +0x48 bool
    bool BridgeOpened; // +0x49 bool
    bool GrannyMemoryReturned; // +0x4a bool
    bool IsLunaHuman; // +0x4b bool
    bool CondemnedManDead; // +0x4c bool
    bool CondemnedManForgiven; // +0x4d bool
    bool CondemnedManMeetsBodyGuard; // +0x4e bool
    bool CondemnedManMeetsBodyGuardCutSceneStart; // +0x4f bool
    bool CondemnedManMeetsBodyGuardCutSceneFinished; // +0x50 bool
    CCharString TeddySolution; // +0x54 CCharString
    long OrchardFarmRaidLastCompleted; // +0x58 long
    long OrchardFarmTraderEscortCounter; // +0x5c long
    bool SeenAbbeyMotherAtGuild; // +0x60 bool
    bool DefeatedThunder; // +0x61 bool
    bool LostToThunder; // +0x62 bool
    bool KilledThunder; // +0x63 bool
    bool CollectedSoulFromArena; // +0x64 bool
    bool KilledBriar; // +0x65 bool
    bool CollectedSoulFromMother; // +0x66 bool
    bool KilledGM; // +0x67 bool
    bool CollectedSoulFromNostro; // +0x68 bool
    long DeliveredSoul; // +0x6c long
    unsigned char CurrentHeroSoulsPosition[4]; // +0x70 EPlaceInHeroSouls
    bool WhisperKilledByHero; // +0x74 bool
    bool ArenaFinished; // +0x75 bool
    bool GatesRequireClosing; // +0x76 bool
    bool GatesRequireOpening; // +0x77 bool
    long HeroDrunkness; // +0x78 long
    bool TrophyDealerHeroSpokenToDemonDoors; // +0x7c bool
    float VillagerAngryRating; // +0x80 float
    bool MotherDodgingKraken; // +0x84 bool
    long JackBossBattleResult; // +0x88 long
    bool JackBossBattleHeroGoodAtEnd; // +0x8c bool
    bool WifeLeavingYou; // +0x8d bool
    bool WifeLeftYou; // +0x8e bool
    long CurrentScene; // +0x90 long
    bool StartNextScene; // +0x94 bool
    long CurrentChapter; // +0x98 long
    bool ScorpionsDestroyed; // +0x9c bool
    bool ScorpionsDestroyedCutscenePlayed; // +0x9d bool
    bool SkillTrainingStarted; // +0x9e bool
    bool WillTrainingStarted; // +0x9f bool
    bool MovingDummiesNeeded; // +0xa0 bool
    long SkillScore; // +0xa4 long
    long HighestSkillScore; // +0xa8 long
    long WillScore; // +0xac long
    bool MeleeApprenticeNeededForCutscene; // +0xb0 bool
    long GlobalMeleeGrade; // +0xb4 long
    long GlobalSkillGrade; // +0xb8 long
    long GlobalWillGrade; // +0xbc long
    bool GuildWarningOccuring; // +0xc0 bool
    bool SkillTestOccuring; // +0xc1 bool
    bool WillTestOccuring; // +0xc2 bool
    bool SkillRepeating; // +0xc3 bool
    bool SkillRepeatKnown; // +0xc4 bool
    bool SkillDummyReset; // +0xc5 bool
    bool HeroTakingGuildTest; // +0xc6 bool
    bool SwordInTheStoneComplete; // +0xc7 bool
    bool AmbushTradersAllGuardsDead; // +0xc8 bool
    bool AmbushTradersAllTradersDead; // +0xc9 bool
    long AmbushTradersKillCount; // +0xcc long
    long AmbushTradersBanditHireCount; // +0xd0 long
    bool AmbushTradersSpyDead; // +0xd4 bool
    bool StruckDealWithLadyGrey; // +0xd5 bool
    bool HeroExposedLadyGrey; // +0xd6 bool
    bool HeroMarriedLadyGrey; // +0xd7 bool
    bool BountyHuntWithinTimeLimit; // +0xd8 bool
    bool BountyHuntDecapitation; // +0xd9 bool
    bool BountyHuntTimeLimitExceeded; // +0xda bool
    unsigned long AchievementsWorthyOfSong; // +0xdc unsigned long
    bool BreakSiegeFinished; // +0xe0 bool
    bool WhiteBalverineFinished; // +0xe1 bool
    bool MadBomberFinished; // +0xe2 bool
    bool BodyGuardsMustStandAndWait; // +0xe3 bool
    bool BodyGuardsInLimbo; // +0xe4 bool
    bool WifeHeroFlirting; // +0xe5 bool
    long PrisonRaceNumber; // +0xe8 long
    bool PrisonRaceWonByHero; // +0xec bool
    bool WasPrisonRaceEverWonByHero; // +0xed bool
    bool PrisonKeyStolenByHero; // +0xee bool
    bool BooksPreviouslyOpened[3]; // +0xef {'array_of': 'bool', 'size_bytes': 3}
    bool PrisonTorturedBefore; // +0xf2 bool
    bool PrisonGameDoneBefore; // +0xf3 bool
    bool HeroDollsScriptUsingTeacher; // +0xf4 bool
    long MaxChickenKickingScore; // +0xf8 long
    unsigned long StoryTellerSpecialStories; // +0xfc unsigned long
    unsigned long StoryTellerToldSpecialStories; // +0x100 unsigned long
    unsigned char GHeroName[4]; // +0x104 EHeroNames
    bool TimeAdvancePointTriggered; // +0x108 bool
    bool OFBRCratesStolen; // +0x109 bool
    bool OFBR_NoCratesWereStolen; // +0x10a bool
    bool DarkwoodAllTradersAlive; // +0x10b bool
    bool MadBomberNoBombsExplode; // +0x10c bool
    bool HobbeCavePerpertratorKilled; // +0x10d bool
    long HobbeCaveInnocentSacrificed; // +0x110 long
    bool HobbeCaveBoyUnharmed; // +0x114 bool
    bool BanditCampKillNoBandits; // +0x115 bool
    bool BreakSiegeNoAssistance; // +0x116 bool
    bool BreakSiegeKilledLeader; // +0x117 bool
    bool OrchardFarmBanditKilled; // +0x118 bool
    bool OrchardFarmGuardKilled; // +0x119 bool
    bool BanditCampKillManyBandits; // +0x11a bool
    bool HobbeContestKillMoreHobbes; // +0x11b bool
    bool GraveyardKillAllInnocents; // +0x11c bool
    long DarkwoodPickpocketedAllTraders; // +0x120 long
    bool HangingTreeBanditKilled; // +0x124 bool
    bool HangingTreeGuardKilled; // +0x125 bool
    bool TCGKillNoBandits; // +0x126 bool
    bool TCGMadeTimeLimit; // +0x127 bool
    bool TCEKeepBanditFollowerAlive; // +0x128 bool
    bool TCEMadeTimeLimit; // +0x129 bool
    bool RansomVictimVictimNoDamage; // +0x12a bool
    bool RansomVictimKidnappersKilled; // +0x12b bool
    bool RansomVictimSaveVictim; // +0x12c bool
    bool RansomVictimHaveVictimKilled; // +0x12d bool
    bool MinionCampBriarNoDamage; // +0x12e bool
    bool FireHeartFreeAllPrisoners; // +0x12f bool
    bool FireHeartKillAllPrisoners; // +0x130 bool
    bool TCETimeLimitBoastTaken; // +0x131 bool
    bool TCGTimeLimitBoastTaken; // +0x132 bool
    bool HeroWoreMask; // +0x133 bool
    std::vector<CCharString> BoastRegions; // +0x134 std::vector<CCharString,std::allocator<CCharString> >
    bool SingingStonesInSync; // +0x140 bool
    virtual void OnPersist(CPersistContext&);
    virtual void Init();
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_SunnyvaleMasterData)==0x144);
#endif
