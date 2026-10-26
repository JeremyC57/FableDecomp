// CTCHeroStats::CTCHeroStats(CThing*) -- the hero-stats component: CTCBase, zeroed counters and records, default multipliers and recovery marker, sized experience/region/ability tables, random wobble counters and the def-derived age, stamina, fatness timer and renown cap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0057CDB5, 1007 bytes, `ret 4`; frame-pointer size TU, no EH frame.
// Callees: CTCBase::CTCBase(CThing*) 0x00686800 (manifest "CEngineSubPrimitive2DClockSprite" propagated), the crime map ctor
//   0x00581114 (manifest "CopyBackBufferToTexture"), CCharString copy ctor 0x0099EC30 (global 0x013B8B0C), the primitive-handle
//   base ctor 0x0099A2D0 (manifest "NHeroInformationScreens::CBase"), list<CHeroMarriageInfo> ctor 0x0057FDA2, vector<long>::resize
//   0x004428F0, CTCBase::GetWorldMap 0x00686D30 / GetWorld 0x00686A70 / GetDefinitions 0x006869D0, CWorldMap::GetNoMaps 0x004FB470,
//   CWorld random seed 0x0049D810 (manifest "DoSizeof"), GFFloatRandom 0x004978A0, GetFatnessReducingFrequencyInFrames 0x0057A60A,
//   the marriage-list reset 0x0057F58E (descriptive; manifest "SortTreeRecursively").
// Layout: PDB CTCHeroStats with STLport map/vectors at 12 bytes: map +0x0c, ExperienceSpentOn +0x18, NumberOfDecapitations..
//   TrainingStatus at PDB-8, the three tables +0x118/+0x124/+0x130, then PDB-0x14 to HousePositions, PDB-0x18 from InfiniteStamina
//   (+0x16c) to Marriages (+0x1d8). Age, FramesUntilFatnessReduces, RenownMaxLevel, DrinkAffectMultiplierMultiplier and the three
//   counters are not in the initialiser list.
// Shape: declaration-order initialiser list; the RegionTime resize passes the literal 0 so the `const long&` temporary (in the dead
//   owner slot) is pushed before the map-count calls; the handle's derived ctor is force-inlined after its out-of-line base.
#pragma optimize("y", off)
#pragma optimize("s", on)

class CThing;
class CCharStringData;
class CCharString { public:
    CCharStringData* PStringData;
    CCharString(const CCharString& other);           // 0x0099EC30
    ~CCharString();
};
extern CCharString g_HeroGuildRecoveryMarker;        // 0x013B8B0C ("HERO_GUILD_RECOVERY_MARKER")

struct CCrimeMap {                                   // map<CCharString, MirroredCriminalStatus> (retail 12 bytes)
    void* _M_header[3];
    CCrimeMap();                                     // 0x00581114
};
struct CLongVector {                                 // STLport vector<long> (retail 12 bytes)
    long* _M_start;
    long* _M_finish;
    long* _M_end_of_storage;
    CLongVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    void resize(unsigned long size, const long& value);   // 0x004428F0
};
struct SHousePosition;
struct CHousePositionVector {
    SHousePosition* _M_start;
    SHousePosition* _M_finish;
    SHousePosition* _M_end_of_storage;
    CHousePositionVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
};
class CSpecialAbilitiesDrunkennessDef;
struct CDrunkennessDefPointer {
    const CSpecialAbilitiesDrunkennessDef* Object;
    CDrunkennessDefPointer() : Object(0) {}
};
struct CPrimitiveHandleBase {
    virtual void h00();
    CPrimitiveHandleBase();                          // 0x0099A2D0
};
class CEnginePrimitiveHandle : public CPrimitiveHandleBase { public:   // vtable 0x0122F598
    long Handle;                                     // +0x04
    __forceinline CEnginePrimitiveHandle() : Handle(0) {}
    virtual void h00();
};
struct CMarriageAllocator { CMarriageAllocator() {} };
struct CMarriageList {                               // STLport list<CHeroMarriageInfo>
    void* _M_node;
    CMarriageList(const CMarriageAllocator& allocator = CMarriageAllocator());   // 0x0057FDA2
    void Reset();                                    // 0x0057F58E (descriptive)
};

struct CRenownLevelVector { long* _M_start; long* _M_finish; long* _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; } };
class CHeroStatsDef { public:
    unsigned char      _pad00[0x3c];
    long               MaxStamina;                   // +0x3c
    unsigned char      _pad40[0x68 - 0x40];
    float              StartAge;                     // +0x68
    unsigned char      _pad6c[0xac - 0x6c];
    CRenownLevelVector RenownLevels;                 // +0xac
};
class CGameDefinitionManager { public:
    unsigned char  _pad_0x00[0xdc];
    CHeroStatsDef* PHeroStatsDef;                    // +0xdc
};
class CWorld { public: unsigned long* GetRandomSeed();     // 0x0049D810
};
class CWorldMap { public: long GetNoMaps() const;           // 0x004FB470
};
float __fastcall GFFloatRandom(float range, unsigned long* seed);   // 0x004978A0

