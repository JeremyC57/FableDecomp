// NScript::CQ_SunnyvaleMasterData::Init() -- reset the Sunnyvale master quest data: the relation suffix strings and counters, the teddy solution, and every quest flag/score to its starting value
// Leaf: Init
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CDBA10, 569 bytes (576 with int3 padding; vtable reference 0x012C2750).
// Callees: CCharString::operator=(const char*) 0x0099EFE0 (x4). Statics: three longs at 0x0143E928 and three
//   CCharStrings at 0x0143E934 (unnamed in the PDB dumps; CQ_SunnyvaleMasterData's transfer at 0x00CDC890 walks the
//   longs as one array, 0x00E7D8EF indexes the strings by a counter).
// Data: "" 0x0122D70E, "_LIKE" 0x012C275C, "_HATE" 0x012C2754, "A" 0x0122DCE0.
// Layout: Ego_r.pdb's CQ_SunnyvaleMasterData at the PDB offsets up to +0x134 (the landed quest scripts read
//   TCGMadeTimeLimit +0x127, RansomVictimVictimNoDamage +0x12A there); the BoastRegions vector is the 12-byte retail
//   STLport vector, so SingingStonesInSync sits at +0x140 (PDB +0x144).
// Shape: one zero promoted into ebx serves every long/bool/float reset; the two archery states share one `2` through a
//   chained assignment (Required = Current = 2 stores Current first); BooksPreviouslyOpened[3] is cleared by a
//   3-step loop, which VC turns into a word + byte store from a fresh xor eax (an explicit memset adds `lea ecx`).
struct CCharString {
    void* Data;
    CCharString& operator=(const char* text);           // 0x0099EFE0
};
namespace NScript {
struct CQ_SunnyvaleMasterData {
    void*         _vfptr;                            // +0x00 CScriptDataBase vptr
    long          PostSavePosition;                  // +0x04 EGameflowPosition
    long          HenchmanCommentaryState;           // +0x08
    long          HenchmanOverrideState;             // +0x0C
    bool          HenchmanSacrificedBodge;           // +0x10
    bool          HauntedBarrowFieldsCompleted;      // +0x11
    long          RandomEntityVillagerMax;           // +0x14
    long          RandomEntityGuardMax;              // +0x18
    long          RandomEntityBanditMax;             // +0x1C
    long          RandomEntityVillagerCount;         // +0x20
    long          RandomEntityGuardCount;            // +0x24
    long          RandomEntityBanditCount;           // +0x28
    float         RandomEntityVillagerPercentage;    // +0x2C
    float         RandomEntityGuardPercentage;       // +0x30
    float         RandomEntityBanditPercentage;      // +0x34
    bool          RandomEntitySheriffAlive;          // +0x38
    bool          RandomEntityLieutenantAlive;       // +0x39
    bool          BanditCampTwinbladeKilled;         // +0x3A
    long          ArcheryStateCurrent;               // +0x3C ArcheryOpenState
    long          ArcheryStateRequired;              // +0x40 ArcheryOpenState
    long          ArcheryHighScore;                  // +0x44
    bool          FriendOfForeman;                   // +0x48
    bool          BridgeOpened;                      // +0x49
    bool          GrannyMemoryReturned;              // +0x4A
    bool          IsLunaHuman;                       // +0x4B
    bool          CondemnedManDead;                  // +0x4C
    bool          CondemnedManForgiven;              // +0x4D
    bool          CondemnedManMeetsBodyGuard;        // +0x4E
    bool          CondemnedManMeetsBodyGuardCutSceneStart; // +0x4F
    bool          CondemnedManMeetsBodyGuardCutSceneFinished; // +0x50
    CCharString   TeddySolution;                     // +0x54
    long          OrchardFarmRaidLastCompleted;      // +0x58
    long          OrchardFarmTraderEscortCounter;    // +0x5C
    bool          SeenAbbeyMotherAtGuild;            // +0x60
    bool          DefeatedThunder;                   // +0x61
    bool          LostToThunder;                     // +0x62
    bool          KilledThunder;                     // +0x63
    bool          CollectedSoulFromArena;            // +0x64
    bool          KilledBriar;                       // +0x65
    bool          CollectedSoulFromMother;           // +0x66
    bool          KilledGM;                          // +0x67
    bool          CollectedSoulFromNostro;           // +0x68
    long          DeliveredSoul;                     // +0x6C
    long          CurrentHeroSoulsPosition;          // +0x70 EPlaceInHeroSouls
    bool          WhisperKilledByHero;               // +0x74
    bool          ArenaFinished;                     // +0x75
    bool          GatesRequireClosing;               // +0x76
    bool          GatesRequireOpening;               // +0x77
    long          HeroDrunkness;                     // +0x78
    bool          TrophyDealerHeroSpokenToDemonDoors; // +0x7C
    float         VillagerAngryRating;               // +0x80
    bool          MotherDodgingKraken;               // +0x84
    long          JackBossBattleResult;              // +0x88
    bool          JackBossBattleHeroGoodAtEnd;       // +0x8C
    bool          WifeLeavingYou;                    // +0x8D
    bool          WifeLeftYou;                       // +0x8E
    long          CurrentScene;                      // +0x90
    bool          StartNextScene;                    // +0x94
    long          CurrentChapter;                    // +0x98
    bool          ScorpionsDestroyed;                // +0x9C
    bool          ScorpionsDestroyedCutscenePlayed;  // +0x9D
    bool          SkillTrainingStarted;              // +0x9E
    bool          WillTrainingStarted;               // +0x9F
    bool          MovingDummiesNeeded;               // +0xA0
    long          SkillScore;                        // +0xA4
    long          HighestSkillScore;                 // +0xA8
    long          WillScore;                         // +0xAC
    bool          MeleeApprenticeNeededForCutscene;  // +0xB0
    long          GlobalMeleeGrade;                  // +0xB4
    long          GlobalSkillGrade;                  // +0xB8
    long          GlobalWillGrade;                   // +0xBC
    bool          GuildWarningOccuring;              // +0xC0
    bool          SkillTestOccuring;                 // +0xC1
    bool          WillTestOccuring;                  // +0xC2
    bool          SkillRepeating;                    // +0xC3
    bool          SkillRepeatKnown;                  // +0xC4
    bool          SkillDummyReset;                   // +0xC5
    bool          HeroTakingGuildTest;               // +0xC6
    bool          SwordInTheStoneComplete;           // +0xC7
    bool          AmbushTradersAllGuardsDead;        // +0xC8
    bool          AmbushTradersAllTradersDead;       // +0xC9
    long          AmbushTradersKillCount;            // +0xCC
    long          AmbushTradersBanditHireCount;      // +0xD0
    bool          AmbushTradersSpyDead;              // +0xD4
    bool          StruckDealWithLadyGrey;            // +0xD5
    bool          HeroExposedLadyGrey;               // +0xD6
    bool          HeroMarriedLadyGrey;               // +0xD7
    bool          BountyHuntWithinTimeLimit;         // +0xD8
    bool          BountyHuntDecapitation;            // +0xD9
    bool          BountyHuntTimeLimitExceeded;       // +0xDA
    unsigned long AchievementsWorthyOfSong;          // +0xDC ulong
    bool          BreakSiegeFinished;                // +0xE0
    bool          WhiteBalverineFinished;            // +0xE1
    bool          MadBomberFinished;                 // +0xE2
    bool          BodyGuardsMustStandAndWait;        // +0xE3
    bool          BodyGuardsInLimbo;                 // +0xE4
    bool          WifeHeroFlirting;                  // +0xE5
    long          PrisonRaceNumber;                  // +0xE8
    bool          PrisonRaceWonByHero;               // +0xEC
    bool          WasPrisonRaceEverWonByHero;        // +0xED
    bool          PrisonKeyStolenByHero;             // +0xEE
    bool          BooksPreviouslyOpened[3];          // +0xEF
    bool          PrisonTorturedBefore;              // +0xF2
    bool          PrisonGameDoneBefore;              // +0xF3
    bool          HeroDollsScriptUsingTeacher;       // +0xF4
    long          MaxChickenKickingScore;            // +0xF8
    unsigned long StoryTellerSpecialStories;         // +0xFC ulong
    unsigned long StoryTellerToldSpecialStories;     // +0x100 ulong
    long          GHeroName;                         // +0x104 EHeroNames
    bool          TimeAdvancePointTriggered;         // +0x108
    bool          OFBRCratesStolen;                  // +0x109
    bool          OFBR_NoCratesWereStolen;           // +0x10A
    bool          DarkwoodAllTradersAlive;           // +0x10B
    bool          MadBomberNoBombsExplode;           // +0x10C
    bool          HobbeCavePerpertratorKilled;       // +0x10D
    long          HobbeCaveInnocentSacrificed;       // +0x110
    bool          HobbeCaveBoyUnharmed;              // +0x114
    bool          BanditCampKillNoBandits;           // +0x115
    bool          BreakSiegeNoAssistance;            // +0x116
    bool          BreakSiegeKilledLeader;            // +0x117
    bool          OrchardFarmBanditKilled;           // +0x118
    bool          OrchardFarmGuardKilled;            // +0x119
    bool          BanditCampKillManyBandits;         // +0x11A
    bool          HobbeContestKillMoreHobbes;        // +0x11B
    bool          GraveyardKillAllInnocents;         // +0x11C
    long          DarkwoodPickpocketedAllTraders;    // +0x120
    bool          HangingTreeBanditKilled;           // +0x124
    bool          HangingTreeGuardKilled;            // +0x125
    bool          TCGKillNoBandits;                  // +0x126
    bool          TCGMadeTimeLimit;                  // +0x127
    bool          TCEKeepBanditFollowerAlive;        // +0x128
    bool          TCEMadeTimeLimit;                  // +0x129
    bool          RansomVictimVictimNoDamage;        // +0x12A
    bool          RansomVictimKidnappersKilled;      // +0x12B
    bool          RansomVictimSaveVictim;            // +0x12C
    bool          RansomVictimHaveVictimKilled;      // +0x12D
    bool          MinionCampBriarNoDamage;           // +0x12E
    bool          FireHeartFreeAllPrisoners;         // +0x12F
    bool          FireHeartKillAllPrisoners;         // +0x130
    bool          TCETimeLimitBoastTaken;            // +0x131
    bool          TCGTimeLimitBoastTaken;            // +0x132
    bool          HeroWoreMask;                      // +0x133
    void*         BoastRegions[3];                   // +0x134 vector<CCharString> (retail STLport 12 bytes)
    bool          SingingStonesInSync;               // +0x140
    void Init();
    static long        RelationCounters[3];             // 0x0143E928 (unnamed statics, transferred together)
    static CCharString RelationSuffixes[3];             // 0x0143E934 ("", "_LIKE", "_HATE")
};

void CQ_SunnyvaleMasterData::Init()
{
    PostSavePosition = 0;
    HenchmanOverrideState = 0;
    HenchmanSacrificedBodge = false;
    RelationSuffixes[0] = "";
    RelationSuffixes[1] = "_LIKE";
    RelationSuffixes[2] = "_HATE";
    RelationCounters[0] = 0;
    RelationCounters[1] = 0;
    RelationCounters[2] = 0;
    HauntedBarrowFieldsCompleted = false;
    GrannyMemoryReturned = false;
    IsLunaHuman = false;
    FriendOfForeman = false;
    BridgeOpened = false;
    RandomEntityVillagerMax = 0;
    RandomEntityGuardMax = 0;
    RandomEntityBanditMax = 0;
    RandomEntityVillagerCount = 0;
    RandomEntityGuardCount = 0;
    RandomEntityBanditCount = 0;
    RandomEntityVillagerPercentage = 0;
    RandomEntityGuardPercentage = 0;
    RandomEntityBanditPercentage = 0;
    RandomEntitySheriffAlive = false;
    RandomEntityLieutenantAlive = false;
    TrophyDealerHeroSpokenToDemonDoors = false;
    CondemnedManDead = false;
    CondemnedManForgiven = false;
    CondemnedManMeetsBodyGuard = false;
    CondemnedManMeetsBodyGuardCutSceneStart = false;
    CondemnedManMeetsBodyGuardCutSceneFinished = false;
    TeddySolution = "A";
    OrchardFarmRaidLastCompleted = 0;
    OrchardFarmTraderEscortCounter = 0;
    SeenAbbeyMotherAtGuild = false;
    DefeatedThunder = false;
    LostToThunder = false;
    KilledThunder = false;
    CollectedSoulFromArena = false;
    KilledBriar = false;
    CollectedSoulFromMother = false;
    KilledGM = false;
    CollectedSoulFromNostro = false;
    DeliveredSoul = 0;
    CurrentHeroSoulsPosition = 0;
    WhisperKilledByHero = false;
    ArenaFinished = false;
    HangingTreeBanditKilled = false;
    HangingTreeGuardKilled = false;
    StruckDealWithLadyGrey = false;
    HeroExposedLadyGrey = false;
    HeroMarriedLadyGrey = false;
    GatesRequireClosing = false;
    GatesRequireOpening = false;
    AchievementsWorthyOfSong = 0;
    StoryTellerSpecialStories = 0;
    StoryTellerToldSpecialStories = 0;
    HeroDrunkness = 0;
    VillagerAngryRating = 0;
    GHeroName = 0;
    TimeAdvancePointTriggered = false;
    ScorpionsDestroyed = false;
    ScorpionsDestroyedCutscenePlayed = false;
    SkillTrainingStarted = false;
    WillTrainingStarted = false;
    MovingDummiesNeeded = false;
    MeleeApprenticeNeededForCutscene = false;
    GlobalMeleeGrade = 0;
    GlobalSkillGrade = 0;
    GlobalWillGrade = 0;
    GuildWarningOccuring = false;
    SingingStonesInSync = false;
    SkillTestOccuring = false;
    WillTestOccuring = false;
    SkillRepeatKnown = false;
    SkillDummyReset = false;
    HeroTakingGuildTest = false;
    SwordInTheStoneComplete = false;
    AmbushTradersAllGuardsDead = false;
    AmbushTradersAllTradersDead = false;
    AmbushTradersKillCount = 0;
    AmbushTradersBanditHireCount = 0;
    AmbushTradersSpyDead = false;
    BountyHuntWithinTimeLimit = false;
    BountyHuntTimeLimitExceeded = false;
    BountyHuntDecapitation = false;
    TCETimeLimitBoastTaken = false;
    TCGTimeLimitBoastTaken = false;
    ArcheryHighScore = 0;
    BreakSiegeFinished = false;
    WhiteBalverineFinished = false;
    MadBomberFinished = false;
    BodyGuardsMustStandAndWait = false;
    BodyGuardsInLimbo = false;
    PrisonRaceNumber = 0;
    PrisonRaceWonByHero = false;
    WasPrisonRaceEverWonByHero = false;
    PrisonKeyStolenByHero = false;
    HighestSkillScore = 10;
    SkillRepeating = true;
    ArcheryStateRequired = ArcheryStateCurrent = 2;
    for (int i = 0; i < 3; ++i)
        BooksPreviouslyOpened[i] = false;
    BanditCampTwinbladeKilled = false;
    JackBossBattleResult = 0;
    JackBossBattleHeroGoodAtEnd = false;
    MaxChickenKickingScore = 0;
    HeroDollsScriptUsingTeacher = false;
}
}