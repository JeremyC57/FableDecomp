// Exact selected action body with controlled component/definition boundaries.
#include <assert.h>
#include <stdio.h>
#include <string.h>
class CTrollBattleDef { public:float GroundPoundSpeedMultiplierStart,GroundPoundSpeedMultiplierEnd;mutable int refs;};
template<class T>class CDefPointer {
 T* Object;
public:CDefPointer():Object(0){}~CDefPointer(){if(Object)--Object->refs;}
 void Reset(T* p){if(Object)--Object->refs;Object=p;if(p)++p->refs;}
 T* operator->()const{return Object;}
};
class CThingBaseDef {public:CTrollBattleDef* value;bool GetSubDef(CDefPointer<const CTrollBattleDef>* out)const{if(!value)return false;out->Reset(value);return true;}};
class CCharString {public:const char* Text;CCharString(const char* t):Text(t){}};
class CTCBase {public:virtual ~CTCBase(){}};
class CTCHaste:public CTCBase {public:void* key;float speed;int count;void SetActionSpeedMultiplier(void* p,float x){key=p;speed=x;++count;}};
class CThingCreatureBase {
public:CThingBaseDef def;CTCHaste* present;CTCHaste* add_result;float health;int adds,health_reads,base_calls;
 const CThingBaseDef& PeekBaseDef()const{assert(base_calls==1);return def;}
 bool GetTC(CTCHaste** out){if(present){*out=present;return true;}return false;}
 CTCBase* AddTC(const CCharString& name,bool activate,const void* params){assert(strcmp(name.Text,"CTCHaste")==0&&!activate&&!params);++adds;return add_result;}
 float GetHealthPercentage(){++health_reads;return health;}
};
class CCombatActionBase {protected:CThingCreatureBase* PCreature;public:CCombatActionBase(CThingCreatureBase* p):PCreature(p){}virtual void Initialise(){++PCreature->base_calls;}};
class CCreatureAction_TrollGroundPound:public CCombatActionBase {public:CCreatureAction_TrollGroundPound(CThingCreatureBase* p):CCombatActionBase(p){}virtual void Initialise();};
void CCreatureAction_TrollGroundPound::Initialise(){
 CCombatActionBase::Initialise();
 CDefPointer<const CTrollBattleDef> ptroll_battle_def;
 if(PCreature->PeekBaseDef().GetSubDef(&ptroll_battle_def)){
  CTCHaste* ptc_haste;
  if(!PCreature->GetTC(&ptc_haste)){
   ptc_haste=static_cast<CTCHaste*>(PCreature->AddTC("CTCHaste",false,0));
  }
  if(ptc_haste){
   float speed_multiplier=ptroll_battle_def->GroundPoundSpeedMultiplierEnd-ptroll_battle_def->GroundPoundSpeedMultiplierStart;
   ptc_haste->SetActionSpeedMultiplier(this,ptroll_battle_def->GroundPoundSpeedMultiplierStart+speed_multiplier*(1.0f-PCreature->GetHealthPercentage()));
  }
 }
}

int main(){
 int cases=0;
 const float starts[4]={2.0f,6.0f,0.0f,-2.0f};
 const float ends[4]={6.0f,2.0f,0.0f,2.0f};
 for(int range_index=0;range_index<4;++range_index)
 for(int has_def=0;has_def<2;++has_def)for(int existing=0;existing<2;++existing)for(int add_ok=0;add_ok<2;++add_ok)for(int q=-1;q<=5;++q){
  CTrollBattleDef def;def.GroundPoundSpeedMultiplierStart=starts[range_index];def.GroundPoundSpeedMultiplierEnd=ends[range_index];def.refs=1;
  CTCHaste haste;haste.key=0;haste.speed=-999;haste.count=0;
  CThingCreatureBase creature;creature.def.value=has_def?&def:0;creature.present=existing?&haste:0;creature.add_result=add_ok?&haste:0;creature.health=(float)q*0.25f;creature.adds=creature.health_reads=creature.base_calls=0;
  CCreatureAction_TrollGroundPound action(&creature);action.Initialise();
  assert(creature.base_calls==1&&def.refs==1);
  bool effect=has_def&&(existing||add_ok);
  assert(haste.count==(effect?1:0));assert(creature.health_reads==(effect?1:0));
  assert(creature.adds==(has_def&&!existing?1:0));
  if(effect){assert(haste.key==&action);assert(haste.speed==starts[range_index]*(float)q*0.25f+ends[range_index]*(1.0f-(float)q*0.25f));}
  else assert(haste.key==0&&haste.speed==-999);
  ++cases;
 }
 assert(cases==224);puts("008c7280_TEST PASS");return 0;
}