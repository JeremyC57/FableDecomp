// 006d8980 behavior fixture: target body pasted unchanged.
// Fixture-only constructors/public storage and reduced unused interfaces create real objects.
// External callbacks are checked boundaries; native offsets and symbols are reviewed separately.
#include <stdio.h>
#include <stdlib.h>
namespace std {template<class T>class vector{T*first;T*last;public:vector():first(0),last(0){}void Set(T*p,unsigned int n){first=p;last=p+n;}unsigned int size()const{return (unsigned int)(last-first);}T&operator[](unsigned int n){return first[n];}const T&operator[](unsigned int n)const{return first[n];}};}
static int failures=0;static void Check(bool x){if(!x)++failures;}
enum EHeroTrainableStatType{Stat0,Stat1,Stat2};
class CAbilityTrackLevelDef{};
class CAbilityTrackDef{public:std::vector<CAbilityTrackLevelDef>Abilities;};
class CHeroStatsDef{public:std::vector<CAbilityTrackDef>StatLevels;};
template<class T>class CDefPointer{T*Object;public:CDefPointer(T*p):Object(p){}T*operator->()const{return Object;}};
class CGlobalDef{public:float AgeIncreasePerLevelUp;};
class CGameDefinitionManager{const CGlobalDef&global;public:mutable int reads;CGameDefinitionManager(const CGlobalDef&g):global(g),reads(0){}const CGlobalDef&PeekGlobalDef()const{++reads;return global;}};
class CTCHeroStats{public:float Age;int sets,gets;CTCHeroStats(float a):Age(a),sets(0),gets(0){}float GetAge()const{++const_cast<CTCHeroStats*>(this)->gets;return Age;}void SetAge(float x){++sets;Age=x;}};
class CThing{public:CTCHeroStats*stats;int lookups;bool has;CThing(CTCHeroStats*p,bool h):stats(p),lookups(0),has(h){}template<class T>bool GetTC(T**out){++lookups;if(has){*out=stats;return true;}return false;}};
class CTCBase{protected:CThing&ParentThing;const CGameDefinitionManager&manager;CTCBase(CThing&t,const CGameDefinitionManager&m):ParentThing(t),manager(m){}const CGameDefinitionManager&GetDefinitionManager()const{return manager;}};
class CTCHeroExperience:public CTCBase{public:std::vector<long>TrainableStatLevels;CDefPointer<const CHeroStatsDef>PHeroStatsDef;int updates,changes,sequence;EHeroTrainableStatType wanted;long expected;bool switchPresence;
 CTCHeroExperience(CThing&t,const CGameDefinitionManager&m,const CHeroStatsDef*d,long*p):CTCBase(t,m),PHeroStatsDef(d),updates(0),changes(0),sequence(0),wanted(Stat0),expected(0),switchPresence(false){TrainableStatLevels.Set(p,3);}
 void UpgradeTrainableStat(EHeroTrainableStatType);
 void UpdateAbilitiesForStat(EHeroTrainableStatType s,bool b){Check(sequence++==0);Check(s==wanted&&!b);Check(TrainableStatLevels[s]==expected);++updates;}
 void OnStatLevelChange(bool b){Check(sequence++==1&&b);++changes;if(switchPresence)ParentThing.has=!ParentThing.has;}
};
void CTCHeroExperience::UpgradeTrainableStat(EHeroTrainableStatType stat){
 const std::vector<CAbilityTrackLevelDef>&abilities=PHeroStatsDef->StatLevels[stat].Abilities;
 long maxLevel=(long)abilities.size()-1;
 if(maxLevel<0)maxLevel=0;
 if(TrainableStatLevels[stat]<maxLevel){
  TrainableStatLevels[stat]=TrainableStatLevels[stat]+1;UpdateAbilitiesForStat(stat,false);OnStatLevelChange(true);
  const CGlobalDef&global=GetDefinitionManager().PeekGlobalDef();
  CThing*thing=&ParentThing;CTCHeroStats*stats;
  if(thing->GetTC(&stats))stats->SetAge(stats->GetAge()+global.AgeIncreasePerLevelUp);
 }
}

int main(){unsigned int cases=0;CAbilityTrackLevelDef levelStorage[3][5];
 for(int count=0;count<=5;++count)for(int stat=0;stat<3;++stat)for(int initial=-1;initial<=6;++initial)for(int present=0;present<2;++present)for(int mutate=0;mutate<2;++mutate)for(int ageCase=0;ageCase<3;++ageCase){
  CAbilityTrackDef tracks[3];for(int j=0;j<3;++j)tracks[j].Abilities.Set(levelStorage[j],j==stat?count:5);
  CHeroStatsDef def;def.StatLevels.Set(tracks,3);CGlobalDef global;global.AgeIncreasePerLevelUp=ageCase==0?0.0f:ageCase==1?0.5f:2.0f;
  CGameDefinitionManager manager(global);CTCHeroStats stats(16.0f);CThing thing(&stats,present!=0);long values[3]={2,3,4};values[stat]=initial;long before[3];for(int j=0;j<3;++j)before[j]=values[j];
  CTCHeroExperience xp(thing,manager,&def,values);xp.wanted=(EHeroTrainableStatType)stat;xp.expected=initial+1;xp.switchPresence=mutate!=0;
  int maximum=count>0?count-1:0;bool change=initial<maximum;bool ageChange=change&&((present!=0)!=(mutate!=0));
  xp.UpgradeTrainableStat((EHeroTrainableStatType)stat);
  for(int j=0;j<3;++j)Check(values[j]==before[j]+((j==stat&&change)?1:0));
  Check(xp.updates==(change?1:0)&&xp.changes==(change?1:0));Check(xp.sequence==(change?2:0));Check(manager.reads==(change?1:0)&&thing.lookups==(change?1:0));
  Check(stats.gets==(ageChange?1:0)&&stats.sets==(ageChange?1:0));Check(stats.Age==16.0f+(ageChange?global.AgeIncreasePerLevelUp:0.0f));++cases;
 }
 Check(cases==1728);if(failures){fprintf(stderr,"failures=%d cases=%u\n",failures,cases);return 1;}puts("006d8980_TEST PASS");return 0;
}
