#include <stdio.h>
#include <vector>
enum EHeroExperienceStatCategory{HERO_STAT_STRENGTH,HERO_STAT_SKILL,HERO_STAT_WILL,NUMBER_OF_HERO_STAT_CATEGORIES};
template<class T,class L>inline void GFLimitLower(T& v,const L& lo){if(v<lo)v=lo;}
class CTCHeroStats{public:int Count;EHeroExperienceStatCategory LastCategory;long LastAmount;CTCHeroStats():Count(0),LastCategory(HERO_STAT_STRENGTH),LastAmount(0){}void AddHeroStatExperience(EHeroExperienceStatCategory c,long n){++Count;LastCategory=c;LastAmount=n;}};
class CThing{public:CTCHeroStats* Stats;float Weapon;CThing():Stats(0),Weapon(1){}template<class T>bool GetTC(T** p){if(Stats){*p=Stats;return true;}return false;}};
namespace NHeroExperience{float GetWeaponModifiedExperienceMultiplierForThing(const CThing& t){return t.Weapon;}}
class CHeroExperienceDef{public:float ExperienceMultiplierParamPerUnitComboMultiplier;};
class CTCHeroExperience{public:
 CThing* ParentThing;std::vector<long> StatExperiencePoints;float ExperienceMultiplierParam;bool AbleToGainExperience;CHeroExperienceDef* PDef;
 CThing& GetParentThing(){return *ParentThing;}
 long GetExperiencePickUpMultiplier()const{return GetExperiencePickupMultiplierForParam(ExperienceMultiplierParam);}
 void AddTrainableStatExperience(EHeroExperienceStatCategory,long);
private:
 long GetExperiencePickupMultiplierForParam(float param)const{float param_per_level=PDef->ExperienceMultiplierParamPerUnitComboMultiplier;long multiplier=(long)(param/param_per_level+1.0);GFLimitLower(multiplier,1);return multiplier;}
};
void CTCHeroExperience::AddTrainableStatExperience(EHeroExperienceStatCategory stat_type,long experience)
{
 if(AbleToGainExperience){
  long experience_to_collect=GetExperiencePickUpMultiplier()*experience;
  experience_to_collect=(long)(NHeroExperience::GetWeaponModifiedExperienceMultiplierForThing(GetParentThing())*experience_to_collect);
  StatExperiencePoints[stat_type]+=experience_to_collect;
  GFLimitLower(StatExperiencePoints[stat_type],0);
  CTCHeroStats* HeroStats;
  if(GetParentThing().GetTC(&HeroStats))HeroStats->AddHeroStatExperience(stat_type,experience_to_collect);
 }
}

int main(){
 const float parameters[]={-8,-4,0,2,4,8};const float weapons[]={0,0.5f,1,1.5f,2};const long amounts[]={-16,-4,0,4,16};
 for(int enabled=0;enabled<2;++enabled)for(int present=0;present<2;++present)for(int c=0;c<3;++c)for(int p=0;p<6;++p)for(int w=0;w<5;++w)for(int a=0;a<5;++a){
  CThing thing;CTCHeroStats stats;thing.Stats=present?&stats:0;thing.Weapon=weapons[w];CHeroExperienceDef def;def.ExperienceMultiplierParamPerUnitComboMultiplier=2;
  CTCHeroExperience row;row.ParentThing=&thing;row.PDef=&def;row.ExperienceMultiplierParam=parameters[p];row.AbleToGainExperience=enabled!=0;row.StatExperiencePoints.assign(3,8);
  long multiplier=(long)(parameters[p]/2.0f+1.0);if(multiplier<1)multiplier=1;long gain=(long)(weapons[w]*(multiplier*amounts[a]));long expected=8+gain;if(expected<0)expected=0;
  row.AddTrainableStatExperience((EHeroExperienceStatCategory)c,amounts[a]);
  for(int k=0;k<3;++k)if(row.StatExperiencePoints[k]!=(enabled&&k==c?expected:8))return 1;
  if(stats.Count!=(enabled&&present?1:0))return 2;
  if(stats.Count&&(stats.LastCategory!=c||stats.LastAmount!=gain))return 3;
 }
 puts("006d8300_TEST PASS");return 0;
}