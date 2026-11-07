// CTCHeroStats::OnSerialise(CPersistContext&,bool) -- Transfer hero statistics and in-game crime and marriage state.
// Leaf: ?OnSerialise@CTCHeroStats@@MAEXAAVCPersistContext@@_N@Z
// Flags: /O1 /Oy- /Oi
// Retail: Fable.exe 0x0057E2F1, RELOCATION_MATCH (chunk 849 rebase of the held Codex pass 12 source).
// Callees: the out-of-line CPersistContext::Transfer<long/bool/float/CCharString/EHeroTrainingStatus> (2- and 3-argument) and
//   TransferVector<long/float/CCharString/CHeroMarriageInfo>, IsSaving 0x004AEF50, CTCBase::GetDefinitionManager 0x006869D0 /
//   GetWorld 0x00686A70, CWorld::GetFrame 0x0049D870, vector push_back<long/float/CCharString>, map operator[] 0x005812DF,
//   MirroredCriminalStatus() 0x0057A5DD, vector<CHeroMarriageInfo>::reserve 0x005806DF, std::copy 0x00581376 / 0x005813A0,
//   _List_base<CHeroMarriageInfo>::clear 0x0057F58E.
// Shape: Codex body; the max-stamina default reads CGameDefinitionManager::PHeroStatsDef (+0xDC, descriptive) instead of the Codex
//   reinterpret_cast; Marriages.size() is STLport's distance(begin(),end()) form (FABLE_RETAIL_STLP_LIST_SIZE_DISTANCE).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCHEROSTATS
#define FABLE_RETAIL_CTCHEROSTATS_SERIALISE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER_HEROSTATSDEF
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CPERSISTCONTEXT_ISSAVING_OUTOFLINE
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_LIST_SIZE_DISTANCE
#define FABLE_RETAIL_STLP_LIST_CLEAR_OUTOFLINE
#define FABLE_RETAIL_STLP_BACK_INSERTER
#include <string.h>
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
#include "fable_string.h"
#include "engine/CTCHeroStats.h"
#include "engine/CHeroStatsDef.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CPersistContext.h"
#include "engine/CWorld.h"
namespace std { template<> vector<CCharString>::~vector(); }
#pragma auto_inline(off)
namespace std {
template<> void vector<long>::_M_insert_overflow(iterator position,const long& x,const __true_type&,size_type fillLength,bool atEnd) {
 const size_type oldSize=size();const size_type len=oldSize+(oldSize<fillLength?fillLength:oldSize);
 long* newStart=this->_M_end_of_storage.allocate(len);
 size_type before=position-this->_M_start;
 memmove(newStart,this->_M_start,before*sizeof(long));
 long* newFinish=newStart+before;
 for(size_type n=fillLength;n>0;--n,++newFinish)*newFinish=x;
 if(!atEnd){size_type after=this->_M_finish-position;memmove(newFinish,position,after*sizeof(long));newFinish+=after;}
 this->_M_end_of_storage.deallocate(this->_M_start,this->_M_end_of_storage._M_data-this->_M_start);
 this->_M_start=newStart;this->_M_finish=newFinish;this->_M_end_of_storage._M_data=newStart+len;
}
template<> void vector<long>::push_back(const long& x) {
 if(this->_M_finish!=this->_M_end_of_storage._M_data) {*this->_M_finish=x;++this->_M_finish;}
 else _M_insert_overflow(end(),x,__true_type(),1,true);
}
}
CTCHeroStats::MirroredCriminalStatus::MirroredCriminalStatus()
{
 for(long c=0;c<10;++c) Crimes[c]=0;
 EntrancePosX=0.0f;EntrancePosY=0.0f;EntrancePosZ=0.0f;
 LastTimeSeenByGuard=0;ExitedUsingTeleporter=false;LastTimeExited=0;EntranceAngle=0.0f;
}
#pragma auto_inline(on)
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
 context.Transfer("MaxStamina",MaxStamina,GetDefinitionManager().PHeroStatsDef->MaximumStamina+ExtraMaxStamina);
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