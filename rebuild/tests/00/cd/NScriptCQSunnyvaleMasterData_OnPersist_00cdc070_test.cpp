#include <cassert>
#include <cstdio>
#include <string>
#include <vector>
#include <map>
#include <sstream>
static int strings=0;
class CCharString{public:std::string value;CCharString():value(){++strings;}CCharString(const char*s):value(s){++strings;}CCharString(const CCharString&x):value(x.value){++strings;}~CCharString(){--strings;}CCharString&operator=(const CCharString&x){value=x.value;return *this;}bool operator==(const CCharString&x)const{return value==x.value;}};
class CPersistContext;class CScriptDataBase{public:virtual ~CScriptDataBase(){}};
// Original Ego_r TPI 0x11e3a, native constructor 00cdb9e0 / Init 00cdba10 / Alloc 00cdbd20.
// Layout: allocator requests 0x144; vector is 12 bytes at +0x134, final flag +0x140.
// Every original field is retained. Native Main 00d51520 accesses GuildWarningOccuring +0xc0.
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
};
}
void NScript::CQ_SunnyvaleMasterData::Init(){}
long NPCAttitudes[3];
template<class T>T defaultOf(){return T();}
template<class T>T valueFor(int seed){return static_cast<T>(seed);}
template<>bool valueFor<bool>(int seed){return (seed&1)!=0;}
template<>float valueFor<float>(int seed){return float(seed)+0.25f;}
template<>CCharString valueFor<CCharString>(int seed){std::ostringstream s;s<<seed;return CCharString(s.str().c_str());}
template<class T>std::string encode(const T&v){std::ostringstream s;s<<v;return s.str();}
template<>std::string encode<CCharString>(const CCharString&v){return v.value;}
struct Entry{std::string key,before,def,after;const void*address;Entry(const char*k,const std::string&b,const std::string&d,const std::string&a,const void*p):key(k),before(b),def(d),after(a),address(p){}};
class CPersistContext{public:int mode,defaults;std::vector<Entry> entries;std::vector<long> originalAttitudes;std::vector<bool> originalBooks;CPersistContext(int m):mode(m),defaults(0){}template<class T>void Transfer(const char*k,T&v,const T&def){std::string before=encode(v);if(mode==1)v=valueFor<T>(int(entries.size())+201);if(mode==2)v=def;entries.push_back(Entry(k,before,encode(def),encode(v),&v));}template<class T>void Transfer(const char*k,T&v){++defaults;Transfer(k,v,T());}void TransferVector(const char*k,std::vector<long>&v){assert(std::string(k)=="NPCAttitudesTransferVector"&&v.size()==3);originalAttitudes=v;entries.push_back(Entry(k,"vector<long>","","",0));if(mode)v.assign(5,999);}void TransferVector(const char*k,std::vector<bool>&v){assert(std::string(k)=="BooksPreviouslyOpened_Vector"&&v.size()==3);originalBooks=v;entries.push_back(Entry(k,"vector<bool>","","",0));if(mode)v.assign(5,true);}};

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

