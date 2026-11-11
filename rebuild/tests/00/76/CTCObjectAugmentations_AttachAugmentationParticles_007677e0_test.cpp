// Self-contained C++03 behavior and lifetime fixture; production body copied unchanged below.
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <map>
#include <algorithm>
static void check(bool x,int line){if(!x){std::printf("FAIL %d\n",line);std::exit(1);}}
#define CHECK(x) check(!!(x),__LINE__)
enum EObjectAugmentationType{A_ONE=1,A_FIRE=4,A_LIGHTNING=8,A_COMBO=12,A_OTHER=16};
enum ETCInterfaceType{TC_CHECK=0x77};
struct Event{long a,b;Event(long x,long y):a(x),b(y){}};
static std::vector<Event> events;
static long strings_live=0,pointers_live=0,null_assignments=0,string_gets=0,mutation=0;
class CMainGameComponent{public:static long fps;static long GetConstantFPS(){return fps;}};
long CMainGameComponent::fps=0;
class CCharString{
public:
 long Value;
 CCharString():Value(-999){++strings_live;}
 explicit CCharString(long v):Value(v){++strings_live;}
 CCharString(const CCharString&x):Value(x.Value){++strings_live;}
 ~CCharString(){CHECK(strings_live>0);--strings_live;}
 const CCharString&operator=(const CCharString&x){Value=x.Value;return *this;}
};
class CDefString{
public:long TablePos;CDefString():TablePos(-1){}explicit CDefString(long p):TablePos(p){}
 operator CCharString()const{
  events.push_back(Event(4,TablePos));++string_gets;
  if(mutation)CMainGameComponent::fps=2+(string_gets%4)*2;
  return CCharString(TablePos);
 }
};
class CThing{public:bool Limbo,Component;CThing():Limbo(false),Component(false){}
 bool IsInLimbo()const{return Limbo;}bool HasTC(ETCInterfaceType t)const{CHECK(t==TC_CHECK);return Component;}
};
template<class T>class CIntelligentPointer{
public:T*Value;
 CIntelligentPointer():Value(0){++pointers_live;}
 CIntelligentPointer(T*p):Value(p){++pointers_live;}
 CIntelligentPointer(const CIntelligentPointer&x):Value(x.Value){++pointers_live;}
 ~CIntelligentPointer(){CHECK(pointers_live>0);--pointers_live;}
 CIntelligentPointer&operator=(const CIntelligentPointer&x){Value=x.Value;++null_assignments;CHECK(!Value);return *this;}
};
struct BasicParticleAndEndPoints{CCharString StartPoint,EndPoint;long ParticleEffectID,ParticleEffectIDToBlendTo;};
struct ParticleAndEndPoints:BasicParticleAndEndPoints{long PeriodicFrames;};
struct AttackParticleAndEndPoints:BasicParticleAndEndPoints{CIntelligentPointer<CThing>ParticleEffect;};
class CObjectAugmentationParticleSet{
public:float PeriodSecs;long ParticleEffect,ParticleEffectToBlendTo,AttackParticleEffect,AttackParticleEffectToBlendTo;
 std::vector<CDefString>StartPoint,EndPoint;
 CObjectAugmentationParticleSet():PeriodSecs(0),ParticleEffect(0),ParticleEffectToBlendTo(0),AttackParticleEffect(0),AttackParticleEffectToBlendTo(0){}
};
class CObjectAugmentationsDef{public:std::map<EObjectAugmentationType,CObjectAugmentationParticleSet>AugmentationParticles;};
class CTCObjectAugmentations{
public:
 const CObjectAugmentationsDef*PDef;CThing Parent;
 std::vector<EObjectAugmentationType>Augmentations;
 std::vector<std::vector<ParticleAndEndPoints> >ParticleEffects;
 std::vector<std::vector<AttackParticleAndEndPoints> >AttackParticleEffects;
 CThing&GetParentThing(){return Parent;}
 void ClearAllParticleEffects(){events.push_back(Event(1,0));ParticleEffects.clear();AttackParticleEffects.clear();}
 bool HasAugmentationOfType(EObjectAugmentationType type)const{
  events.push_back(Event(3,type));return std::find(Augmentations.begin(),Augmentations.end(),type)!=Augmentations.end();
 }
 bool HasCombinationOfAugmentations(EObjectAugmentationType type)const{
  events.push_back(Event(2,type));long v=0;for(unsigned i=0;i<Augmentations.size();++i)v|=Augmentations[i];return (v&type)==type;
 }
 void AttachAugmentationParticles();
};
void CTCObjectAugmentations::AttachAugmentationParticles()
{
 ClearAllParticleEffects();
 if(!GetParentThing().IsInLimbo()&&!GetParentThing().HasTC((ETCInterfaceType)0x77)){
  typedef std::map<EObjectAugmentationType,CObjectAugmentationParticleSet> ParticleMap;
  ParticleMap::const_iterator findit=PDef->AugmentationParticles.find((EObjectAugmentationType)12);
  bool exists_lightning_and_flame_combo=findit!=PDef->AugmentationParticles.end()&&HasCombinationOfAugmentations((EObjectAugmentationType)12);
  ParticleMap::const_iterator it,endit;
  for(it=PDef->AugmentationParticles.begin(),endit=PDef->AugmentationParticles.end();it!=endit;++it){
   if(HasAugmentationOfType((*it).first)){
    long num=(*it).second.StartPoint.size();
    std::vector<ParticleAndEndPoints> peps;
    std::vector<AttackParticleAndEndPoints> apeps;
    for(long i=0;i<num;++i){
     if((*it).second.ParticleEffect>0){
      ParticleAndEndPoints pep;
      pep.PeriodicFrames=(long)((*it).second.PeriodSecs*CMainGameComponent::GetConstantFPS());
      pep.ParticleEffectID=(*it).second.ParticleEffect;
      pep.ParticleEffectIDToBlendTo=(*it).second.ParticleEffectToBlendTo;
      pep.StartPoint=(*it).second.StartPoint[i];
      pep.EndPoint=(*it).second.EndPoint[i];
      peps.push_back(pep);
     }
     if((*it).second.AttackParticleEffect>0&&(!exists_lightning_and_flame_combo||(*it).first==(EObjectAugmentationType)12)){
      AttackParticleAndEndPoints apep;
      apep.ParticleEffect=0;
      apep.ParticleEffectID=(*it).second.AttackParticleEffect;
      apep.ParticleEffectIDToBlendTo=(*it).second.AttackParticleEffectToBlendTo;
      apep.StartPoint=(*it).second.StartPoint[i];
      apep.EndPoint=(*it).second.EndPoint[i];
      apeps.push_back(apep);
     }
    }
    ParticleEffects.push_back(peps);
    AttackParticleEffects.push_back(apeps);
   }
   if(exists_lightning_and_flame_combo&&(*it).first==(EObjectAugmentationType)12){
    long num=(*it).second.StartPoint.size();
    std::vector<AttackParticleAndEndPoints> apeps;
    for(long i=0;i<num;++i){
     AttackParticleAndEndPoints apep;
     apep.ParticleEffect=0;
     apep.ParticleEffectID=(*it).second.AttackParticleEffect;
     apep.ParticleEffectIDToBlendTo=(*it).second.AttackParticleEffectToBlendTo;
     apep.StartPoint=(*it).second.StartPoint[i];
     apep.EndPoint=(*it).second.EndPoint[i];
     apeps.push_back(apep);
    }
    AttackParticleEffects.push_back(apeps);
   }
  }
 }
}

