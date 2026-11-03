// Standalone C++03 behavior test; reconstructed constructor and function bodies appear unchanged.
#include <cassert>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <algorithm>
#include <iterator>
#include <new>
#include <cstddef>
typedef std::string CCharString;
enum EHeroTrainingStatus {
    TRAINING_STATUS_GRADUATED = 0,
    TRAINING_STATUS_APPRENTICE = 1,
    TRAINING_STATUS_BOY = 2
};

class CHeroMarriageInfo { public:
#if defined(_MSC_VER)
 unsigned __int64 WifeUID,MaritalHomeUID;
#else
 unsigned long WifeUID,MaritalHomeUID;
#endif
 int State;bool GayMarriage;
};
class CHeroStatsDef {public:long MaximumStamina;};
class CGameDefinitionManager {public:unsigned char Storage[240];};
static CGameDefinitionManager* DefinitionManager;
class CWorld {public:static long Frame;static long Calls;static long GetFrame(){++Calls;return Frame;}};
long CWorld::Frame=100;long CWorld::Calls=0;
static CWorld World;
class CPersistContext {public:
 bool Saving;std::vector<std::string> Trace;
 std::map<std::string,long> Integers;std::map<std::string,float> Floats;std::map<std::string,bool> Booleans;std::map<std::string,CCharString> Strings;
 std::map<std::string,std::vector<long> > LongVectors;std::map<std::string,std::vector<float> > FloatVectors;
 std::map<std::string,std::vector<CCharString> > StringVectors;std::map<std::string,std::vector<CHeroMarriageInfo> > MarriageVectors;
 explicit CPersistContext(bool saving):Saving(saving){}
 bool IsSaving()const{return Saving;}
 template<class T>void Value(const char*key,T&value,std::map<std::string,T>&data){Trace.push_back(key);if(Saving)data[key]=value;else value=data[key];}
 void Transfer(const char*k,long&v){Value(k,v,Integers);}
 void Transfer(const char*k,float&v){Value(k,v,Floats);}
 void Transfer(const char*k,bool&v){Value(k,v,Booleans);}
 void Transfer(const char*k,CCharString&v){Value(k,v,Strings);}
 void Transfer(const char*k,EHeroTrainingStatus&v){long n=(long)v;Value(k,n,Integers);v=(EHeroTrainingStatus)n;}
 template<class T>void Transfer(const char*k,T&v,const T&fallback){if(!Saving){v=fallback;Trace.push_back(k);}else Transfer(k,v);}
 void TransferVector(const char*k,std::vector<long>&v){Value(k,v,LongVectors);}
 void TransferVector(const char*k,std::vector<float>&v){Value(k,v,FloatVectors);}
 void TransferVector(const char*k,std::vector<CCharString>&v){Value(k,v,StringVectors);}
 void TransferVector(const char*k,std::vector<CHeroMarriageInfo>&v){Value(k,v,MarriageVectors);}
};
class CTCHeroStats {public:
 struct MirroredCriminalStatus {
  long Crimes[10];long LastTimeSeenByGuard,LastTimeExited;bool ExitedUsingTeleporter;
  float EntrancePosX,EntrancePosY,EntrancePosZ,EntranceAngle;
  MirroredCriminalStatus();
 };
    std::map<CCharString,MirroredCriminalStatus> OutstandingCrimes;                 // +0x0c map<CCharString,CTCHeroStats::MirroredCriminalStatus,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CTCHeroStats::MirroredCriminalStatus>_>_>
    std::vector<long> ExperienceSpentOn;                 // +0x18 vector<long,std::allocator<long>_>
    long             NumberOfDecapitations;                  // +0x24
    long             Morality;                               // +0x28
    bool             MoralityChangingEnabled;                // +0x2c
    float            Age;                                    // +0x30
    float            SunTan;                                 // +0x34
    float            Fatness;                                // +0x38
    long             Money;                                  // +0x3c
    long             MaxMoney;                               // +0x40
    long             ConfiscatedMoney;                       // +0x44
    long             TotalMoneyEarned;                       // +0x48
    long             TotalMoneySpent;                        // +0x4c
    long             RewardMoney;                            // +0x50
    long             RentMoney;                              // +0x54
    long             Stamina;                                // +0x58
    long             MaxStamina;                             // +0x5c
    long             FramesUntilStaminaRefills;              // +0x60
    long             FramesUntilHealthRefills;               // +0x64
    bool             ScriptOverridePreventStaminaRefill;     // +0x68
    long             FramesUntilFatnessReduces;              // +0x6c
    long             RenownLevel;                            // +0x70
    long             RenownMaxLevel;                         // +0x74
    long             RenownPointsInLevel;                    // +0x78
    long             RenownTotal;                            // +0x7c
    float            Agreeableness;                          // +0x80
    float            InherentAttractiveness;                 // +0x84
    float            InherentScariness;                      // +0x88
    float            ExtraMaxHealth;                         // +0x8c
    long             ExtraMaxStamina;                        // +0x90
    long             FramesLeftForSpeedMultiplier;           // +0x94
    long             MaximumMultiplier;                      // +0x98
    float            Attractiveness;                         // +0x9c
    float            Scariness;                              // +0xa0
    float            AppearanceGoodstrength;                 // +0xa4
    float            SoundRadiusMultiplier;                  // +0xa8
    float            VisibilityMultiplier;                   // +0xac
    long             FrameUpdatedClothingStats;              // +0xb0
    long             FrameUpdatedTattooStats;                // +0xb4
    long             NumFailedAQuest;                        // +0xb8
    long             NumCompletedAQuest;                     // +0xbc
    long             NumCompletedACoreQuest;                 // +0xc0
    long             NumCompletedAFeatQuest;                 // +0xc4
    long             NumCompletedAnOptionalQuest;            // +0xc8
    long             NumBoastsTaken;                         // +0xcc
    long             NumBoastsCompleted;                     // +0xd0
    long             NumMountedTrophies;                     // +0xd4
    long             NumTrophyGamePlayed;                    // +0xd8
    long             BestTrophyWitnesses;                    // +0xdc
    long             NumberOfShopsOwned;                     // +0xe0
    long             NumberOfHousesOwned;                    // +0xe4
    long             NumberOfBuildingsRented;                // +0xe8
    long             NumberOfVomits;                         // +0xec
    long             NumberOfDrinks;                         // +0xf0
    long             NumberOfCrimes;                         // +0xf4
    long             NumberOfCrimesOfMurder;                 // +0xf8
    long             TotalFine;                              // +0xfc
    long             FinePaid;                               // +0x100
    long             TimesThrownOut;                         // +0x104
    long             TotalBribeAmount;                       // +0x108
    long             NumTeleports;                           // +0x10c
    float            MaxChickenThrow;                        // +0x110
    EHeroTrainingStatus TrainingStatus;                    // +0x114 EHeroTrainingStatus
    std::vector<long> HeroStatsExperience;               // +0x118 vector<long,std::allocator<long>_>
    std::vector<long> RegionTime;                        // +0x124 vector<long,std::allocator<long>_>
    std::vector<long> NumberOfTimesUsedAbility;          // +0x130 vector<long,std::allocator<long>_>
    long             TotalSpentExperience;                   // +0x13c
    long             NumLoversAcquired;                      // +0x140
    long             NumWeddings;                            // +0x144
    long             NumSpousesMurdered;                     // +0x148
    long             NumSpousesDied;                         // +0x14c
    long             NumDivorces;                            // +0x150
    long             NumSex;                                 // +0x154
    bool             HadScriptedSex;                         // +0x158
    bool             HadScriptedGaySex;                      // +0x159
    float            BiggestFish;                            // +0x15c
    bool             InfiniteStamina;                        // +0x16c
    CCharString DeathRecoveryMarkerName;    // +0x170 CCharString::PStringData
    bool             FailQuestOnDeath;                       // +0x174
    float            BestTimePairs;                          // +0x178
    float            BestTimeSorting;                        // +0x17c
    float            BestScoreBlackjack;                     // +0x180
    float            BestScoreCoinGolfOakVale;               // +0x184
    float            BestScoreCoinGolfSnowSpire;             // +0x188
    float            BestScoreShoveHaPenny;                  // +0x18c
    float            BestTimeGuessTheAddition;               // +0x190
    bool             PrizeGivenPairs;                        // +0x194
    bool             PrizeGivenSorting;                      // +0x195
    bool             PrizeGivenBlackjack;                    // +0x196
    bool             PrizeGivenCoinGolfOakVale;              // +0x197
    bool             PrizeGivenCoinGolfSnowSpire;            // +0x198
    bool             PrizeGivenShoveHaPenny;                 // +0x199
    bool             PrizeGivenGuessTheAddition;             // +0x19a
    float            Drunkenness;                            // +0x19c
    float            BeerReservoir;                          // +0x1a0
    float            DrinkAffectMultiplier;                  // +0x1a4
    float            DrinkAffectMultiplierMultiplier;        // +0x1a8
    long             FrameToVomit;                           // +0x1ac
    long             DeedHandle;                             // +0x1b0
    long             FrameEndVomit;                          // +0x1b4
    bool             UpdateDrunkenness;                      // +0x1b8
    float            UCounter;                               // +0x1c8
    float            VCounter;                               // +0x1cc
    float            RotCounter;                             // +0x1d0
    bool             IsInCutSceneFirstTime;                  // +0x1d4
    std::list<CHeroMarriageInfo> Marriages;                         // +0x1d8 list<CHeroMarriageInfo,std::allocator<CHeroMarriageInfo>_>
 const CGameDefinitionManager& GetDefinitionManager()const{return *DefinitionManager;}
 CWorld& GetWorld()const{return World;}
 void OnSerialise(CPersistContext&,bool);
};
CTCHeroStats::MirroredCriminalStatus::MirroredCriminalStatus()
{
 for(long c=0;c<10;++c) Crimes[c]=0;
 EntrancePosX=0.0f;EntrancePosY=0.0f;EntrancePosZ=0.0f;
 LastTimeSeenByGuard=0;ExitedUsingTeleporter=false;LastTimeExited=0;EntranceAngle=0.0f;
}
void CTCHeroStats::OnSerialise(CPersistContext& context,bool in_game)
{
 context.Transfer("Morality",Morality);
 context.Transfer("MoralityChangingEnabled",MoralityChangingEnabled);
 context.Transfer("Age",Age);
 context.Transfer("SunTan",SunTan);
 context.Transfer("Fatness",Fatness);
 context.Transfer("Money",Money);
 context.Transfer("ConfiscatedMoney",ConfiscatedMoney);
 context.Transfer("RewardMoney",RewardMoney);
 context.Transfer("Stamina",Stamina);
 context.Transfer("FramesUntilStaminaRefills",FramesUntilStaminaRefills);
 context.Transfer("TrainingStatus",TrainingStatus);
 context.Transfer("RenownLevel",RenownLevel);
 context.Transfer("RenownTotal",RenownTotal);
 context.Transfer("RenownPointsInLevel",RenownPointsInLevel);
 context.Transfer("Agreeableness",Agreeableness);
 context.Transfer("InherentAttractiveness",InherentAttractiveness);
 context.Transfer("InherentScariness",InherentScariness);
 context.Transfer("ExtraMaxHealth",ExtraMaxHealth);
 context.Transfer("ExtraMaxStamina",ExtraMaxStamina);
 // Layout: the native definition-manager pointer at +dc has no surviving field name.
 context.Transfer("MaxStamina",MaxStamina,(*reinterpret_cast<const CHeroStatsDef* const*>(reinterpret_cast<const unsigned char*>(&GetDefinitionManager())+0xdc))->MaximumStamina+ExtraMaxStamina);
 context.Transfer("DeathRecoveryMarkerName",DeathRecoveryMarkerName);
 context.Transfer("FailQuestOnDeath",FailQuestOnDeath,true);
 context.Transfer("BestTimePairs",BestTimePairs);
 context.Transfer("BestTimeSorting",BestTimeSorting);
 context.Transfer("BestScoreBlackjack",BestScoreBlackjack);
 context.Transfer("BestScoreCoinGolfOakVale",BestScoreCoinGolfOakVale);
 context.Transfer("BestScoreCoinGolfSnowSpire",BestScoreCoinGolfSnowSpire);
 context.Transfer("BestScoreShoveHaPenny",BestScoreShoveHaPenny);
 context.Transfer("BestTimeGuessTheAddition",BestTimeGuessTheAddition);
 context.Transfer("PrizeGivenPairs",PrizeGivenPairs);
 context.Transfer("PrizeGivenSorting",PrizeGivenSorting);
 context.Transfer("PrizeGivenBlackjack",PrizeGivenBlackjack);
 context.Transfer("PrizeGivenCoinGolfOakVale",PrizeGivenCoinGolfOakVale);
 context.Transfer("PrizeGivenCoinGolfSnowSpire",PrizeGivenCoinGolfSnowSpire);
 context.Transfer("PrizeGivenShoveHaPenny",PrizeGivenShoveHaPenny);
 context.Transfer("PrizeGivenGuessTheAddition",PrizeGivenGuessTheAddition);
 context.Transfer("Drunkenness",Drunkenness);
 context.Transfer("NumFailedAQuest",NumFailedAQuest);
 context.Transfer("NumCompletedAQuest",NumCompletedAQuest);
 context.Transfer("NumCompletedACoreQuest",NumCompletedACoreQuest);
 context.Transfer("NumCompletedAFeatQuest",NumCompletedAFeatQuest);
 context.Transfer("NumCompletedAnOptionalQuest",NumCompletedAnOptionalQuest);
 context.Transfer("NumBoastsTaken",NumBoastsTaken);
 context.Transfer("NumBoastsCompleted",NumBoastsCompleted);
 context.Transfer("NumMountedTrophies",NumMountedTrophies);
 context.Transfer("NumTrophyGamePlayed",NumTrophyGamePlayed);
 context.Transfer("BestTrophyWitnesses",BestTrophyWitnesses);
 context.Transfer("NumberOfHousesOwned",NumberOfHousesOwned);
 context.Transfer("NumberOfShopsOwned",NumberOfShopsOwned);
 context.Transfer("NumberOfBuildingsRented",NumberOfBuildingsRented);
 context.Transfer("RentMoney",RentMoney);
 context.Transfer("NumberOfVomits",NumberOfVomits);
 context.Transfer("NumberOfDrinks",NumberOfDrinks);
 context.Transfer("NumberOfCrimes",NumberOfCrimes);
 context.Transfer("NumberOfCrimesOfMurder",NumberOfCrimesOfMurder);
 context.Transfer("TotalFine",TotalFine);
 context.Transfer("FinePaid",FinePaid);
 context.Transfer("TimesThrownOut",TimesThrownOut);
 context.Transfer("TotalBribeAmount",TotalBribeAmount);
 context.Transfer("NumLoversAcquired",NumLoversAcquired);
 context.Transfer("NumWeddings",NumWeddings);
 context.Transfer("NumSpousesMurdered",NumSpousesMurdered);
 context.Transfer("NumSpousesDied",NumSpousesDied);
 context.Transfer("NumDivorces",NumDivorces);
 context.Transfer("MaxMoney",MaxMoney);
 context.Transfer("TotalMoneyEarned",TotalMoneyEarned);
 context.Transfer("TotalMoneySpent",TotalMoneySpent);
 context.TransferVector("HeroStatsExperience",HeroStatsExperience);
 context.Transfer("TotalSpentExperience",TotalSpentExperience);
 context.TransferVector("RegionTime",RegionTime);
 context.Transfer("NumTeleports",NumTeleports);
 context.Transfer("NumDecapitations",NumberOfDecapitations);
 context.TransferVector("NumberOfTimesUsedAbility",NumberOfTimesUsedAbility);
 context.Transfer("MaxChickenThrow",MaxChickenThrow);
 context.Transfer("NumSex",NumSex);
 context.Transfer("HadScriptedSex",HadScriptedSex);
 context.Transfer("HadScriptedGaySex",HadScriptedGaySex);
 context.TransferVector("ExperienceSpentOn",ExperienceSpentOn);
 context.Transfer("MaxMultiplier",MaximumMultiplier);
 context.Transfer("BiggestFish",BiggestFish);
 if(in_game) {
  if(context.IsSaving()) {
   std::vector<long> region_ints;
   std::vector<float> region_floats;
   std::vector<CCharString> region_names;
   for(std::map<CCharString,MirroredCriminalStatus>::iterator i=OutstandingCrimes.begin();i!=OutstandingCrimes.end();++i) {
    for(long c=0;c<10;++c) region_ints.push_back(i->second.Crimes[c]);
    region_ints.push_back(i->second.LastTimeSeenByGuard-GetWorld().GetFrame());
    region_ints.push_back(i->second.LastTimeExited-GetWorld().GetFrame());
    region_ints.push_back(i->second.ExitedUsingTeleporter?true:false);
    region_floats.push_back(i->second.EntrancePosX);
    region_floats.push_back(i->second.EntrancePosY);
    region_floats.push_back(i->second.EntrancePosZ);
    region_floats.push_back(i->second.EntranceAngle);
    region_names.push_back(i->first);
   }
   context.TransferVector("CrimeRegionInts",region_ints);
   context.TransferVector("CrimeRegionFloats",region_floats);
   context.TransferVector("CrimeRegionNames",region_names);
  } else {
   std::vector<long> region_ints;
   std::vector<float> region_floats;
   std::vector<CCharString> region_names;
   context.TransferVector("CrimeRegionInts",region_ints);
   context.TransferVector("CrimeRegionFloats",region_floats);
   context.TransferVector("CrimeRegionNames",region_names);
   long i=0,f=0;
   for(long n=0;n<region_names.size();++n) {
    MirroredCriminalStatus status;
    for(long c=0;c<10;++c) status.Crimes[c]=region_ints[i++];
    status.LastTimeSeenByGuard=region_ints[i++]+GetWorld().GetFrame();
    status.LastTimeExited=region_ints[i++]+GetWorld().GetFrame();
    status.ExitedUsingTeleporter=region_ints[i++]>0;
    status.EntrancePosX=region_floats[f++];
    status.EntrancePosY=region_floats[f++];
    status.EntrancePosZ=region_floats[f++];
    status.EntranceAngle=region_floats[f++];
    OutstandingCrimes[region_names[n]]=status;
   }
  }
  if(context.IsSaving()) {
   std::vector<CHeroMarriageInfo> marriages_vector;
   marriages_vector.reserve(Marriages.size());
   std::copy(Marriages.begin(),Marriages.end(),std::back_inserter(marriages_vector));
   context.TransferVector("Marriage",marriages_vector);
  } else {
   std::vector<CHeroMarriageInfo> marriages_vector;
   context.TransferVector("Marriage",marriages_vector);
   Marriages.clear();
   std::copy(marriages_vector.begin(),marriages_vector.end(),std::back_inserter(Marriages));
  }
 }
}

