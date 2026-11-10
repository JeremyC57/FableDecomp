// Standalone fixture: selected caller body is verbatim; external services are observable stubs.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <map>
#define CHECK(c) do{if(!(c)){printf("FAIL %d\n",__LINE__);abort();}}while(0)
static int baseCalls,peeks,getCalls,adds,reads,updates,liveStrings;
class CCharString {
public:
 const char* Text;
 CCharString(const char* s,long n=-1):Text(s){CHECK(n==-1);++liveStrings;}
 CCharString(const CCharString& x):Text(x.Text){++liveStrings;}
 ~CCharString(){--liveStrings;}
};
class CTCBase {public:virtual ~CTCBase(){}};
class CTCBulletTime:public CTCBase {
public:
 float Value;
 float GetTimeMultiplier()const{CHECK(baseCalls==1 && liveStrings==0);++reads;return Value;}
};
class CTCHaste:public CTCBase {
public:
 std::map<void*,float> Multipliers;
 void UpdateActionSpeedMultiplier(){CHECK(reads==1 && liveStrings==0);++updates;}
 void SetActionSpeedMultiplier(void* key,float multiplier){
  Multipliers.insert(std::make_pair(key,multiplier));
  UpdateActionSpeedMultiplier();
 }
};
class CTCParamsBase {};
class CThingCreatureBase {
public:
 const CTCBulletTime* Bullet;
 CTCHaste* Haste;
 CTCHaste Added;
 bool PeekTC(const CTCBulletTime** out)const{
  CHECK(baseCalls==1 && getCalls==0 && adds==0);++peeks;
  if(Bullet){*out=Bullet;return true;}return false;
 }
 bool GetTC(CTCHaste** out){
  CHECK(peeks==1 && Bullet!=0 && reads==0);++getCalls;
  if(Haste){*out=Haste;return true;}return false;
 }
 CTCBase* AddTC(const CCharString& name,bool init,const CTCParamsBase* params){
  CHECK(getCalls==1 && Haste==0 && init && params==0 && liveStrings>0);
  CHECK(strcmp(name.Text,"CTCHaste")==0);++adds;Haste=&Added;return Haste;
 }
};
template<class T>class CIntelligentPointer {
public:T* Value;T* operator->()const{return Value;}
};
template<class T>T* GFDebugPDynamicCast(CTCBase* p,T*){return static_cast<T*>(p);}
class CCreatureAction_UnsheatheItemFromInventory {
public:
 CIntelligentPointer<CThingCreatureBase> PCreature;
 virtual void FinishAction(){CHECK(baseCalls==0 && peeks==0 && getCalls==0);++baseCalls;}
};
class CCreatureAction_UnsheatheItemFromInventoryWithHasteAction:public CCreatureAction_UnsheatheItemFromInventory {
public:void* HasteKey;virtual void FinishAction();
};
void CCreatureAction_UnsheatheItemFromInventoryWithHasteAction::FinishAction()
{
 CCreatureAction_UnsheatheItemFromInventory::FinishAction();
 const CTCBulletTime* tctime=0;
 if(PCreature->PeekTC(&tctime)){
  CTCHaste* ptc_haste=0;
  if(!PCreature->GetTC(&ptc_haste))
   ptc_haste=GFDebugPDynamicCast(PCreature->AddTC("CTCHaste",true,0),(CTCHaste*)0);
  ptc_haste->SetActionSpeedMultiplier(HasteKey,tctime->GetTimeMultiplier());
 }
}

static void Reset(){CHECK(liveStrings==0);baseCalls=peeks=getCalls=adds=reads=updates=0;}
int main(){
 int keys[3]={0,0,0};float values[]={0.0f,0.25f,0.5f,1.0f,2.0f};
 for(int bullet=0;bullet<2;++bullet)for(int existing=0;existing<2;++existing)
 for(int duplicate=0;duplicate<2;++duplicate)for(int nullkey=0;nullkey<2;++nullkey)
 for(int v=0;v<5;++v){
  CTCBulletTime time;time.Value=values[v];CTCHaste haste;
  CThingCreatureBase creature;creature.Bullet=bullet?&time:0;creature.Haste=existing?&haste:0;
  void* key=nullkey?0:&keys[0];
  CTCHaste* expected=existing?&haste:&creature.Added;
  expected->Multipliers.insert(std::make_pair((void*)&keys[1],4.0f));
  expected->Multipliers.insert(std::make_pair((void*)&keys[2],8.0f));
  if(duplicate)expected->Multipliers.insert(std::make_pair(key,0.125f));
  CCreatureAction_UnsheatheItemFromInventoryWithHasteAction action;
  action.PCreature.Value=&creature;action.HasteKey=key;
  for(int repeat=0;repeat<2;++repeat){
   Reset();action.FinishAction();
   CHECK(baseCalls==1 && peeks==1 && getCalls==bullet && reads==bullet && updates==bullet);
   CHECK(adds==(bullet && !existing && repeat==0));CHECK(liveStrings==0);
   CHECK(creature.Bullet==(bullet?&time:0));
   CHECK(creature.Haste==(bullet?expected:(existing?&haste:0)));
   CHECK(expected->Multipliers.size()==(unsigned)(2+(duplicate||bullet)));
   CHECK(expected->Multipliers.find(&keys[1])->second==4.0f);
   CHECK(expected->Multipliers.find(&keys[2])->second==8.0f);
   if(duplicate||bullet)CHECK(expected->Multipliers.find(key)->second==(duplicate?0.125f:values[v]));
   else CHECK(expected->Multipliers.find(key)==expected->Multipliers.end());
   time.Value=4.0f; // Repeated insertion must preserve the original contribution.
   CHECK(action.HasteKey==key && action.PCreature.Value==&creature);
  }
 }
 puts("006a02b0_TEST PASS");return 0;
}