class CTCBase { public:
    virtual void v00();
    CThing*       ParentThing;                       // +0x04
    unsigned char _pad08[4];
    CTCBase(CThing* owner);                          // 0x00686800
protected:
    const CGameDefinitionManager& GetDefinitionManager() const;   // 0x006869D0
    CWorld& GetWorld() const;   // 0x00686A70
    CWorldMap& GetWorldMap();   // 0x00686D30
};

class CTCHeroStats : public CTCBase { public:
    CCrimeMap                OutstandingCrimes;             // +0xc
    CLongVector              ExperienceSpentOn;             // +0x18
    long                     NumberOfDecapitations;
    long                     Morality;
    bool                     MoralityChangingEnabled;       // +0x2c
    float                    Age;                           // +0x30
    float                    SunTan;
    float                    Fatness;
    long                     Money;
    long                     MaxMoney;
    long                     ConfiscatedMoney;
    long                     TotalMoneyEarned;
    long                     TotalMoneySpent;
    long                     RewardMoney;
    long                     RentMoney;
    long                     Stamina;
    long                     MaxStamina;
    long                     FramesUntilStaminaRefills;
    long                     FramesUntilHealthRefills;
    bool                     ScriptOverridePreventStaminaRefill;// +0x68
    long                     FramesUntilFatnessReduces;     // +0x6c
    long                     RenownLevel;
    long                     RenownMaxLevel;                // +0x74
    long                     RenownPointsInLevel;
    long                     RenownTotal;
    float                    Agreeableness;
    float                    InherentAttractiveness;
    float                    InherentScariness;
    float                    ExtraMaxHealth;
    long                     ExtraMaxStamina;               // +0x90
    long                     FramesLeftForSpeedMultiplier;
    long                     MaximumMultiplier;
    float                    Attractiveness;
    float                    Scariness;
    float                    AppearanceGoodstrength;
    float                    SoundRadiusMultiplier;
    float                    VisibilityMultiplier;
    long                     FrameUpdatedClothingStats;
    long                     FrameUpdatedTattooStats;
    long                     NumFailedAQuest;
    long                     NumCompletedAQuest;
    long                     NumCompletedACoreQuest;
    long                     NumCompletedAFeatQuest;
    long                     NumCompletedAnOptionalQuest;
    long                     NumBoastsTaken;
    long                     NumBoastsCompleted;
    long                     NumMountedTrophies;
    long                     NumTrophyGamePlayed;
    long                     BestTrophyWitnesses;
    long                     NumberOfShopsOwned;
    long                     NumberOfHousesOwned;
    long                     NumberOfBuildingsRented;
    long                     NumberOfVomits;
    long                     NumberOfDrinks;
    long                     NumberOfCrimes;
    long                     NumberOfCrimesOfMurder;
    long                     TotalFine;
    long                     FinePaid;
    long                     TimesThrownOut;
    long                     TotalBribeAmount;
    long                     NumTeleports;
    float                    MaxChickenThrow;               // +0x110
    long                     TrainingStatus;                // +0x114
    CLongVector              HeroStatsExperience;           // +0x118
    CLongVector              RegionTime;                    // +0x124
    CLongVector              NumberOfTimesUsedAbility;      // +0x130
    long                     TotalSpentExperience;
    long                     NumLoversAcquired;
    long                     NumWeddings;
    long                     NumSpousesMurdered;
    long                     NumSpousesDied;
    long                     NumDivorces;
    long                     NumSex;
    bool                     HadScriptedSex;                // +0x158
    bool                     HadScriptedGaySex;
    float                    BiggestFish;                   // +0x15c
    CHousePositionVector     HousePositions;                // +0x160
    bool                     InfiniteStamina;               // +0x16c
    CCharString              DeathRecoveryMarkerName;       // +0x170
    bool                     FailQuestOnDeath;              // +0x174
    float                    BestTimePairs;
    float                    BestTimeSorting;
    float                    BestScoreBlackjack;
    float                    BestScoreCoinGolfOakVale;
    float                    BestScoreCoinGolfSnowSpire;
    float                    BestScoreShoveHaPenny;
    float                    BestTimeGuessTheAddition;
    bool                     PrizeGivenPairs;
    bool                     PrizeGivenSorting;
    bool                     PrizeGivenBlackjack;
    bool                     PrizeGivenCoinGolfOakVale;
    bool                     PrizeGivenCoinGolfSnowSpire;
    bool                     PrizeGivenShoveHaPenny;
    bool                     PrizeGivenGuessTheAddition;
    float                    Drunkenness;                   // +0x19c
    float                    BeerReservoir;
    float                    DrinkAffectMultiplier;
    float                    DrinkAffectMultiplierMultiplier;
    long                     FrameToVomit;                  // +0x1ac
    long                     DeedHandle;
    long                     FrameEndVomit;
    bool                     UpdateDrunkenness;             // +0x1b8
    CDrunkennessDefPointer   PDrunkennessDef;               // +0x1bc
    CEnginePrimitiveHandle   PrimitiveHandle;               // +0x1c0
    float                    UCounter;                      // +0x1c8
    float                    VCounter;
    float                    RotCounter;
    bool                     IsInCutSceneFirstTime;         // +0x1d4
    CMarriageList            Marriages;                     // +0x1d8
    CTCHeroStats(CThing* owner);
    virtual void v00();
    const CHeroStatsDef* GetHeroStatsDef() const { return GetDefinitionManager().PHeroStatsDef; }
    long GetFatnessReducingFrequencyInFrames() const;   // 0x0057A60A
};