struct Result{long start,end,id,blend,frames;Result(long s,long e,long i,long b,long f):start(s),end(e),id(i),blend(b),frames(f){}};
static const EObjectAugmentationType keys[]={A_ONE,A_FIRE,A_LIGHTNING,A_COMBO,A_OTHER};
static bool has(unsigned mask,unsigned k){return (mask&(1u<<k))!=0;}
static void verify(unsigned mapmask,unsigned augmask,unsigned profile,long count,long fps,int stop,int mutate){
 CHECK(strings_live==0&&pointers_live==0);
 CObjectAugmentationsDef def;CTCObjectAugmentations row;row.PDef=&def;
 for(unsigned k=0;k<5;++k){
  if(has(augmask,k))row.Augmentations.push_back(keys[k]);
  if(!has(mapmask,k))continue;
  CObjectAugmentationParticleSet d;
  d.PeriodSecs=(k&1)?0.5f:0.25f;
  d.ParticleEffect=(profile&1)?100+(long)k:-(long)k;
  d.ParticleEffectToBlendTo=200+(long)k;
  d.AttackParticleEffect=(profile&2)?300+(long)k:-(long)k;
  d.AttackParticleEffectToBlendTo=400+(long)k;
  for(long j=0;j<count;++j){d.StartPoint.push_back(CDefString(1000+k*100+j));d.EndPoint.push_back(CDefString(2000+k*100+j));}
  def.AugmentationParticles.insert(std::make_pair(keys[k],d));
 }
 row.Parent.Limbo=(stop&1)!=0;row.Parent.Component=(stop&2)!=0;
 // Pre-existing owned output must be cleared before any early return.
 row.ParticleEffects.resize(1);row.ParticleEffects[0].resize(2);
 row.AttackParticleEffects.resize(1);row.AttackParticleEffects[0].resize(3);
 CMainGameComponent::fps=fps;events.clear();string_gets=0;mutation=mutate;null_assignments=0;
 row.AttachAugmentationParticles();
 std::vector<Event>expected;expected.push_back(Event(1,0));
 std::vector<std::vector<Result> >periodic,attack;
 long actualfps=fps,expectedgets=0,expectedassign=0;
 if(!stop){
  unsigned uniontypes=0;for(unsigned j=0;j<5;++j)if(has(augmask,j))uniontypes|=keys[j];
  bool combo=has(mapmask,3)&&((uniontypes&12)==12);
  if(has(mapmask,3))expected.push_back(Event(2,12));
  for(unsigned k=0;k<5;++k){
   if(!has(mapmask,k))continue;
   expected.push_back(Event(3,keys[k]));
   bool included=has(augmask,k),normal=(profile&1)!=0,combat=(profile&2)!=0;
   // A row with this exact augmentation creates both categories, including empty vectors.
   if(included){
    std::vector<Result>p,a;
    for(long j=0;j<count;++j){
     if(normal){
      long frames=(long)(((k&1)?0.5f:0.25f)*actualfps);
      p.push_back(Result(1000+k*100+j,2000+k*100+j,100+k,200+k,frames));
      for(long side=0;side<2;++side){expected.push_back(Event(4,(side?2000:1000)+k*100+j));++expectedgets;if(mutate)actualfps=2+(expectedgets%4)*2;}
     }
     if(combat&&(!combo||k==3)){
      a.push_back(Result(1000+k*100+j,2000+k*100+j,300+k,400+k,0));++expectedassign;
      for(long side=0;side<2;++side){expected.push_back(Event(4,(side?2000:1000)+k*100+j));++expectedgets;if(mutate)actualfps=2+(expectedgets%4)*2;}
     }
    }
    periodic.push_back(p);attack.push_back(a);
   }
   // The combination path ignores the sign of AttackParticleEffect and may append a second category.
   if(combo&&k==3){
    std::vector<Result>a;
    for(long j=0;j<count;++j){
     a.push_back(Result(1000+k*100+j,2000+k*100+j,combat?300+(long)k:-(long)k,400+k,0));++expectedassign;
     for(long side=0;side<2;++side){expected.push_back(Event(4,(side?2000:1000)+k*100+j));++expectedgets;if(mutate)actualfps=2+(expectedgets%4)*2;}
    }
    attack.push_back(a);
   }
  }
 }
 CHECK(row.ParticleEffects.size()==periodic.size());CHECK(row.AttackParticleEffects.size()==attack.size());
 long items=0,attackitems=0;
 for(unsigned k=0;k<periodic.size();++k){CHECK(row.ParticleEffects[k].size()==periodic[k].size());for(unsigned j=0;j<periodic[k].size();++j){
  const ParticleAndEndPoints&a=row.ParticleEffects[k][j];const Result&b=periodic[k][j];
  CHECK(a.StartPoint.Value==b.start&&a.EndPoint.Value==b.end&&a.ParticleEffectID==b.id&&a.ParticleEffectIDToBlendTo==b.blend&&a.PeriodicFrames==b.frames);++items;
 }}
 for(unsigned k=0;k<attack.size();++k){CHECK(row.AttackParticleEffects[k].size()==attack[k].size());for(unsigned j=0;j<attack[k].size();++j){
  const AttackParticleAndEndPoints&a=row.AttackParticleEffects[k][j];const Result&b=attack[k][j];
  CHECK(a.StartPoint.Value==b.start&&a.EndPoint.Value==b.end&&a.ParticleEffectID==b.id&&a.ParticleEffectIDToBlendTo==b.blend&&!a.ParticleEffect.Value);++items;++attackitems;
 }}
 CHECK(strings_live==2*items&&pointers_live==attackitems);CHECK(null_assignments==expectedassign);CHECK(string_gets==expectedgets&&CMainGameComponent::fps==actualfps);
 CHECK(events.size()==expected.size());for(unsigned i=0;i<events.size();++i)CHECK(events[i].a==expected[i].a&&events[i].b==expected[i].b);
 CHECK(row.Augmentations.size()==(unsigned)(has(augmask,0)+has(augmask,1)+has(augmask,2)+has(augmask,3)+has(augmask,4)));
 row.ParticleEffects.clear();row.AttackParticleEffects.clear();CHECK(strings_live==0&&pointers_live==0);
}
int main(){
 long cases=0;
 for(unsigned mapmask=0;mapmask<32;++mapmask)for(unsigned augmask=0;augmask<32;++augmask)for(unsigned profile=0;profile<4;++profile)
 for(long count=0;count<4;++count)for(int stop=0;stop<4;++stop)for(int mutate=0;mutate<2;++mutate){
  verify(mapmask,augmask,profile,count,8,stop,mutate);++cases;
 }
 CHECK(cases==131072);std::printf("007677e0_TEST PASS\n");return 0;
}