static void Init(CTCHeroStats& x){
 x.NumberOfDecapitations=7;
 x.Morality=7;
 x.MoralityChangingEnabled=false;
 x.Age=1.5f;
 x.SunTan=1.5f;
 x.Fatness=1.5f;
 x.Money=7;
 x.MaxMoney=7;
 x.ConfiscatedMoney=7;
 x.TotalMoneyEarned=7;
 x.TotalMoneySpent=7;
 x.RewardMoney=7;
 x.RentMoney=7;
 x.Stamina=7;
 x.MaxStamina=7;
 x.FramesUntilStaminaRefills=7;
 x.FramesUntilHealthRefills=7;
 x.ScriptOverridePreventStaminaRefill=false;
 x.FramesUntilFatnessReduces=7;
 x.RenownLevel=7;
 x.RenownMaxLevel=7;
 x.RenownPointsInLevel=7;
 x.RenownTotal=7;
 x.Agreeableness=1.5f;
 x.InherentAttractiveness=1.5f;
 x.InherentScariness=1.5f;
 x.ExtraMaxHealth=1.5f;
 x.ExtraMaxStamina=7;
 x.FramesLeftForSpeedMultiplier=7;
 x.MaximumMultiplier=7;
 x.Attractiveness=1.5f;
 x.Scariness=1.5f;
 x.AppearanceGoodstrength=1.5f;
 x.SoundRadiusMultiplier=1.5f;
 x.VisibilityMultiplier=1.5f;
 x.FrameUpdatedClothingStats=7;
 x.FrameUpdatedTattooStats=7;
 x.NumFailedAQuest=7;
 x.NumCompletedAQuest=7;
 x.NumCompletedACoreQuest=7;
 x.NumCompletedAFeatQuest=7;
 x.NumCompletedAnOptionalQuest=7;
 x.NumBoastsTaken=7;
 x.NumBoastsCompleted=7;
 x.NumMountedTrophies=7;
 x.NumTrophyGamePlayed=7;
 x.BestTrophyWitnesses=7;
 x.NumberOfShopsOwned=7;
 x.NumberOfHousesOwned=7;
 x.NumberOfBuildingsRented=7;
 x.NumberOfVomits=7;
 x.NumberOfDrinks=7;
 x.NumberOfCrimes=7;
 x.NumberOfCrimesOfMurder=7;
 x.TotalFine=7;
 x.FinePaid=7;
 x.TimesThrownOut=7;
 x.TotalBribeAmount=7;
 x.NumTeleports=7;
 x.MaxChickenThrow=1.5f;
 x.TrainingStatus=TRAINING_STATUS_GRADUATED;
 x.TotalSpentExperience=7;
 x.NumLoversAcquired=7;
 x.NumWeddings=7;
 x.NumSpousesMurdered=7;
 x.NumSpousesDied=7;
 x.NumDivorces=7;
 x.NumSex=7;
 x.HadScriptedSex=false;
 x.HadScriptedGaySex=false;
 x.BiggestFish=1.5f;
 x.InfiniteStamina=false;
 x.FailQuestOnDeath=false;
 x.BestTimePairs=1.5f;
 x.BestTimeSorting=1.5f;
 x.BestScoreBlackjack=1.5f;
 x.BestScoreCoinGolfOakVale=1.5f;
 x.BestScoreCoinGolfSnowSpire=1.5f;
 x.BestScoreShoveHaPenny=1.5f;
 x.BestTimeGuessTheAddition=1.5f;
 x.PrizeGivenPairs=false;
 x.PrizeGivenSorting=false;
 x.PrizeGivenBlackjack=false;
 x.PrizeGivenCoinGolfOakVale=false;
 x.PrizeGivenCoinGolfSnowSpire=false;
 x.PrizeGivenShoveHaPenny=false;
 x.PrizeGivenGuessTheAddition=false;
 x.Drunkenness=1.5f;
 x.BeerReservoir=1.5f;
 x.DrinkAffectMultiplier=1.5f;
 x.DrinkAffectMultiplierMultiplier=1.5f;
 x.FrameToVomit=7;
 x.DeedHandle=7;
 x.FrameEndVomit=7;
 x.UpdateDrunkenness=false;
 x.UCounter=1.5f;
 x.VCounter=1.5f;
 x.RotCounter=1.5f;
 x.IsInCutSceneFirstTime=false;
 x.DeathRecoveryMarkerName="recover";x.ExperienceSpentOn.push_back(3);x.HeroStatsExperience.push_back(9);x.RegionTime.push_back(15);x.NumberOfTimesUsedAbility.push_back(4);
}
static void CheckOrder(const CPersistContext& c,bool in_game) {
 static const char* expected[]={"Morality","MoralityChangingEnabled","Age","SunTan","Fatness","Money","ConfiscatedMoney","RewardMoney","Stamina","FramesUntilStaminaRefills","TrainingStatus","RenownLevel","RenownTotal","RenownPointsInLevel","Agreeableness","InherentAttractiveness","InherentScariness","ExtraMaxHealth","ExtraMaxStamina","MaxStamina","DeathRecoveryMarkerName","FailQuestOnDeath","BestTimePairs","BestTimeSorting","BestScoreBlackjack","BestScoreCoinGolfOakVale","BestScoreCoinGolfSnowSpire","BestScoreShoveHaPenny","BestTimeGuessTheAddition","PrizeGivenPairs","PrizeGivenSorting","PrizeGivenBlackjack","PrizeGivenCoinGolfOakVale","PrizeGivenCoinGolfSnowSpire","PrizeGivenShoveHaPenny","PrizeGivenGuessTheAddition","Drunkenness","NumFailedAQuest","NumCompletedAQuest","NumCompletedACoreQuest","NumCompletedAFeatQuest","NumCompletedAnOptionalQuest","NumBoastsTaken","NumBoastsCompleted","NumMountedTrophies","NumTrophyGamePlayed","BestTrophyWitnesses","NumberOfHousesOwned","NumberOfShopsOwned","NumberOfBuildingsRented","RentMoney","NumberOfVomits","NumberOfDrinks","NumberOfCrimes","NumberOfCrimesOfMurder","TotalFine","FinePaid","TimesThrownOut","TotalBribeAmount","NumLoversAcquired","NumWeddings","NumSpousesMurdered","NumSpousesDied","NumDivorces","MaxMoney","TotalMoneyEarned","TotalMoneySpent","HeroStatsExperience","TotalSpentExperience","RegionTime","NumTeleports","NumDecapitations","NumberOfTimesUsedAbility","MaxChickenThrow","NumSex","HadScriptedSex","HadScriptedGaySex","ExperienceSpentOn","MaxMultiplier","BiggestFish"};
 assert(c.Trace.size()==(in_game?84:80));for(unsigned n=0;n<80;++n)assert(c.Trace[n]==expected[n]);
 if(in_game){assert(c.Trace[80]=="CrimeRegionInts"&&c.Trace[81]=="CrimeRegionFloats"&&c.Trace[82]=="CrimeRegionNames"&&c.Trace[83]=="Marriage");}
}
static void CheckEqual(const CTCHeroStats::MirroredCriminalStatus&a,const CTCHeroStats::MirroredCriminalStatus&b,long shift){
 for(int c=0;c<10;++c)assert(a.Crimes[c]==b.Crimes[c]);
 assert(b.LastTimeSeenByGuard==a.LastTimeSeenByGuard+shift&&b.LastTimeExited==a.LastTimeExited+shift);
 assert(a.ExitedUsingTeleporter==b.ExitedUsingTeleporter);
 assert(a.EntrancePosX==b.EntrancePosX&&a.EntrancePosY==b.EntrancePosY&&a.EntrancePosZ==b.EntrancePosZ&&a.EntranceAngle==b.EntranceAngle);
}
int main(){
 // Align the proven opaque +dc pointer slot on both host ABIs without changing the function body.
 union Arena{void* Alignment;unsigned char Bytes[512];} arena;
 unsigned char* storage=arena.Bytes;while((reinterpret_cast<size_t>(storage)+0xdc)%sizeof(void*))++storage;
 DefinitionManager=new(storage) CGameDefinitionManager;
 CHeroStatsDef definition;definition.MaximumStamina=64;
 typedef const CHeroStatsDef* DefinitionPtr;new(DefinitionManager->Storage+0xdc) DefinitionPtr(&definition);
 for(int count=0;count<4;++count)for(int marriageCount=0;marriageCount<3;++marriageCount)for(int enabled=0;enabled<2;++enabled){
  CTCHeroStats original;Init(original);original.ExtraMaxStamina=9;original.MaxStamina=99;
  for(int n=0;n<count;++n){char key[8];std::sprintf(key,"area%d",n);CTCHeroStats::MirroredCriminalStatus status;
   for(int c=0;c<10;++c)status.Crimes[c]=n*20+c;
   status.LastTimeSeenByGuard=80+n;status.LastTimeExited=120+n;status.ExitedUsingTeleporter=(n%2)!=0;
   status.EntrancePosX=0.5f*n;status.EntrancePosY=-1.5f*n;status.EntrancePosZ=2.0f*n;status.EntranceAngle=0.25f*n;
   original.OutstandingCrimes[key]=status;
  }
  for(int n=0;n<marriageCount;++n){CHeroMarriageInfo marriage;marriage.WifeUID=10+n;marriage.MaritalHomeUID=20+n;marriage.State=n;marriage.GayMarriage=(n!=0);original.Marriages.push_back(marriage);}
  CWorld::Frame=100;CWorld::Calls=0;CPersistContext saved(true);original.OnSerialise(saved,enabled!=0);CheckOrder(saved,enabled!=0);assert(CWorld::Calls==(enabled?count*2:0));
  if(enabled){assert(saved.LongVectors["CrimeRegionInts"].size()==(size_t)count*13);assert(saved.FloatVectors["CrimeRegionFloats"].size()==(size_t)count*4);assert(saved.StringVectors["CrimeRegionNames"].size()==(size_t)count);assert(saved.MarriageVectors["Marriage"].size()==(size_t)marriageCount);}
  CTCHeroStats restored;Init(restored);CTCHeroStats::MirroredCriminalStatus old;old.Crimes[0]=55;restored.OutstandingCrimes["preserved"]=old;CHeroMarriageInfo oldMarriage;oldMarriage.WifeUID=999;restored.Marriages.push_back(oldMarriage);
  CPersistContext loaded=saved;loaded.Saving=false;loaded.Trace.clear();CWorld::Frame=164;CWorld::Calls=0;restored.OnSerialise(loaded,enabled!=0);CheckOrder(loaded,enabled!=0);
  assert(restored.MaxStamina==73&&restored.FailQuestOnDeath);
  assert(restored.OutstandingCrimes["preserved"].Crimes[0]==55);
  if(enabled){
   assert(restored.OutstandingCrimes.size()==(size_t)count+1);assert(restored.Marriages.size()==(size_t)marriageCount);
   for(std::map<CCharString,CTCHeroStats::MirroredCriminalStatus>::const_iterator i=original.OutstandingCrimes.begin();i!=original.OutstandingCrimes.end();++i)CheckEqual(i->second,restored.OutstandingCrimes[i->first],64);
   std::list<CHeroMarriageInfo>::const_iterator a=original.Marriages.begin(),b=restored.Marriages.begin();for(;a!=original.Marriages.end();++a,++b){assert(a->WifeUID==b->WifeUID&&a->MaritalHomeUID==b->MaritalHomeUID&&a->State==b->State&&a->GayMarriage==b->GayMarriage);}
   assert(CWorld::Calls==count*2);
  }else{assert(restored.OutstandingCrimes.size()==1&&restored.Marriages.size()==1&&restored.Marriages.front().WifeUID==999&&CWorld::Calls==0);}
 }
 std::puts("0057e2f1_TEST PASS");return 0;
}