int main(){int cases=0;for(int mode=0;mode<3;++mode)for(int mask=0;mask<8;++mask)for(int edge=0;edge<4;++edge)for(int pattern=0;pattern<2;++pattern){assert(strings==0);{NScript::CQ_SunnyvaleMasterData data;
data.PostSavePosition=valueFor<EGameflowPosition>(11+pattern);
data.HenchmanCommentaryState=valueFor<long>(12+pattern);
data.HenchmanOverrideState=valueFor<long>(13+pattern);
data.HenchmanSacrificedBodge=valueFor<bool>(14+pattern);
data.HauntedBarrowFieldsCompleted=valueFor<bool>(15+pattern);
data.RandomEntityVillagerMax=valueFor<long>(16+pattern);
data.RandomEntityGuardMax=valueFor<long>(17+pattern);
data.RandomEntityBanditMax=valueFor<long>(18+pattern);
data.RandomEntityVillagerCount=valueFor<long>(19+pattern);
data.RandomEntityGuardCount=valueFor<long>(20+pattern);
data.RandomEntityBanditCount=valueFor<long>(21+pattern);
data.RandomEntityVillagerPercentage=valueFor<float>(22+pattern);
data.RandomEntityGuardPercentage=valueFor<float>(23+pattern);
data.RandomEntityBanditPercentage=valueFor<float>(24+pattern);
data.RandomEntitySheriffAlive=valueFor<bool>(25+pattern);
data.RandomEntityLieutenantAlive=valueFor<bool>(26+pattern);
data.BanditCampTwinbladeKilled=valueFor<bool>(27+pattern);
data.ArcheryStateCurrent=valueFor<NScript::CQ_SunnyvaleMasterData::ArcheryOpenState>(28+pattern);
data.ArcheryStateRequired=valueFor<NScript::CQ_SunnyvaleMasterData::ArcheryOpenState>(29+pattern);
data.ArcheryHighScore=valueFor<long>(30+pattern);
data.FriendOfForeman=valueFor<bool>(31+pattern);
data.BridgeOpened=valueFor<bool>(32+pattern);
data.GrannyMemoryReturned=valueFor<bool>(33+pattern);
data.IsLunaHuman=valueFor<bool>(34+pattern);
data.CondemnedManDead=valueFor<bool>(35+pattern);
data.CondemnedManForgiven=valueFor<bool>(36+pattern);
data.CondemnedManMeetsBodyGuard=valueFor<bool>(37+pattern);
data.CondemnedManMeetsBodyGuardCutSceneStart=valueFor<bool>(38+pattern);
data.CondemnedManMeetsBodyGuardCutSceneFinished=valueFor<bool>(39+pattern);
data.TeddySolution=valueFor<CCharString>(40+pattern);
data.OrchardFarmRaidLastCompleted=valueFor<long>(41+pattern);
data.OrchardFarmTraderEscortCounter=valueFor<long>(42+pattern);
data.SeenAbbeyMotherAtGuild=valueFor<bool>(43+pattern);
data.DefeatedThunder=valueFor<bool>(44+pattern);
data.LostToThunder=valueFor<bool>(45+pattern);
data.KilledThunder=valueFor<bool>(46+pattern);
data.CollectedSoulFromArena=valueFor<bool>(47+pattern);
data.KilledBriar=valueFor<bool>(48+pattern);
data.CollectedSoulFromMother=valueFor<bool>(49+pattern);
data.KilledGM=valueFor<bool>(50+pattern);
data.CollectedSoulFromNostro=valueFor<bool>(51+pattern);
data.DeliveredSoul=valueFor<long>(52+pattern);
data.CurrentHeroSoulsPosition=valueFor<EPlaceInHeroSouls>(53+pattern);
data.WhisperKilledByHero=valueFor<bool>(54+pattern);
data.ArenaFinished=valueFor<bool>(55+pattern);
data.GatesRequireClosing=valueFor<bool>(56+pattern);
data.GatesRequireOpening=valueFor<bool>(57+pattern);
data.HeroDrunkness=valueFor<long>(58+pattern);
data.TrophyDealerHeroSpokenToDemonDoors=valueFor<bool>(59+pattern);
data.VillagerAngryRating=valueFor<float>(60+pattern);
data.MotherDodgingKraken=valueFor<bool>(61+pattern);
data.JackBossBattleResult=valueFor<long>(62+pattern);
data.JackBossBattleHeroGoodAtEnd=valueFor<bool>(63+pattern);
data.WifeLeavingYou=valueFor<bool>(64+pattern);
data.WifeLeftYou=valueFor<bool>(65+pattern);
data.CurrentScene=valueFor<long>(66+pattern);
data.StartNextScene=valueFor<bool>(67+pattern);
data.CurrentChapter=valueFor<long>(68+pattern);
data.ScorpionsDestroyed=valueFor<bool>(69+pattern);
data.ScorpionsDestroyedCutscenePlayed=valueFor<bool>(70+pattern);
data.SkillTrainingStarted=valueFor<bool>(71+pattern);
data.WillTrainingStarted=valueFor<bool>(72+pattern);
data.MovingDummiesNeeded=valueFor<bool>(73+pattern);
data.SkillScore=valueFor<long>(74+pattern);
data.HighestSkillScore=valueFor<long>(75+pattern);
data.WillScore=valueFor<long>(76+pattern);
data.MeleeApprenticeNeededForCutscene=valueFor<bool>(77+pattern);
data.GlobalMeleeGrade=valueFor<long>(78+pattern);
data.GlobalSkillGrade=valueFor<long>(79+pattern);
data.GlobalWillGrade=valueFor<long>(80+pattern);
data.GuildWarningOccuring=valueFor<bool>(81+pattern);
data.SkillTestOccuring=valueFor<bool>(82+pattern);
data.WillTestOccuring=valueFor<bool>(83+pattern);
data.SkillRepeating=valueFor<bool>(84+pattern);
data.SkillRepeatKnown=valueFor<bool>(85+pattern);
data.SkillDummyReset=valueFor<bool>(86+pattern);
data.HeroTakingGuildTest=valueFor<bool>(87+pattern);
data.SwordInTheStoneComplete=valueFor<bool>(88+pattern);
data.AmbushTradersAllGuardsDead=valueFor<bool>(89+pattern);
data.AmbushTradersAllTradersDead=valueFor<bool>(90+pattern);
data.AmbushTradersKillCount=valueFor<long>(91+pattern);
data.AmbushTradersBanditHireCount=valueFor<long>(92+pattern);
data.AmbushTradersSpyDead=valueFor<bool>(93+pattern);
data.StruckDealWithLadyGrey=valueFor<bool>(94+pattern);
data.HeroExposedLadyGrey=valueFor<bool>(95+pattern);
data.HeroMarriedLadyGrey=valueFor<bool>(96+pattern);
data.BountyHuntWithinTimeLimit=valueFor<bool>(97+pattern);
data.BountyHuntDecapitation=valueFor<bool>(98+pattern);
data.BountyHuntTimeLimitExceeded=valueFor<bool>(99+pattern);
data.AchievementsWorthyOfSong=valueFor<unsigned long>(100+pattern);
data.BreakSiegeFinished=valueFor<bool>(101+pattern);
data.WhiteBalverineFinished=valueFor<bool>(102+pattern);
data.MadBomberFinished=valueFor<bool>(103+pattern);
data.BodyGuardsMustStandAndWait=valueFor<bool>(104+pattern);
data.BodyGuardsInLimbo=valueFor<bool>(105+pattern);
data.WifeHeroFlirting=valueFor<bool>(106+pattern);
data.PrisonRaceNumber=valueFor<long>(107+pattern);
data.PrisonRaceWonByHero=valueFor<bool>(108+pattern);
data.WasPrisonRaceEverWonByHero=valueFor<bool>(109+pattern);
data.PrisonKeyStolenByHero=valueFor<bool>(110+pattern);
for(int i=0;i<3;++i)data.BooksPreviouslyOpened[i]=(mask&(1<<i))!=0;
data.PrisonTorturedBefore=valueFor<bool>(112+pattern);
data.PrisonGameDoneBefore=valueFor<bool>(113+pattern);
data.HeroDollsScriptUsingTeacher=valueFor<bool>(114+pattern);
data.MaxChickenKickingScore=valueFor<long>(115+pattern);
data.StoryTellerSpecialStories=valueFor<unsigned long>(116+pattern);
data.StoryTellerToldSpecialStories=valueFor<unsigned long>(117+pattern);
data.GHeroName=valueFor<EHeroNames>(118+pattern);
data.TimeAdvancePointTriggered=valueFor<bool>(119+pattern);
data.OFBRCratesStolen=valueFor<bool>(120+pattern);
data.OFBR_NoCratesWereStolen=valueFor<bool>(121+pattern);
data.DarkwoodAllTradersAlive=valueFor<bool>(122+pattern);
data.MadBomberNoBombsExplode=valueFor<bool>(123+pattern);
data.HobbeCavePerpertratorKilled=valueFor<bool>(124+pattern);
data.HobbeCaveInnocentSacrificed=valueFor<long>(125+pattern);
data.HobbeCaveBoyUnharmed=valueFor<bool>(126+pattern);
data.BanditCampKillNoBandits=valueFor<bool>(127+pattern);
data.BreakSiegeNoAssistance=valueFor<bool>(128+pattern);
data.BreakSiegeKilledLeader=valueFor<bool>(129+pattern);
data.OrchardFarmBanditKilled=valueFor<bool>(130+pattern);
data.OrchardFarmGuardKilled=valueFor<bool>(131+pattern);
data.BanditCampKillManyBandits=valueFor<bool>(132+pattern);
data.HobbeContestKillMoreHobbes=valueFor<bool>(133+pattern);
data.GraveyardKillAllInnocents=valueFor<bool>(134+pattern);
data.DarkwoodPickpocketedAllTraders=valueFor<long>(135+pattern);
data.HangingTreeBanditKilled=valueFor<bool>(136+pattern);
data.HangingTreeGuardKilled=valueFor<bool>(137+pattern);
data.TCGKillNoBandits=valueFor<bool>(138+pattern);
data.TCGMadeTimeLimit=valueFor<bool>(139+pattern);
data.TCEKeepBanditFollowerAlive=valueFor<bool>(140+pattern);
data.TCEMadeTimeLimit=valueFor<bool>(141+pattern);
data.RansomVictimVictimNoDamage=valueFor<bool>(142+pattern);
data.RansomVictimKidnappersKilled=valueFor<bool>(143+pattern);
data.RansomVictimSaveVictim=valueFor<bool>(144+pattern);
data.RansomVictimHaveVictimKilled=valueFor<bool>(145+pattern);
data.MinionCampBriarNoDamage=valueFor<bool>(146+pattern);
data.FireHeartFreeAllPrisoners=valueFor<bool>(147+pattern);
data.FireHeartKillAllPrisoners=valueFor<bool>(148+pattern);
data.TCETimeLimitBoastTaken=valueFor<bool>(149+pattern);
data.TCGTimeLimitBoastTaken=valueFor<bool>(150+pattern);
data.HeroWoreMask=valueFor<bool>(151+pattern);
data.SingingStonesInSync=valueFor<bool>(152+pattern);
data.BoastRegions.push_back(CCharString("untouched"));
long edges[4]={0L,-1L,2147483647L,(-2147483647L-1L)};for(int i=0;i<3;++i)NPCAttitudes[i]=edges[(edge+i)%4];CPersistContext context(mode);data.OnPersist(context);
assert(context.entries.size()==81);
assert(context.entries[0].key=="PostSavePosition");
assert(context.entries[0].address==&data.PostSavePosition);assert(context.entries[0].before==encode(valueFor<EGameflowPosition>(11+pattern)));assert(context.entries[0].def==encode(defaultOf<EGameflowPosition>()));assert(data.PostSavePosition==(mode==0?valueFor<EGameflowPosition>(11+pattern):mode==1?valueFor<EGameflowPosition>(201):defaultOf<EGameflowPosition>()));
assert(context.entries[1].key=="HauntedBarrowFieldsCompleted");
assert(context.entries[1].address==&data.HauntedBarrowFieldsCompleted);assert(context.entries[1].before==encode(valueFor<bool>(15+pattern)));assert(context.entries[1].def==encode(defaultOf<bool>()));assert(data.HauntedBarrowFieldsCompleted==(mode==0?valueFor<bool>(15+pattern):mode==1?valueFor<bool>(202):defaultOf<bool>()));
assert(context.entries[2].key=="GrannyMemoryReturned");
assert(context.entries[2].address==&data.GrannyMemoryReturned);assert(context.entries[2].before==encode(valueFor<bool>(33+pattern)));assert(context.entries[2].def==encode(defaultOf<bool>()));assert(data.GrannyMemoryReturned==(mode==0?valueFor<bool>(33+pattern):mode==1?valueFor<bool>(203):defaultOf<bool>()));
assert(context.entries[3].key=="IsLunaHuman");
assert(context.entries[3].address==&data.IsLunaHuman);assert(context.entries[3].before==encode(valueFor<bool>(34+pattern)));assert(context.entries[3].def==encode(defaultOf<bool>()));assert(data.IsLunaHuman==(mode==0?valueFor<bool>(34+pattern):mode==1?valueFor<bool>(204):defaultOf<bool>()));
assert(context.entries[4].key=="FriendOfForeman");
assert(context.entries[4].address==&data.FriendOfForeman);assert(context.entries[4].before==encode(valueFor<bool>(31+pattern)));assert(context.entries[4].def==encode(defaultOf<bool>()));assert(data.FriendOfForeman==(mode==0?valueFor<bool>(31+pattern):mode==1?valueFor<bool>(205):defaultOf<bool>()));
assert(context.entries[5].key=="BridgeOpened");
assert(context.entries[5].address==&data.BridgeOpened);assert(context.entries[5].before==encode(valueFor<bool>(32+pattern)));assert(context.entries[5].def==encode(defaultOf<bool>()));assert(data.BridgeOpened==(mode==0?valueFor<bool>(32+pattern):mode==1?valueFor<bool>(206):defaultOf<bool>()));
assert(context.entries[6].key=="ArcheryHighScore");
assert(context.entries[6].address==&data.ArcheryHighScore);assert(context.entries[6].before==encode(valueFor<long>(30+pattern)));assert(context.entries[6].def==encode(defaultOf<long>()));assert(data.ArcheryHighScore==(mode==0?valueFor<long>(30+pattern):mode==1?valueFor<long>(207):defaultOf<long>()));
assert(context.entries[7].key=="ArcheryStateCurrent");
assert(context.entries[7].address==&data.ArcheryStateCurrent);assert(context.entries[7].before==encode(valueFor<NScript::CQ_SunnyvaleMasterData::ArcheryOpenState>(28+pattern)));assert(context.entries[7].def==encode(defaultOf<NScript::CQ_SunnyvaleMasterData::ArcheryOpenState>()));assert(data.ArcheryStateCurrent==(mode==0?valueFor<NScript::CQ_SunnyvaleMasterData::ArcheryOpenState>(28+pattern):mode==1?valueFor<NScript::CQ_SunnyvaleMasterData::ArcheryOpenState>(208):defaultOf<NScript::CQ_SunnyvaleMasterData::ArcheryOpenState>()));
assert(context.entries[8].key=="ArcheryStateRequired");
assert(context.entries[8].address==&data.ArcheryStateRequired);assert(context.entries[8].before==encode(valueFor<NScript::CQ_SunnyvaleMasterData::ArcheryOpenState>(29+pattern)));assert(context.entries[8].def==encode(defaultOf<NScript::CQ_SunnyvaleMasterData::ArcheryOpenState>()));assert(data.ArcheryStateRequired==(mode==0?valueFor<NScript::CQ_SunnyvaleMasterData::ArcheryOpenState>(29+pattern):mode==1?valueFor<NScript::CQ_SunnyvaleMasterData::ArcheryOpenState>(209):defaultOf<NScript::CQ_SunnyvaleMasterData::ArcheryOpenState>()));
assert(context.entries[9].key=="CondemnedManDead");
assert(context.entries[9].address==&data.CondemnedManDead);assert(context.entries[9].before==encode(valueFor<bool>(35+pattern)));assert(context.entries[9].def==encode(defaultOf<bool>()));assert(data.CondemnedManDead==(mode==0?valueFor<bool>(35+pattern):mode==1?valueFor<bool>(210):defaultOf<bool>()));
assert(context.entries[10].key=="CondemnedManForgiven");
assert(context.entries[10].address==&data.CondemnedManForgiven);assert(context.entries[10].before==encode(valueFor<bool>(36+pattern)));assert(context.entries[10].def==encode(defaultOf<bool>()));assert(data.CondemnedManForgiven==(mode==0?valueFor<bool>(36+pattern):mode==1?valueFor<bool>(211):defaultOf<bool>()));
assert(context.entries[11].key=="CondemnedManMeetsBodyGuard");
assert(context.entries[11].address==&data.CondemnedManMeetsBodyGuard);assert(context.entries[11].before==encode(valueFor<bool>(37+pattern)));assert(context.entries[11].def==encode(defaultOf<bool>()));assert(data.CondemnedManMeetsBodyGuard==(mode==0?valueFor<bool>(37+pattern):mode==1?valueFor<bool>(212):defaultOf<bool>()));
assert(context.entries[12].key=="CondemnedManMeetsBodyGuardCutSceneStart");
assert(context.entries[12].address==&data.CondemnedManMeetsBodyGuardCutSceneStart);assert(context.entries[12].before==encode(valueFor<bool>(38+pattern)));assert(context.entries[12].def==encode(defaultOf<bool>()));assert(data.CondemnedManMeetsBodyGuardCutSceneStart==(mode==0?valueFor<bool>(38+pattern):mode==1?valueFor<bool>(213):defaultOf<bool>()));
assert(context.entries[13].key=="CondemnedManMeetsBodyGuardCutSceneFinished");
assert(context.entries[13].address==&data.CondemnedManMeetsBodyGuardCutSceneFinished);assert(context.entries[13].before==encode(valueFor<bool>(39+pattern)));assert(context.entries[13].def==encode(defaultOf<bool>()));assert(data.CondemnedManMeetsBodyGuardCutSceneFinished==(mode==0?valueFor<bool>(39+pattern):mode==1?valueFor<bool>(214):defaultOf<bool>()));
assert(context.entries[14].key=="TeddySolution");
assert(context.entries[14].address==&data.TeddySolution);assert(context.entries[14].before==encode(valueFor<CCharString>(40+pattern)));assert(context.entries[14].def==encode(defaultOf<CCharString>()));assert(data.TeddySolution==(mode==0?valueFor<CCharString>(40+pattern):mode==1?valueFor<CCharString>(215):defaultOf<CCharString>()));
assert(context.entries[15].key=="OrchardFarmRaidLastCompleted");
assert(context.entries[15].address==&data.OrchardFarmRaidLastCompleted);assert(context.entries[15].before==encode(valueFor<long>(41+pattern)));assert(context.entries[15].def==encode(defaultOf<long>()));assert(data.OrchardFarmRaidLastCompleted==(mode==0?valueFor<long>(41+pattern):mode==1?valueFor<long>(216):defaultOf<long>()));
assert(context.entries[16].key=="OrchardFarmTraderEscortCounter");
assert(context.entries[16].address==&data.OrchardFarmTraderEscortCounter);assert(context.entries[16].before==encode(valueFor<long>(42+pattern)));assert(context.entries[16].def==encode(defaultOf<long>()));assert(data.OrchardFarmTraderEscortCounter==(mode==0?valueFor<long>(42+pattern):mode==1?valueFor<long>(217):defaultOf<long>()));
assert(context.entries[17].key=="SeenAbbeyMotherAtGuild");
assert(context.entries[17].address==&data.SeenAbbeyMotherAtGuild);assert(context.entries[17].before==encode(valueFor<bool>(43+pattern)));assert(context.entries[17].def==encode(defaultOf<bool>()));assert(data.SeenAbbeyMotherAtGuild==(mode==0?valueFor<bool>(43+pattern):mode==1?valueFor<bool>(218):defaultOf<bool>()));
assert(context.entries[18].key=="DefeatedThunder");
assert(context.entries[18].address==&data.DefeatedThunder);assert(context.entries[18].before==encode(valueFor<bool>(44+pattern)));assert(context.entries[18].def==encode(defaultOf<bool>()));assert(data.DefeatedThunder==(mode==0?valueFor<bool>(44+pattern):mode==1?valueFor<bool>(219):defaultOf<bool>()));
assert(context.entries[19].key=="LostToThunder");
assert(context.entries[19].address==&data.LostToThunder);assert(context.entries[19].before==encode(valueFor<bool>(45+pattern)));assert(context.entries[19].def==encode(defaultOf<bool>()));assert(data.LostToThunder==(mode==0?valueFor<bool>(45+pattern):mode==1?valueFor<bool>(220):defaultOf<bool>()));
assert(context.entries[20].key=="KilledThunder");
assert(context.entries[20].address==&data.KilledThunder);assert(context.entries[20].before==encode(valueFor<bool>(46+pattern)));assert(context.entries[20].def==encode(defaultOf<bool>()));assert(data.KilledThunder==(mode==0?valueFor<bool>(46+pattern):mode==1?valueFor<bool>(221):defaultOf<bool>()));
assert(context.entries[21].key=="CollectedSoulFromArena");
assert(context.entries[21].address==&data.CollectedSoulFromArena);assert(context.entries[21].before==encode(valueFor<bool>(47+pattern)));assert(context.entries[21].def==encode(defaultOf<bool>()));assert(data.CollectedSoulFromArena==(mode==0?valueFor<bool>(47+pattern):mode==1?valueFor<bool>(222):defaultOf<bool>()));
assert(context.entries[22].key=="KilledBriar");
assert(context.entries[22].address==&data.KilledBriar);assert(context.entries[22].before==encode(valueFor<bool>(48+pattern)));assert(context.entries[22].def==encode(defaultOf<bool>()));assert(data.KilledBriar==(mode==0?valueFor<bool>(48+pattern):mode==1?valueFor<bool>(223):defaultOf<bool>()));
assert(context.entries[23].key=="CollectedSoulFromMother");
assert(context.entries[23].address==&data.CollectedSoulFromMother);assert(context.entries[23].before==encode(valueFor<bool>(49+pattern)));assert(context.entries[23].def==encode(defaultOf<bool>()));assert(data.CollectedSoulFromMother==(mode==0?valueFor<bool>(49+pattern):mode==1?valueFor<bool>(224):defaultOf<bool>()));
assert(context.entries[24].key=="KilledGM");
assert(context.entries[24].address==&data.KilledGM);assert(context.entries[24].before==encode(valueFor<bool>(50+pattern)));assert(context.entries[24].def==encode(defaultOf<bool>()));assert(data.KilledGM==(mode==0?valueFor<bool>(50+pattern):mode==1?valueFor<bool>(225):defaultOf<bool>()));
assert(context.entries[25].key=="CollectedSoulFromNostro");
assert(context.entries[25].address==&data.CollectedSoulFromNostro);assert(context.entries[25].before==encode(valueFor<bool>(51+pattern)));assert(context.entries[25].def==encode(defaultOf<bool>()));assert(data.CollectedSoulFromNostro==(mode==0?valueFor<bool>(51+pattern):mode==1?valueFor<bool>(226):defaultOf<bool>()));
assert(context.entries[26].key=="DeliveredSoul");
assert(context.entries[26].address==&data.DeliveredSoul);assert(context.entries[26].before==encode(valueFor<long>(52+pattern)));assert(context.entries[26].def==encode(defaultOf<long>()));assert(data.DeliveredSoul==(mode==0?valueFor<long>(52+pattern):mode==1?valueFor<long>(227):defaultOf<long>()));
assert(context.entries[27].key=="CurrentHeroSoulsPosition");
assert(context.entries[27].address==&data.CurrentHeroSoulsPosition);assert(context.entries[27].before==encode(valueFor<EPlaceInHeroSouls>(53+pattern)));assert(context.entries[27].def==encode(defaultOf<EPlaceInHeroSouls>()));assert(data.CurrentHeroSoulsPosition==(mode==0?valueFor<EPlaceInHeroSouls>(53+pattern):mode==1?valueFor<EPlaceInHeroSouls>(228):defaultOf<EPlaceInHeroSouls>()));
assert(context.entries[28].key=="WhisperKilledByHero");
assert(context.entries[28].address==&data.WhisperKilledByHero);assert(context.entries[28].before==encode(valueFor<bool>(54+pattern)));assert(context.entries[28].def==encode(defaultOf<bool>()));assert(data.WhisperKilledByHero==(mode==0?valueFor<bool>(54+pattern):mode==1?valueFor<bool>(229):defaultOf<bool>()));
assert(context.entries[29].key=="ArenaFinished");
assert(context.entries[29].address==&data.ArenaFinished);assert(context.entries[29].before==encode(valueFor<bool>(55+pattern)));assert(context.entries[29].def==encode(defaultOf<bool>()));assert(data.ArenaFinished==(mode==0?valueFor<bool>(55+pattern):mode==1?valueFor<bool>(230):defaultOf<bool>()));
assert(context.entries[30].key=="HangingTreeBanditKilled");
assert(context.entries[30].address==&data.HangingTreeBanditKilled);assert(context.entries[30].before==encode(valueFor<bool>(136+pattern)));assert(context.entries[30].def==encode(defaultOf<bool>()));assert(data.HangingTreeBanditKilled==(mode==0?valueFor<bool>(136+pattern):mode==1?valueFor<bool>(231):defaultOf<bool>()));
assert(context.entries[31].key=="HangingTreeGuardKilled");
assert(context.entries[31].address==&data.HangingTreeGuardKilled);assert(context.entries[31].before==encode(valueFor<bool>(137+pattern)));assert(context.entries[31].def==encode(defaultOf<bool>()));assert(data.HangingTreeGuardKilled==(mode==0?valueFor<bool>(137+pattern):mode==1?valueFor<bool>(232):defaultOf<bool>()));
assert(context.entries[32].key=="GatesRequireClosing");
assert(context.entries[32].address==&data.GatesRequireClosing);assert(context.entries[32].before==encode(valueFor<bool>(56+pattern)));assert(context.entries[32].def==encode(defaultOf<bool>()));assert(data.GatesRequireClosing==(mode==0?valueFor<bool>(56+pattern):mode==1?valueFor<bool>(233):defaultOf<bool>()));
assert(context.entries[33].key=="GatesRequireOpening");
assert(context.entries[33].address==&data.GatesRequireOpening);assert(context.entries[33].before==encode(valueFor<bool>(57+pattern)));assert(context.entries[33].def==encode(defaultOf<bool>()));assert(data.GatesRequireOpening==(mode==0?valueFor<bool>(57+pattern):mode==1?valueFor<bool>(234):defaultOf<bool>()));
assert(context.entries[34].key=="AchievementsWorthyOfSong");
assert(context.entries[34].address==&data.AchievementsWorthyOfSong);assert(context.entries[34].before==encode(valueFor<unsigned long>(100+pattern)));assert(context.entries[34].def==encode(defaultOf<unsigned long>()));assert(data.AchievementsWorthyOfSong==(mode==0?valueFor<unsigned long>(100+pattern):mode==1?valueFor<unsigned long>(235):defaultOf<unsigned long>()));
assert(context.entries[35].key=="StoryTellerSpecialStories");
assert(context.entries[35].address==&data.StoryTellerSpecialStories);assert(context.entries[35].before==encode(valueFor<unsigned long>(116+pattern)));assert(context.entries[35].def==encode(defaultOf<unsigned long>()));assert(data.StoryTellerSpecialStories==(mode==0?valueFor<unsigned long>(116+pattern):mode==1?valueFor<unsigned long>(236):defaultOf<unsigned long>()));
assert(context.entries[36].key=="StoryTellerToldSpecialStories");
assert(context.entries[36].address==&data.StoryTellerToldSpecialStories);assert(context.entries[36].before==encode(valueFor<unsigned long>(117+pattern)));assert(context.entries[36].def==encode(defaultOf<unsigned long>()));assert(data.StoryTellerToldSpecialStories==(mode==0?valueFor<unsigned long>(117+pattern):mode==1?valueFor<unsigned long>(237):defaultOf<unsigned long>()));
assert(context.entries[37].key=="HeroDrunkness");
assert(context.entries[37].address==&data.HeroDrunkness);assert(context.entries[37].before==encode(valueFor<long>(58+pattern)));assert(context.entries[37].def==encode(defaultOf<long>()));assert(data.HeroDrunkness==(mode==0?valueFor<long>(58+pattern):mode==1?valueFor<long>(238):defaultOf<long>()));
assert(context.entries[38].key=="VillagerAngryRating");
assert(context.entries[38].address==&data.VillagerAngryRating);assert(context.entries[38].before==encode(valueFor<float>(60+pattern)));assert(context.entries[38].def==encode(defaultOf<float>()));assert(data.VillagerAngryRating==(mode==0?valueFor<float>(60+pattern):mode==1?valueFor<float>(239):defaultOf<float>()));
assert(context.entries[39].key=="TrophyDealerHeroSpokenToDemonDoors");
assert(context.entries[39].address==&data.TrophyDealerHeroSpokenToDemonDoors);assert(context.entries[39].before==encode(valueFor<bool>(59+pattern)));assert(context.entries[39].def==encode(defaultOf<bool>()));assert(data.TrophyDealerHeroSpokenToDemonDoors==(mode==0?valueFor<bool>(59+pattern):mode==1?valueFor<bool>(240):defaultOf<bool>()));
assert(context.entries[40].key=="StruckDealWithLadyGrey");
assert(context.entries[40].address==&data.StruckDealWithLadyGrey);assert(context.entries[40].before==encode(valueFor<bool>(94+pattern)));assert(context.entries[40].def==encode(defaultOf<bool>()));assert(data.StruckDealWithLadyGrey==(mode==0?valueFor<bool>(94+pattern):mode==1?valueFor<bool>(241):defaultOf<bool>()));
assert(context.entries[41].key=="HeroExposedLadyGrey");
assert(context.entries[41].address==&data.HeroExposedLadyGrey);assert(context.entries[41].before==encode(valueFor<bool>(95+pattern)));assert(context.entries[41].def==encode(defaultOf<bool>()));assert(data.HeroExposedLadyGrey==(mode==0?valueFor<bool>(95+pattern):mode==1?valueFor<bool>(242):defaultOf<bool>()));
assert(context.entries[42].key=="HeroMarriedLadyGrey");
assert(context.entries[42].address==&data.HeroMarriedLadyGrey);assert(context.entries[42].before==encode(valueFor<bool>(96+pattern)));assert(context.entries[42].def==encode(defaultOf<bool>()));assert(data.HeroMarriedLadyGrey==(mode==0?valueFor<bool>(96+pattern):mode==1?valueFor<bool>(243):defaultOf<bool>()));
assert(context.entries[43].key=="TimeAdvancePointTriggered");
assert(context.entries[43].address==&data.TimeAdvancePointTriggered);assert(context.entries[43].before==encode(valueFor<bool>(119+pattern)));assert(context.entries[43].def==encode(defaultOf<bool>()));assert(data.TimeAdvancePointTriggered==(mode==0?valueFor<bool>(119+pattern):mode==1?valueFor<bool>(244):defaultOf<bool>()));
assert(context.entries[44].key=="ScorpionsDestroyed");
assert(context.entries[44].address==&data.ScorpionsDestroyed);assert(context.entries[44].before==encode(valueFor<bool>(69+pattern)));assert(context.entries[44].def==encode(defaultOf<bool>()));assert(data.ScorpionsDestroyed==(mode==0?valueFor<bool>(69+pattern):mode==1?valueFor<bool>(245):defaultOf<bool>()));
assert(context.entries[45].key=="GuildWarningOccuring");
assert(context.entries[45].address==&data.GuildWarningOccuring);assert(context.entries[45].before==encode(valueFor<bool>(81+pattern)));assert(context.entries[45].def==encode(defaultOf<bool>()));assert(data.GuildWarningOccuring==(mode==0?valueFor<bool>(81+pattern):mode==1?valueFor<bool>(246):defaultOf<bool>()));
assert(context.entries[46].key=="SkillTestOccuring");
assert(context.entries[46].address==&data.SkillTestOccuring);assert(context.entries[46].before==encode(valueFor<bool>(82+pattern)));assert(context.entries[46].def==encode(defaultOf<bool>()));assert(data.SkillTestOccuring==(mode==0?valueFor<bool>(82+pattern):mode==1?valueFor<bool>(247):defaultOf<bool>()));
assert(context.entries[47].key=="WillTestOccuring");
assert(context.entries[47].address==&data.WillTestOccuring);assert(context.entries[47].before==encode(valueFor<bool>(83+pattern)));assert(context.entries[47].def==encode(defaultOf<bool>()));assert(data.WillTestOccuring==(mode==0?valueFor<bool>(83+pattern):mode==1?valueFor<bool>(248):defaultOf<bool>()));
assert(context.entries[48].key=="ScorpionsDestroyedCutscenePlayed");
assert(context.entries[48].address==&data.ScorpionsDestroyedCutscenePlayed);assert(context.entries[48].before==encode(valueFor<bool>(70+pattern)));assert(context.entries[48].def==encode(defaultOf<bool>()));assert(data.ScorpionsDestroyedCutscenePlayed==(mode==0?valueFor<bool>(70+pattern):mode==1?valueFor<bool>(249):defaultOf<bool>()));
assert(context.entries[49].key=="SkillTrainingStarted");
assert(context.entries[49].address==&data.SkillTrainingStarted);assert(context.entries[49].before==encode(valueFor<bool>(71+pattern)));assert(context.entries[49].def==encode(defaultOf<bool>()));assert(data.SkillTrainingStarted==(mode==0?valueFor<bool>(71+pattern):mode==1?valueFor<bool>(250):defaultOf<bool>()));
assert(context.entries[50].key=="WillTrainingStarted");
assert(context.entries[50].address==&data.WillTrainingStarted);assert(context.entries[50].before==encode(valueFor<bool>(72+pattern)));assert(context.entries[50].def==encode(defaultOf<bool>()));assert(data.WillTrainingStarted==(mode==0?valueFor<bool>(72+pattern):mode==1?valueFor<bool>(251):defaultOf<bool>()));
assert(context.entries[51].key=="MovingDummiesNeeded");
assert(context.entries[51].address==&data.MovingDummiesNeeded);assert(context.entries[51].before==encode(valueFor<bool>(73+pattern)));assert(context.entries[51].def==encode(defaultOf<bool>()));assert(data.MovingDummiesNeeded==(mode==0?valueFor<bool>(73+pattern):mode==1?valueFor<bool>(252):defaultOf<bool>()));
assert(context.entries[52].key=="MeleeApprenticeNeededForCutscene");
assert(context.entries[52].address==&data.MeleeApprenticeNeededForCutscene);assert(context.entries[52].before==encode(valueFor<bool>(77+pattern)));assert(context.entries[52].def==encode(defaultOf<bool>()));assert(data.MeleeApprenticeNeededForCutscene==(mode==0?valueFor<bool>(77+pattern):mode==1?valueFor<bool>(253):defaultOf<bool>()));
assert(context.entries[53].key=="GlobalMeleeGrade");
assert(context.entries[53].address==&data.GlobalMeleeGrade);assert(context.entries[53].before==encode(valueFor<long>(78+pattern)));assert(context.entries[53].def==encode(defaultOf<long>()));assert(data.GlobalMeleeGrade==(mode==0?valueFor<long>(78+pattern):mode==1?valueFor<long>(254):defaultOf<long>()));
assert(context.entries[54].key=="GlobalSkillGrade");
assert(context.entries[54].address==&data.GlobalSkillGrade);assert(context.entries[54].before==encode(valueFor<long>(79+pattern)));assert(context.entries[54].def==encode(defaultOf<long>()));assert(data.GlobalSkillGrade==(mode==0?valueFor<long>(79+pattern):mode==1?valueFor<long>(255):defaultOf<long>()));
assert(context.entries[55].key=="GlobalWillGrade");
assert(context.entries[55].address==&data.GlobalWillGrade);assert(context.entries[55].before==encode(valueFor<long>(80+pattern)));assert(context.entries[55].def==encode(defaultOf<long>()));assert(data.GlobalWillGrade==(mode==0?valueFor<long>(80+pattern):mode==1?valueFor<long>(256):defaultOf<long>()));
assert(context.entries[56].key=="SkillRepeating");
assert(context.entries[56].address==&data.SkillRepeating);assert(context.entries[56].before==encode(valueFor<bool>(84+pattern)));assert(context.entries[56].def==encode(defaultOf<bool>()));assert(data.SkillRepeating==(mode==0?valueFor<bool>(84+pattern):mode==1?valueFor<bool>(257):defaultOf<bool>()));
assert(context.entries[57].key=="SkillRepeatKnown");
assert(context.entries[57].address==&data.SkillRepeatKnown);assert(context.entries[57].before==encode(valueFor<bool>(85+pattern)));assert(context.entries[57].def==encode(defaultOf<bool>()));assert(data.SkillRepeatKnown==(mode==0?valueFor<bool>(85+pattern):mode==1?valueFor<bool>(258):defaultOf<bool>()));
assert(context.entries[58].key=="SkillDummyReset");
assert(context.entries[58].address==&data.SkillDummyReset);assert(context.entries[58].before==encode(valueFor<bool>(86+pattern)));assert(context.entries[58].def==encode(defaultOf<bool>()));assert(data.SkillDummyReset==(mode==0?valueFor<bool>(86+pattern):mode==1?valueFor<bool>(259):defaultOf<bool>()));
assert(context.entries[59].key=="HighestSkillScore");
assert(context.entries[59].address==&data.HighestSkillScore);assert(context.entries[59].before==encode(valueFor<long>(75+pattern)));assert(context.entries[59].def==encode(defaultOf<long>()));assert(data.HighestSkillScore==(mode==0?valueFor<long>(75+pattern):mode==1?valueFor<long>(260):defaultOf<long>()));
assert(context.entries[60].key=="SingingStonesInSync");
assert(context.entries[60].address==&data.SingingStonesInSync);assert(context.entries[60].before==encode(valueFor<bool>(152+pattern)));assert(context.entries[60].def==encode(defaultOf<bool>()));assert(data.SingingStonesInSync==(mode==0?valueFor<bool>(152+pattern):mode==1?valueFor<bool>(261):defaultOf<bool>()));
assert(context.entries[61].key=="SwordInTheStoneComplete");
assert(context.entries[61].address==&data.SwordInTheStoneComplete);assert(context.entries[61].before==encode(valueFor<bool>(88+pattern)));assert(context.entries[61].def==encode(defaultOf<bool>()));assert(data.SwordInTheStoneComplete==(mode==0?valueFor<bool>(88+pattern):mode==1?valueFor<bool>(262):defaultOf<bool>()));
assert(context.entries[62].key=="AmbushTradersAllGuardsDead");
assert(context.entries[62].address==&data.AmbushTradersAllGuardsDead);assert(context.entries[62].before==encode(valueFor<bool>(89+pattern)));assert(context.entries[62].def==encode(defaultOf<bool>()));assert(data.AmbushTradersAllGuardsDead==(mode==0?valueFor<bool>(89+pattern):mode==1?valueFor<bool>(263):defaultOf<bool>()));
assert(context.entries[63].key=="AmbushTradersAllTradersDead");
assert(context.entries[63].address==&data.AmbushTradersAllTradersDead);assert(context.entries[63].before==encode(valueFor<bool>(90+pattern)));assert(context.entries[63].def==encode(defaultOf<bool>()));assert(data.AmbushTradersAllTradersDead==(mode==0?valueFor<bool>(90+pattern):mode==1?valueFor<bool>(264):defaultOf<bool>()));
assert(context.entries[64].key=="AmbushTradersKillCount");
assert(context.entries[64].address==&data.AmbushTradersKillCount);assert(context.entries[64].before==encode(valueFor<long>(91+pattern)));assert(context.entries[64].def==encode(defaultOf<long>()));assert(data.AmbushTradersKillCount==(mode==0?valueFor<long>(91+pattern):mode==1?valueFor<long>(265):defaultOf<long>()));
assert(context.entries[65].key=="AmbushTradersBanditHireCount");
assert(context.entries[65].address==&data.AmbushTradersBanditHireCount);assert(context.entries[65].before==encode(valueFor<long>(92+pattern)));assert(context.entries[65].def==encode(defaultOf<long>()));assert(data.AmbushTradersBanditHireCount==(mode==0?valueFor<long>(92+pattern):mode==1?valueFor<long>(266):defaultOf<long>()));
assert(context.entries[66].key=="AmbushTradersSpyDead");
assert(context.entries[66].address==&data.AmbushTradersSpyDead);assert(context.entries[66].before==encode(valueFor<bool>(93+pattern)));assert(context.entries[66].def==encode(defaultOf<bool>()));assert(data.AmbushTradersSpyDead==(mode==0?valueFor<bool>(93+pattern):mode==1?valueFor<bool>(267):defaultOf<bool>()));
assert(context.entries[67].key=="BountyHuntWithinTimeLimit");
assert(context.entries[67].address==&data.BountyHuntWithinTimeLimit);assert(context.entries[67].before==encode(valueFor<bool>(97+pattern)));assert(context.entries[67].def==encode(defaultOf<bool>()));assert(data.BountyHuntWithinTimeLimit==(mode==0?valueFor<bool>(97+pattern):mode==1?valueFor<bool>(268):defaultOf<bool>()));
assert(context.entries[68].key=="BountyHuntTimeLimitExceeded");
assert(context.entries[68].address==&data.BountyHuntTimeLimitExceeded);assert(context.entries[68].before==encode(valueFor<bool>(99+pattern)));assert(context.entries[68].def==encode(defaultOf<bool>()));assert(data.BountyHuntTimeLimitExceeded==(mode==0?valueFor<bool>(99+pattern):mode==1?valueFor<bool>(269):defaultOf<bool>()));
assert(context.entries[69].key=="BountyHuntDecapitation");
assert(context.entries[69].address==&data.BountyHuntDecapitation);assert(context.entries[69].before==encode(valueFor<bool>(98+pattern)));assert(context.entries[69].def==encode(defaultOf<bool>()));assert(data.BountyHuntDecapitation==(mode==0?valueFor<bool>(98+pattern):mode==1?valueFor<bool>(270):defaultOf<bool>()));
assert(context.entries[70].key=="NPCAttitudesTransferVector");
assert(context.entries[71].key=="BreakSiegeFinished");
assert(context.entries[71].address==&data.BreakSiegeFinished);assert(context.entries[71].before==encode(valueFor<bool>(101+pattern)));assert(context.entries[71].def==encode(defaultOf<bool>()));assert(data.BreakSiegeFinished==(mode==0?valueFor<bool>(101+pattern):mode==1?valueFor<bool>(272):defaultOf<bool>()));
assert(context.entries[72].key=="WhiteBalverineFinished");
assert(context.entries[72].address==&data.WhiteBalverineFinished);assert(context.entries[72].before==encode(valueFor<bool>(102+pattern)));assert(context.entries[72].def==encode(defaultOf<bool>()));assert(data.WhiteBalverineFinished==(mode==0?valueFor<bool>(102+pattern):mode==1?valueFor<bool>(273):defaultOf<bool>()));
assert(context.entries[73].key=="MadBomberFinished");
assert(context.entries[73].address==&data.MadBomberFinished);assert(context.entries[73].before==encode(valueFor<bool>(103+pattern)));assert(context.entries[73].def==encode(defaultOf<bool>()));assert(data.MadBomberFinished==(mode==0?valueFor<bool>(103+pattern):mode==1?valueFor<bool>(274):defaultOf<bool>()));
assert(context.entries[74].key=="BanditCampTwinbladeKilled");
assert(context.entries[74].address==&data.BanditCampTwinbladeKilled);assert(context.entries[74].before==encode(valueFor<bool>(27+pattern)));assert(context.entries[74].def==encode(defaultOf<bool>()));assert(data.BanditCampTwinbladeKilled==(mode==0?valueFor<bool>(27+pattern):mode==1?valueFor<bool>(275):defaultOf<bool>()));
assert(context.entries[75].key=="PrisonRaceNumber");
assert(context.entries[75].address==&data.PrisonRaceNumber);assert(context.entries[75].before==encode(valueFor<long>(107+pattern)));assert(context.entries[75].def==encode(defaultOf<long>()));assert(data.PrisonRaceNumber==(mode==0?valueFor<long>(107+pattern):mode==1?valueFor<long>(276):defaultOf<long>()));
assert(context.entries[76].key=="PrisonKeyStolenByHero");
assert(context.entries[76].address==&data.PrisonKeyStolenByHero);assert(context.entries[76].before==encode(valueFor<bool>(110+pattern)));assert(context.entries[76].def==encode(defaultOf<bool>()));assert(data.PrisonKeyStolenByHero==(mode==0?valueFor<bool>(110+pattern):mode==1?valueFor<bool>(277):defaultOf<bool>()));
assert(context.entries[77].key=="BooksPreviouslyOpened_Vector");
assert(context.entries[78].key=="MaxChickenKickingScore");
assert(context.entries[78].address==&data.MaxChickenKickingScore);assert(context.entries[78].before==encode(valueFor<long>(115+pattern)));assert(context.entries[78].def==encode(defaultOf<long>()));assert(data.MaxChickenKickingScore==(mode==0?valueFor<long>(115+pattern):mode==1?valueFor<long>(279):defaultOf<long>()));
assert(context.entries[79].key=="JackBossBattleHeroGoodAtEnd");
assert(context.entries[79].address==&data.JackBossBattleHeroGoodAtEnd);assert(context.entries[79].before==encode(valueFor<bool>(63+pattern)));assert(context.entries[79].def==encode(defaultOf<bool>()));assert(data.JackBossBattleHeroGoodAtEnd==(mode==0?valueFor<bool>(63+pattern):mode==1?valueFor<bool>(280):defaultOf<bool>()));
assert(context.entries[80].key=="JackBossBattleResult");
assert(context.entries[80].address==&data.JackBossBattleResult);assert(context.entries[80].before==encode(valueFor<long>(62+pattern)));assert(context.entries[80].def==encode(defaultOf<long>()));assert(data.JackBossBattleResult==(mode==0?valueFor<long>(62+pattern):mode==1?valueFor<long>(281):defaultOf<long>()));
assert(data.HenchmanCommentaryState==valueFor<long>(12+pattern));
assert(data.HenchmanOverrideState==valueFor<long>(13+pattern));
assert(data.HenchmanSacrificedBodge==valueFor<bool>(14+pattern));
assert(data.RandomEntityVillagerMax==valueFor<long>(16+pattern));
assert(data.RandomEntityGuardMax==valueFor<long>(17+pattern));
assert(data.RandomEntityBanditMax==valueFor<long>(18+pattern));
assert(data.RandomEntityVillagerCount==valueFor<long>(19+pattern));
assert(data.RandomEntityGuardCount==valueFor<long>(20+pattern));
assert(data.RandomEntityBanditCount==valueFor<long>(21+pattern));
assert(data.RandomEntityVillagerPercentage==valueFor<float>(22+pattern));
assert(data.RandomEntityGuardPercentage==valueFor<float>(23+pattern));
assert(data.RandomEntityBanditPercentage==valueFor<float>(24+pattern));
assert(data.RandomEntitySheriffAlive==valueFor<bool>(25+pattern));
assert(data.RandomEntityLieutenantAlive==valueFor<bool>(26+pattern));
assert(data.MotherDodgingKraken==valueFor<bool>(61+pattern));
assert(data.WifeLeavingYou==valueFor<bool>(64+pattern));
assert(data.WifeLeftYou==valueFor<bool>(65+pattern));
assert(data.CurrentScene==valueFor<long>(66+pattern));
assert(data.StartNextScene==valueFor<bool>(67+pattern));
assert(data.CurrentChapter==valueFor<long>(68+pattern));
assert(data.SkillScore==valueFor<long>(74+pattern));
assert(data.WillScore==valueFor<long>(76+pattern));
assert(data.HeroTakingGuildTest==valueFor<bool>(87+pattern));
assert(data.BodyGuardsMustStandAndWait==valueFor<bool>(104+pattern));
assert(data.BodyGuardsInLimbo==valueFor<bool>(105+pattern));
assert(data.WifeHeroFlirting==valueFor<bool>(106+pattern));
assert(data.PrisonRaceWonByHero==valueFor<bool>(108+pattern));
assert(data.WasPrisonRaceEverWonByHero==valueFor<bool>(109+pattern));
for(int i=0;i<3;++i)assert(data.BooksPreviouslyOpened[i]==((mask&(1<<i))!=0));
assert(data.PrisonTorturedBefore==valueFor<bool>(112+pattern));
assert(data.PrisonGameDoneBefore==valueFor<bool>(113+pattern));
assert(data.HeroDollsScriptUsingTeacher==valueFor<bool>(114+pattern));
assert(data.GHeroName==valueFor<EHeroNames>(118+pattern));
assert(data.OFBRCratesStolen==valueFor<bool>(120+pattern));
assert(data.OFBR_NoCratesWereStolen==valueFor<bool>(121+pattern));
assert(data.DarkwoodAllTradersAlive==valueFor<bool>(122+pattern));
assert(data.MadBomberNoBombsExplode==valueFor<bool>(123+pattern));
assert(data.HobbeCavePerpertratorKilled==valueFor<bool>(124+pattern));
assert(data.HobbeCaveInnocentSacrificed==valueFor<long>(125+pattern));
assert(data.HobbeCaveBoyUnharmed==valueFor<bool>(126+pattern));
assert(data.BanditCampKillNoBandits==valueFor<bool>(127+pattern));
assert(data.BreakSiegeNoAssistance==valueFor<bool>(128+pattern));
assert(data.BreakSiegeKilledLeader==valueFor<bool>(129+pattern));
assert(data.OrchardFarmBanditKilled==valueFor<bool>(130+pattern));
assert(data.OrchardFarmGuardKilled==valueFor<bool>(131+pattern));
assert(data.BanditCampKillManyBandits==valueFor<bool>(132+pattern));
assert(data.HobbeContestKillMoreHobbes==valueFor<bool>(133+pattern));
assert(data.GraveyardKillAllInnocents==valueFor<bool>(134+pattern));
assert(data.DarkwoodPickpocketedAllTraders==valueFor<long>(135+pattern));
assert(data.TCGKillNoBandits==valueFor<bool>(138+pattern));
assert(data.TCGMadeTimeLimit==valueFor<bool>(139+pattern));
assert(data.TCEKeepBanditFollowerAlive==valueFor<bool>(140+pattern));
assert(data.TCEMadeTimeLimit==valueFor<bool>(141+pattern));
assert(data.RansomVictimVictimNoDamage==valueFor<bool>(142+pattern));
assert(data.RansomVictimKidnappersKilled==valueFor<bool>(143+pattern));
assert(data.RansomVictimSaveVictim==valueFor<bool>(144+pattern));
assert(data.RansomVictimHaveVictimKilled==valueFor<bool>(145+pattern));
assert(data.MinionCampBriarNoDamage==valueFor<bool>(146+pattern));
assert(data.FireHeartFreeAllPrisoners==valueFor<bool>(147+pattern));
assert(data.FireHeartKillAllPrisoners==valueFor<bool>(148+pattern));
assert(data.TCETimeLimitBoastTaken==valueFor<bool>(149+pattern));
assert(data.TCGTimeLimitBoastTaken==valueFor<bool>(150+pattern));
assert(data.HeroWoreMask==valueFor<bool>(151+pattern));
assert(data.BoastRegions.size()==1&&data.BoastRegions[0].value=="untouched");assert(context.defaults==43);for(int i=0;i<3;++i){assert(NPCAttitudes[i]==edges[(edge+i)%4]&&context.originalAttitudes[i]==NPCAttitudes[i]);assert(context.originalBooks[i]==((mask&(1<<i))!=0));}++cases;}assert(strings==0);}assert(cases==192);std::puts("00cdc070_TEST PASS");}