CTCHeroStats::CTCHeroStats(CThing* owner)
    : CTCBase(owner), OutstandingCrimes(), ExperienceSpentOn(), NumberOfDecapitations(0), Morality(0), MoralityChangingEnabled(true),
      SunTan(0.5f), Fatness(0.0f), Money(0), MaxMoney(0), ConfiscatedMoney(0), TotalMoneyEarned(0),
      TotalMoneySpent(0), RewardMoney(0), RentMoney(0), Stamina(0), MaxStamina(0), FramesUntilStaminaRefills(0),
      FramesUntilHealthRefills(0), ScriptOverridePreventStaminaRefill(false), RenownLevel(0), RenownPointsInLevel(0),
      RenownTotal(0), Agreeableness(0.0f), InherentAttractiveness(0.0f), InherentScariness(0.0f),
      ExtraMaxHealth(0.0f), ExtraMaxStamina(0), FramesLeftForSpeedMultiplier(0), MaximumMultiplier(0),
      Attractiveness(0.0f), Scariness(0.0f), AppearanceGoodstrength(0.0f), SoundRadiusMultiplier(1.0f),
      VisibilityMultiplier(1.0f), FrameUpdatedClothingStats(0), FrameUpdatedTattooStats(0), NumFailedAQuest(0),
      NumCompletedAQuest(0), NumCompletedACoreQuest(0), NumCompletedAFeatQuest(0), NumCompletedAnOptionalQuest(0),
      NumBoastsTaken(0), NumBoastsCompleted(0), NumMountedTrophies(0), NumTrophyGamePlayed(0),
      BestTrophyWitnesses(0), NumberOfShopsOwned(0), NumberOfHousesOwned(0), NumberOfBuildingsRented(0),
      NumberOfVomits(0), NumberOfDrinks(0), NumberOfCrimes(0), NumberOfCrimesOfMurder(0), TotalFine(0), FinePaid(0),
      TimesThrownOut(0), TotalBribeAmount(0), NumTeleports(0), MaxChickenThrow(0.0f), TrainingStatus(0),
      HeroStatsExperience(), RegionTime(), NumberOfTimesUsedAbility(), TotalSpentExperience(0), NumLoversAcquired(0),
      NumWeddings(0), NumSpousesMurdered(0), NumSpousesDied(0), NumDivorces(0), NumSex(0), HadScriptedSex(false),
      HadScriptedGaySex(false), BiggestFish(0.0f), HousePositions(), InfiniteStamina(false),
      DeathRecoveryMarkerName(g_HeroGuildRecoveryMarker), FailQuestOnDeath(true), BestTimePairs(0.0f),
      BestTimeSorting(0.0f), BestScoreBlackjack(0.0f), BestScoreCoinGolfOakVale(0.0f),
      BestScoreCoinGolfSnowSpire(0.0f), BestScoreShoveHaPenny(0.0f), BestTimeGuessTheAddition(0.0f),
      PrizeGivenPairs(false), PrizeGivenSorting(false), PrizeGivenBlackjack(false), PrizeGivenCoinGolfOakVale(false),
      PrizeGivenCoinGolfSnowSpire(false), PrizeGivenShoveHaPenny(false), PrizeGivenGuessTheAddition(false),
      Drunkenness(0.0f), BeerReservoir(0.0f), DrinkAffectMultiplier(1.0f), FrameToVomit(-1), DeedHandle(-1),
      FrameEndVomit(0), UpdateDrunkenness(true), PDrunkennessDef(), PrimitiveHandle(), IsInCutSceneFirstTime(true),
      Marriages()
{
    ExperienceSpentOn.resize(11, 0);
    NumberOfTimesUsedAbility.resize(20, 0);
    RegionTime.resize(GetWorldMap().GetNoMaps(), 0);
    HeroStatsExperience.resize(4, 0);
    UCounter = GFFloatRandom(1.0f, GetWorld().GetRandomSeed());
    VCounter = GFFloatRandom(1.0f, GetWorld().GetRandomSeed());
    RotCounter = GFFloatRandom(1.0f, GetWorld().GetRandomSeed());
    FramesUntilFatnessReduces = GetFatnessReducingFrequencyInFrames();
    Age = GetHeroStatsDef()->StartAge;
    Stamina = MaxStamina = GetHeroStatsDef()->MaxStamina + ExtraMaxStamina;
    RenownMaxLevel = GetHeroStatsDef()->RenownLevels.size();
    Marriages.Reset();
}