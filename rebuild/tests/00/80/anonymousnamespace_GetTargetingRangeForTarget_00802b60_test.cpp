#include <assert.h>
#include <math.h>
#include <stdio.h>
#ifndef _MSC_VER
#define __fastcall
#endif
enum ETargetingType{TEST_TARGETING_ONE=1};
const int TCI_ACTION_USE_SCRIPTED_HOOK=193;
static int refs,range_calls,type_calls,manager_calls,getdef_calls,subdef_calls;
static long definition_index;static bool creature_exists,subdef_exists;
struct CTargetingDef {float range;};static CTargetingDef direct_def,fallback_def;
template<class T>struct CDefPointer {T*Object;CDefPointer():Object(0){}CDefPointer(const CDefPointer&b):Object(b.Object){if(Object)++refs;}~CDefPointer(){if(Object)--refs;}bool IsNull()const{return !Object;}T*operator->()const{return Object;}T&operator*()const{return *Object;}};
struct CTCTargeted {bool direct;unsigned long flags;CDefPointer<const CTargetingDef>GetPTargetingDef()const{CDefPointer<const CTargetingDef>p;if(direct){p.Object=&direct_def;++refs;}return p;}unsigned long GetTargetingFlags()const{return flags;}};
struct CTCAIScratchpad {float offset;float GetTargetingDistanceOffset()const{return offset;}};
struct CThing {CTCTargeted targeted;CTCAIScratchpad scratch;bool have_scratch,hook;bool PeekTC(const CTCTargeted**p)const{*p=&targeted;return true;}bool PeekTC(const CTCAIScratchpad**p)const{if(have_scratch)*p=&scratch;return have_scratch;}bool HasTC(int type)const{assert(type==TCI_ACTION_USE_SCRIPTED_HOOK);return hook;}};
struct CThingCreatureDef {bool GetSubDef(CDefPointer<const CTargetingDef>*p)const{++subdef_calls;if(subdef_exists){p->Object=&fallback_def;++refs;}return subdef_exists;}};static CThingCreatureDef creature_def;
struct CGameDefinitionManager {long GetDefGlobalIndexFromName(const char*name)const;bool GetDef(long index,CDefPointer<const CThingCreatureDef>*p)const{assert(index==definition_index);++getdef_calls;if(creature_exists){p->Object=&creature_def;++refs;}return creature_exists;}};
#include <string.h>
long CGameDefinitionManager::GetDefGlobalIndexFromName(const char*name)const{assert(!strcmp(name,"CREATURE_BASE_TEMPLATE"));++manager_calls;return definition_index;}
static CGameDefinitionManager definitions;
struct CMainGameComponent {static CMainGameComponent*Get(){static CMainGameComponent c;return &c;}const CGameDefinitionManager&PeekDefinitionManager()const{return definitions;}};
static const CThing*expected_targeter;static const CThing*expected_carried;
float __fastcall GetTargetingRange(const CTargetingDef&def,ETargetingType type){assert(&def==&fallback_def&&int(type)==1);++range_calls;return def.range;}
float __fastcall GetTargetingRangeForTargetType(unsigned long type,const CTargetingDef&def,const CThing&targeter,const CThing*carried){assert(type==0x431&&&def==&direct_def&&&targeter==expected_targeter&&carried==expected_carried);++type_calls;return def.range;}

float __fastcall GetTargetingRangeForTarget(const CThing& target_thing,const CThing& targeting_thing,const CThing* pcarried_thing)
{
 const CTCTargeted* ptc_targeted;
 target_thing.PeekTC(&ptc_targeted);
 CDefPointer<const CTargetingDef> ptargeting_def=ptc_targeted->GetPTargetingDef();
 if(ptargeting_def.IsNull()) {
  if(target_thing.HasTC(TCI_ACTION_USE_SCRIPTED_HOOK)) {
   const CGameDefinitionManager& def_manager=CMainGameComponent::Get()->PeekDefinitionManager();
   long def_index=def_manager.GetDefGlobalIndexFromName("CREATURE_BASE_TEMPLATE");
   if(def_index>0) {
    CDefPointer<const CThingCreatureDef> pdef;
    def_manager.GetDef(def_index,&pdef);
    if(!pdef.IsNull()) {
     CDefPointer<const CTargetingDef> pcreature_targeting_def;
     if(pdef->GetSubDef(&pcreature_targeting_def))
      return (GetTargetingRange(*pcreature_targeting_def,(ETargetingType)1));
    }
   }
  }
 }else {
  unsigned long type_flags=ptc_targeted->GetTargetingFlags();
  float dist=GetTargetingRangeForTargetType(type_flags,*ptargeting_def,targeting_thing,pcarried_thing);
  const CTCAIScratchpad* tc_ai_scratchpad;
  if(target_thing.PeekTC(&tc_ai_scratchpad))dist+=tc_ai_scratchpad->GetTargetingDistanceOffset();
  return dist;
 }
 return 0.0f;
}
int main(){
 CThing targeter,carried;expected_targeter=&targeter;
 for(int mode=0;mode<8;++mode)for(int carry=0;carry<2;++carry){
  refs=range_calls=type_calls=manager_calls=getdef_calls=subdef_calls=0;
  expected_carried=carry?&carried:0;
  CThing target;target.targeted.direct=mode<3;target.targeted.flags=0x431;target.have_scratch=mode!=0;target.scratch.offset=mode==2?-5.0f:3.0f;target.hook=mode!=3;
  direct_def.range=25.0f;fallback_def.range=17.5f;definition_index=mode==4?0:(mode==5?-4:99);creature_exists=mode!=6;subdef_exists=mode!=7;
  float got=GetTargetingRangeForTarget(target,targeter,expected_carried);
  float wanted=mode<3?25.0f+(mode?target.scratch.offset:0.0f):0.0f;
  assert(got==wanted&&refs==0&&type_calls==(mode<3)&&range_calls==0);
  assert(manager_calls==(mode>=4));assert(getdef_calls==(mode>=6));assert(subdef_calls==(mode==7));
 }
 refs=range_calls=type_calls=manager_calls=getdef_calls=subdef_calls=0;expected_carried=0;
 CThing target;target.targeted.direct=false;target.targeted.flags=0x431;target.have_scratch=true;target.scratch.offset=100;target.hook=true;definition_index=99;creature_exists=subdef_exists=true;
 assert(GetTargetingRangeForTarget(target,targeter,0)==17.5f&&range_calls==1&&type_calls==0&&manager_calls==1&&getdef_calls==1&&subdef_calls==1&&refs==0);
 puts("00802b60_TEST PASS");return 0;
}