// Selected caller body verbatim; real vectors and observable game-service stubs.
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#define CHECK(c) do{if(!(c)){printf("FAIL %d\n",__LINE__);abort();}}while(0)
enum ETCInterfaceType{TCI_GHOST_SWORD=0x9a};
static int particleUpdates,alphaCalls,postMode;
namespace NGraphicAppearance {class CTCGraphicAppearance{public:int Alpha,Calls;CTCGraphicAppearance():Alpha(-1),Calls(0){}void SetAlpha(unsigned char a){CHECK(particleUpdates==0);Alpha=a;++Calls;++alphaCalls;}};}
class CThing {public:bool Dead;NGraphicAppearance::CTCGraphicAppearance* Appearance;int Removes;CThing():Dead(false),Appearance(0),Removes(0){}bool IsDead()const{return Dead;}NGraphicAppearance::CTCGraphicAppearance* GetTCGraphicAppearance(){return Appearance;}void RemoveTC(ETCInterfaceType type){CHECK(type==TCI_GHOST_SWORD && particleUpdates==1);++Removes;}};
class CThingCreatureBase:public CThing{};
class CThingAICreature:public CThingCreatureBase{};
template<class T>class CIntelligentPointer {public:T* Value;CIntelligentPointer():Value(0){}T* Get()const{return Value;}T* operator->()const{return Value;}};
template<class T>bool operator!=(const CIntelligentPointer<T>& p,int){return p.Get()!=0;}
template<class T>bool IS_THING_ALIVE(const T& thing){if(thing!=0){if(!thing->IsDead())return true;}return false;}
template<class From,class To>To GFDebugPDynamicCast(From value,To){return static_cast<To>(value);}
class CTCBase {public:CThing* ParentThing;CThing& GetParentThing(){return *ParentThing;}};
static CThingCreatureBase* changedParent;
class CTCGhostSword:public CTCBase {
public:std::vector<CIntelligentPointer<CThingAICreature> > Minions;std::vector<long> CurFadeInTimer;long FadeInTimer;
 void UpdateGhostSwordParticleEffects(){CHECK(particleUpdates==0);++particleUpdates;for(unsigned i=0;i<Minions.size();++i)if(Minions[i].Value){if(postMode==1)Minions[i].Value->Dead=true;if(postMode==2)Minions[i].Value->Dead=false;}ParentThing=changedParent;}
 virtual void FrameUpdate();
};
void CTCGhostSword::FrameUpdate()
{
 for(long i=0;i<(long)CurFadeInTimer.size();++i){
  if(CurFadeInTimer[i]>0){
   CurFadeInTimer[i]--;
   if(IS_THING_ALIVE(Minions[i])){
    NGraphicAppearance::CTCGraphicAppearance* ptc_graphic_appearance=Minions[i]->GetTCGraphicAppearance();
    if(ptc_graphic_appearance){
     float ratio=(float)CurFadeInTimer[i]/FadeInTimer;
     ptc_graphic_appearance->SetAlpha((unsigned char)((1.0f-ratio)*255.0f));
    }
   }
  }
 }
 UpdateGhostSwordParticleEffects();
 CThingCreatureBase* pcreature=GFDebugPDynamicCast(&GetParentThing(),(CThingCreatureBase*)0);
 bool all_dead=true;
 for(long i=0;i<Minions.size();++i){
  if(IS_THING_ALIVE(Minions[i])){all_dead=false;break;}
 }
 if(all_dead)pcreature->RemoveTC(TCI_GHOST_SWORD);
}

int main(){int cases=0;
 for(int fade=4;fade<=8;fade+=4)for(int count=0;count<=4;++count)for(int pattern=0;pattern<8;++pattern)for(int mode=0;mode<3;++mode){
  CThingAICreature minions[4];NGraphicAppearance::CTCGraphicAppearance appearances[4];CThingCreatureBase before,after;
  CTCGhostSword spell;spell.ParentThing=&before;spell.FadeInTimer=fade;changedParent=&after;postMode=mode;
  const long times[]={-1,0,1,2,4};long expectedTimes[4];int expectedAlpha[4];int expectedCount=0;bool expectedAlive=false;
  for(int i=0;i<count;++i){int state=(pattern+i)%4;minions[i].Dead=state==1;minions[i].Appearance=state==3?&appearances[i]:0;CIntelligentPointer<CThingAICreature> ptr;ptr.Value=state?&minions[i]:0;spell.Minions.push_back(ptr);long time=times[(pattern+i)%5];spell.CurFadeInTimer.push_back(time);expectedTimes[i]=time>0?time-1:time;expectedAlpha[i]=-1;
   if(time>0 && state==3){expectedAlpha[i]=(255*(fade-int(time-1)))/fade;++expectedCount;}
   bool alive=state && (mode==2 || (mode==0 && state!=1));if(alive)expectedAlive=true;
  }
  particleUpdates=alphaCalls=0;spell.FrameUpdate();++cases;
  CHECK(particleUpdates==1 && alphaCalls==expectedCount && spell.ParentThing==&after);
  CHECK(before.Removes==0 && after.Removes==!expectedAlive);
  CHECK(spell.CurFadeInTimer.size()==(unsigned)count && spell.Minions.size()==(unsigned)count && spell.FadeInTimer==fade);
  for(int i=0;i<count;++i){CHECK(spell.CurFadeInTimer[i]==expectedTimes[i]);CHECK(appearances[i].Alpha==expectedAlpha[i]);CHECK(appearances[i].Calls==(expectedAlpha[i]>=0));}
 }
 CHECK(cases==240);puts("007c6460_TEST PASS");return 0;
}