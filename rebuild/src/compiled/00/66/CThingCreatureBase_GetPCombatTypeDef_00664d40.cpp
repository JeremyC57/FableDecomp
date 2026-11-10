// 00664d40 CThingCreatureBase::GetPCombatTypeDef -- component override, creature combat type, or null-type fallback.
// Leaf: ?GetPCombatTypeDef@CThingCreatureBase@@QBE?AV?$CDefPointer@$$CBVCCombatTypeDef@@@@XZ
// Flags: none
// Retail:00664d40,268 bytes.
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHING_MOREACCESS
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_COMBATTYPE
#define FABLE_RETAIL_CCOMBATABILITY
#define FABLE_RETAIL_CCOMBATABILITYDATA
#define FABLE_RETAIL_CTCCOMBAT
#define FABLE_RETAIL_CCOMBATTYPEDEF
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#include "fable_string.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingCreatureDef.h"
#include "engine/CTCCombat.h"
#include "engine/CCombatTypeDef.h"
#include "engine/CGameDefinitionManager.h"
long CThingCreatureBase::GetCombatType()const{return PeekDefinition().CombatTypeDef;}
#ifdef _MSC_VER
#pragma auto_inline(off)
#endif
template<class T>bool CDefinitionManager::GetDef(long global_index,CDefPointer<const T>* def)const
{
 if(global_index>0){
  CDefPointer<const CDefClassBase> pbase_def=GetPDefFromGlobalIndex(global_index);
  if(pbase_def.Get()){
   def->SetDynamicCast(pbase_def);
   return true;
  }
 }
 return false;
}
template<class T>bool CDefinitionManager::GetDef(const CCharString& instantiation_name,CDefPointer<const T>* def)const
{
 CDefPointer<const CDefClassBase> pbase_def=GetPDefFromInstantiationName(instantiation_name);
 if(pbase_def.Get()){
  def->SetDynamicCast(pbase_def);
  return true;
 }
 return false;
}
#ifdef _MSC_VER
#pragma auto_inline(on)
#endif
CDefPointer<const CCombatTypeDef> CThingCreatureBase::GetPCombatTypeDef()const
{
 CDefPointer<const CCombatTypeDef> pdef;
 const CTCCombat* ptc_combat;
 if(PeekTC(&ptc_combat)){
  pdef=ptc_combat->GetPCombatTypeDef();
  if(pdef.Get())return pdef;
 }
 long combat_type=GetCombatType();
 if(combat_type){
  PeekDefinitionManager().GetDef(combat_type,&pdef);
  return pdef;
 }
 PeekDefinitionManager().GetDef("COMBAT_TYPE_NULL",&pdef);
 return pdef;
}