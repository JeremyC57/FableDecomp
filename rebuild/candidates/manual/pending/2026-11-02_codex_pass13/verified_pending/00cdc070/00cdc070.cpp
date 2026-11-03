// NScript::CQ_SunnyvaleMasterData::OnPersist(CPersistContext&) -- transfers persistent quest data and the original temporary array vectors.
// Leaf: ?OnPersist@CQ_SunnyvaleMasterData@NScript@@UAEXAAVCPersistContext@@@Z
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CPERSISTCONTEXT_DEFAULT_VALUES
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CPersistContext.h"
extern long NPCAttitudes[3];
template<class T> inline void CPersistContext::Transfer(const char* name,T& value) { Transfer(name,value,GetDefaultVal<T>()); }
template<> void std::vector<long>::push_back(const long&);
template<> void std::vector<bool>::push_back(const bool&);
void NScript::CQ_SunnyvaleMasterData::OnPersist(CPersistContext& context)
{
    context.Transfer("PostSavePosition",PostSavePosition);
    context.Transfer("HauntedBarrowFieldsCompleted",HauntedBarrowFieldsCompleted);
    context.Transfer("GrannyMemoryReturned",GrannyMemoryReturned);
    context.Transfer("IsLunaHuman",IsLunaHuman);
    context.Transfer("FriendOfForeman",FriendOfForeman);
    context.Transfer("BridgeOpened",BridgeOpened);
    context.Transfer("ArcheryHighScore",ArcheryHighScore);
    context.Transfer("ArcheryStateCurrent",ArcheryStateCurrent);
    context.Transfer("ArcheryStateRequired",ArcheryStateRequired);
    context.Transfer("CondemnedManDead",CondemnedManDead);
    context.Transfer("CondemnedManForgiven",CondemnedManForgiven);
    context.Transfer("CondemnedManMeetsBodyGuard",CondemnedManMeetsBodyGuard);
    context.Transfer("CondemnedManMeetsBodyGuardCutSceneStart",CondemnedManMeetsBodyGuardCutSceneStart);
    context.Transfer("CondemnedManMeetsBodyGuardCutSceneFinished",CondemnedManMeetsBodyGuardCutSceneFinished);
    context.Transfer("TeddySolution",TeddySolution);
    context.Transfer("OrchardFarmRaidLastCompleted",OrchardFarmRaidLastCompleted);
    context.Transfer("OrchardFarmTraderEscortCounter",OrchardFarmTraderEscortCounter);
    context.Transfer("SeenAbbeyMotherAtGuild",SeenAbbeyMotherAtGuild);
    context.Transfer("DefeatedThunder",DefeatedThunder);
    context.Transfer("LostToThunder",LostToThunder);
    context.Transfer("KilledThunder",KilledThunder);
    context.Transfer("CollectedSoulFromArena",CollectedSoulFromArena);
    context.Transfer("KilledBriar",KilledBriar);
    context.Transfer("CollectedSoulFromMother",CollectedSoulFromMother);
    context.Transfer("KilledGM",KilledGM);
    context.Transfer("CollectedSoulFromNostro",CollectedSoulFromNostro);
    context.Transfer("DeliveredSoul",DeliveredSoul);
    context.Transfer("CurrentHeroSoulsPosition",CurrentHeroSoulsPosition);
    context.Transfer("WhisperKilledByHero",WhisperKilledByHero);
    context.Transfer("ArenaFinished",ArenaFinished);
    context.Transfer("HangingTreeBanditKilled",HangingTreeBanditKilled);
    context.Transfer("HangingTreeGuardKilled",HangingTreeGuardKilled);
    context.Transfer("GatesRequireClosing",GatesRequireClosing);
    context.Transfer("GatesRequireOpening",GatesRequireOpening);
    context.Transfer("AchievementsWorthyOfSong",AchievementsWorthyOfSong);
    context.Transfer("StoryTellerSpecialStories",StoryTellerSpecialStories);
    context.Transfer("StoryTellerToldSpecialStories",StoryTellerToldSpecialStories);
    context.Transfer("HeroDrunkness",HeroDrunkness,0L);
    context.Transfer("VillagerAngryRating",VillagerAngryRating);
    context.Transfer("TrophyDealerHeroSpokenToDemonDoors",TrophyDealerHeroSpokenToDemonDoors,false);
    context.Transfer("StruckDealWithLadyGrey",StruckDealWithLadyGrey,false);
    context.Transfer("HeroExposedLadyGrey",HeroExposedLadyGrey,false);
    context.Transfer("HeroMarriedLadyGrey",HeroMarriedLadyGrey,false);
    context.Transfer("TimeAdvancePointTriggered",TimeAdvancePointTriggered,false);
    context.Transfer("ScorpionsDestroyed",ScorpionsDestroyed,false);
    context.Transfer("GuildWarningOccuring",GuildWarningOccuring,false);
    context.Transfer("SkillTestOccuring",SkillTestOccuring,false);
    context.Transfer("WillTestOccuring",WillTestOccuring,false);
    context.Transfer("ScorpionsDestroyedCutscenePlayed",ScorpionsDestroyedCutscenePlayed,false);
    context.Transfer("SkillTrainingStarted",SkillTrainingStarted,false);
    context.Transfer("WillTrainingStarted",WillTrainingStarted,false);
    context.Transfer("MovingDummiesNeeded",MovingDummiesNeeded,false);
    context.Transfer("MeleeApprenticeNeededForCutscene",MeleeApprenticeNeededForCutscene,false);
    context.Transfer("GlobalMeleeGrade",GlobalMeleeGrade,0L);
    context.Transfer("GlobalSkillGrade",GlobalSkillGrade,0L);
    context.Transfer("GlobalWillGrade",GlobalWillGrade,0L);
    context.Transfer("SkillRepeating",SkillRepeating,false);
    context.Transfer("SkillRepeatKnown",SkillRepeatKnown,false);
    context.Transfer("SkillDummyReset",SkillDummyReset,false);
    context.Transfer("HighestSkillScore",HighestSkillScore,0L);
    context.Transfer("SingingStonesInSync",SingingStonesInSync,false);
    context.Transfer("SwordInTheStoneComplete",SwordInTheStoneComplete,false);
    context.Transfer("AmbushTradersAllGuardsDead",AmbushTradersAllGuardsDead,false);
    context.Transfer("AmbushTradersAllTradersDead",AmbushTradersAllTradersDead,false);
    context.Transfer("AmbushTradersKillCount",AmbushTradersKillCount,0L);
    context.Transfer("AmbushTradersBanditHireCount",AmbushTradersBanditHireCount,0L);
    context.Transfer("AmbushTradersSpyDead",AmbushTradersSpyDead,false);
    context.Transfer("BountyHuntWithinTimeLimit",BountyHuntWithinTimeLimit,false);
    context.Transfer("BountyHuntTimeLimitExceeded",BountyHuntTimeLimitExceeded,false);
    context.Transfer("BountyHuntDecapitation",BountyHuntDecapitation,false);
    std::vector<long> NPCAttitudesTransferVector;
    for(long i=0;i<3;++i) NPCAttitudesTransferVector.push_back(NPCAttitudes[i]);
    context.TransferVector("NPCAttitudesTransferVector",NPCAttitudesTransferVector);
    context.Transfer("BreakSiegeFinished",BreakSiegeFinished);
    context.Transfer("WhiteBalverineFinished",WhiteBalverineFinished);
    context.Transfer("MadBomberFinished",MadBomberFinished,false);
    context.Transfer("BanditCampTwinbladeKilled",BanditCampTwinbladeKilled,false);
    context.Transfer("PrisonRaceNumber",PrisonRaceNumber,0L);
    context.Transfer("PrisonKeyStolenByHero",PrisonKeyStolenByHero,false);
    std::vector<bool> BooksPreviouslyOpened_Vector;
    for(long i=0;i<3;++i) BooksPreviouslyOpened_Vector.push_back(BooksPreviouslyOpened[i]);
    context.TransferVector("BooksPreviouslyOpened_Vector",BooksPreviouslyOpened_Vector);
    context.Transfer("MaxChickenKickingScore",MaxChickenKickingScore);
    context.Transfer("JackBossBattleHeroGoodAtEnd",JackBossBattleHeroGoodAtEnd);
    context.Transfer("JackBossBattleResult",JackBossBattleResult);
}
