// CCreatureAction_TrollGroundPound::Initialise -- apply health-dependent action speed.
// Grade: MASKED_MATCH(268v268); relocation audit and exact-body tests accompany this source.
// Flags:
// Retail: 008c7280,268 bytes,virtual thiscall.
// Leaf: ?Initialise@CCreatureAction_TrollGroundPound@@UAEXXZ
// Complete STLport tree insertion is visible, preserving the native temporary lifetime.
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_PCREATURE
#define FABLE_RETAIL_CCOMBATACTIONBASE_MELEE
#define FABLE_RETAIL_CCOMBATACTIONBASE_INITIALISE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGPHYSICAL_HEALTH_PERCENTAGE
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCHASTE
#define FABLE_RETAIL_CTCHASTE_MAKE_PAIR
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#include "fable_string.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingBaseDef.h"
#include "engine/CDefinitionManager.h"
#include "engine/CTrollBattleDef.h"
#include "engine/CTCHaste.h"
#include "engine/CCreatureAction_TrollGroundPound.h"
inline const CThingBaseDef& CThing::PeekBaseDef()const{return *PBaseDef;}
template<class T> bool CDefClassBase::GetSubDef(CDefPointer<const T>* out)const {
 const CSubDefInfo* info=GetPSubDefInfo(T::StatGetTypeName());
 if(info){
  CDefPointer<const CDefClassBase> def=PDefManager->GetPDefFromGlobalIndex(info->DefIndex);
  out->SetDynamicCast(def);
  return true;
 }
 return false;
}
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