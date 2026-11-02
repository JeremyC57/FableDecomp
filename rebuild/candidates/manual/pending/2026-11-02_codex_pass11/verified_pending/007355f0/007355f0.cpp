// CTCCombat::SetCombatType(CDefPointer<CCombatTypeDef const> const &) -- select combat configuration and rebuild the combat ability collections.
// Leaf: ?SetCombatType@CTCCombat@@QAEXABV?$CDefPointer@$$CBVCCombatTypeDef@@@@@Z
// Flags: /Gr
#define FABLE_RETAIL_COMBAT_TYPE
#define FABLE_STLPORT_CONSTRUCTORS
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_DEFINITION_MANAGER
#include <new>
#include "engine/CTCCombat.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CCombatManager.h"
#include "engine/CCombatTypeDef.h"
#include "engine/CDefinitionManager.h"
#include "engine/CCombatAbilityBlockUnarmedAttackDef.h"
#include "engine/CCombatAbilityBlockLightWeaponAttackDef.h"
#include "engine/CCombatAbilityBlockHeavyWeaponAttackDef.h"
#include "engine/CCombatAbilityBlockProjectileWeaponAttackDef.h"
#include "engine/CCombatAbilityBlockCounterAttackDef.h"
#include "engine/CCombatAbilityFlourishCounterAttackDef.h"
#include "engine/CCombatAbilityGetHitCounterAttackDef.h"
#include "engine/CCombatAbilityStrafeDef.h"
#include "engine/CCombatAbilityUseProjectileWeaponDef.h"

#pragma auto_inline(off)
namespace std {
template<> _Rb_tree<ECombatAbilityDataType,pair<const ECombatAbilityDataType,pair<CCountedPointer<CCombatAbilityData>,CCombatAbility> >,_Select1st<pair<const ECombatAbilityDataType,pair<CCountedPointer<CCombatAbilityData>,CCombatAbility> > >,less<ECombatAbilityDataType>,allocator<pair<const ECombatAbilityDataType,pair<CCountedPointer<CCombatAbilityData>,CCombatAbility> > > >::iterator _Rb_tree<ECombatAbilityDataType,pair<const ECombatAbilityDataType,pair<CCountedPointer<CCombatAbilityData>,CCombatAbility> >,_Select1st<pair<const ECombatAbilityDataType,pair<CCountedPointer<CCombatAbilityData>,CCombatAbility> > >,less<ECombatAbilityDataType>,allocator<pair<const ECombatAbilityDataType,pair<CCountedPointer<CCombatAbilityData>,CCombatAbility> > > >::_M_insert(_Rb_tree_node_base* x,_Rb_tree_node_base* y,const pair<const ECombatAbilityDataType,pair<CCountedPointer<CCombatAbilityData>,CCombatAbility> >& v,_Rb_tree_node_base* w)
{
 _Rb_tree_node<pair<const ECombatAbilityDataType,pair<CCountedPointer<CCombatAbilityData>,CCombatAbility> > >* z;
 if(y==this->_M_header._M_data||(w==0&&(x!=0||_M_key_compare(v.first,static_cast<_Rb_tree_node<pair<const ECombatAbilityDataType,pair<CCountedPointer<CCombatAbilityData>,CCombatAbility> > >*>(y)->_M_value_field.first)))){
  z=this->_M_header.allocate(1);new(&z->_M_value_field)pair<const ECombatAbilityDataType,pair<CCountedPointer<CCombatAbilityData>,CCombatAbility> >(v);
  y->_M_left=z;
  if(y==this->_M_header._M_data){this->_M_header._M_data->_M_parent=z;this->_M_header._M_data->_M_right=z;}
  else if(y==this->_M_header._M_data->_M_left)this->_M_header._M_data->_M_left=z;
 }else{
  z=this->_M_header.allocate(1);new(&z->_M_value_field)pair<const ECombatAbilityDataType,pair<CCountedPointer<CCombatAbilityData>,CCombatAbility> >(v);
  y->_M_right=z;if(y==this->_M_header._M_data->_M_right)this->_M_header._M_data->_M_right=z;
 }
 z->_M_parent=y;z->_M_left=0;z->_M_right=0;
 _Rb_global_inst::_Rebalance(z,this->_M_header._M_data->_M_parent);++_M_node_count;return iterator(z);
}
template<> _Rb_tree<ECombatAbilityDataType,pair<const ECombatAbilityDataType,pair<CCountedPointer<CCombatAbilityData>,CCombatAbility> >,_Select1st<pair<const ECombatAbilityDataType,pair<CCountedPointer<CCombatAbilityData>,CCombatAbility> > >,less<ECombatAbilityDataType>,allocator<pair<const ECombatAbilityDataType,pair<CCountedPointer<CCombatAbilityData>,CCombatAbility> > > >::iterator _Rb_tree<ECombatAbilityDataType,pair<const ECombatAbilityDataType,pair<CCountedPointer<CCombatAbilityData>,CCombatAbility> >,_Select1st<pair<const ECombatAbilityDataType,pair<CCountedPointer<CCombatAbilityData>,CCombatAbility> > >,less<ECombatAbilityDataType>,allocator<pair<const ECombatAbilityDataType,pair<CCountedPointer<CCombatAbilityData>,CCombatAbility> > > >::insert_equal(const pair<const ECombatAbilityDataType,pair<CCountedPointer<CCombatAbilityData>,CCombatAbility> >& v)
{
 _Rb_tree_node<pair<const ECombatAbilityDataType,pair<CCountedPointer<CCombatAbilityData>,CCombatAbility> > >* y=this->_M_header._M_data;
 _Rb_tree_node<pair<const ECombatAbilityDataType,pair<CCountedPointer<CCombatAbilityData>,CCombatAbility> > >* x=static_cast<_Rb_tree_node<pair<const ECombatAbilityDataType,pair<CCountedPointer<CCombatAbilityData>,CCombatAbility> > >*>(y->_M_parent);
 while(x!=0){y=x;x=static_cast<_Rb_tree_node<pair<const ECombatAbilityDataType,pair<CCountedPointer<CCombatAbilityData>,CCombatAbility> > >*>(_M_key_compare(v.first,x->_M_value_field.first)?x->_M_left:x->_M_right);}
 return _M_insert(x,y,v);
}
}
#pragma auto_inline(on)

#pragma auto_inline(off)
template<> bool CDefClassBase::GetSubDef<CCombatAbilityBlockUnarmedAttackDef>(CDefPointer<const CCombatAbilityBlockUnarmedAttackDef>* out)const {
 const CSubDefInfo* info;
 {CCharString name("CCombatAbilityBlockUnarmedAttackDef",-1);info=GetPSubDefInfo(name);}
 if(info){CDefPointer<const CCombatAbilityBlockUnarmedAttackDef> result(PDefManager->GetPDefFromGlobalIndex(info->DefIndex));*out=result;return true;}
 return false;
}
template<> bool CDefClassBase::GetSubDef<CCombatAbilityBlockLightWeaponAttackDef>(CDefPointer<const CCombatAbilityBlockLightWeaponAttackDef>* out)const {
 const CSubDefInfo* info;
 {CCharString name("CCombatAbilityBlockLightWeaponAttackDef",-1);info=GetPSubDefInfo(name);}
 if(info){CDefPointer<const CCombatAbilityBlockLightWeaponAttackDef> result(PDefManager->GetPDefFromGlobalIndex(info->DefIndex));*out=result;return true;}
 return false;
}
template<> bool CDefClassBase::GetSubDef<CCombatAbilityBlockHeavyWeaponAttackDef>(CDefPointer<const CCombatAbilityBlockHeavyWeaponAttackDef>* out)const {
 const CSubDefInfo* info;
 {CCharString name("CCombatAbilityBlockHeavyWeaponAttackDef",-1);info=GetPSubDefInfo(name);}
 if(info){CDefPointer<const CCombatAbilityBlockHeavyWeaponAttackDef> result(PDefManager->GetPDefFromGlobalIndex(info->DefIndex));*out=result;return true;}
 return false;
}
template<> bool CDefClassBase::GetSubDef<CCombatAbilityBlockProjectileWeaponAttackDef>(CDefPointer<const CCombatAbilityBlockProjectileWeaponAttackDef>* out)const {
 const CSubDefInfo* info;
 {CCharString name("CCombatAbilityBlockProjectileWeaponAttackDef",-1);info=GetPSubDefInfo(name);}
 if(info){CDefPointer<const CCombatAbilityBlockProjectileWeaponAttackDef> result(PDefManager->GetPDefFromGlobalIndex(info->DefIndex));*out=result;return true;}
 return false;
}
template<> bool CDefClassBase::GetSubDef<CCombatAbilityBlockCounterAttackDef>(CDefPointer<const CCombatAbilityBlockCounterAttackDef>* out)const {
 const CSubDefInfo* info;
 {CCharString name("CCombatAbilityBlockCounterAttackDef",-1);info=GetPSubDefInfo(name);}
 if(info){CDefPointer<const CCombatAbilityBlockCounterAttackDef> result(PDefManager->GetPDefFromGlobalIndex(info->DefIndex));*out=result;return true;}
 return false;
}
template<> bool CDefClassBase::GetSubDef<CCombatAbilityFlourishCounterAttackDef>(CDefPointer<const CCombatAbilityFlourishCounterAttackDef>* out)const {
 const CSubDefInfo* info;
 {CCharString name("CCombatAbilityFlourishCounterAttackDef",-1);info=GetPSubDefInfo(name);}
 if(info){CDefPointer<const CCombatAbilityFlourishCounterAttackDef> result(PDefManager->GetPDefFromGlobalIndex(info->DefIndex));*out=result;return true;}
 return false;
}
template<> bool CDefClassBase::GetSubDef<CCombatAbilityGetHitCounterAttackDef>(CDefPointer<const CCombatAbilityGetHitCounterAttackDef>* out)const {
 const CSubDefInfo* info;
 {CCharString name("CCombatAbilityGetHitCounterAttackDef",-1);info=GetPSubDefInfo(name);}
 if(info){CDefPointer<const CCombatAbilityGetHitCounterAttackDef> result(PDefManager->GetPDefFromGlobalIndex(info->DefIndex));*out=result;return true;}
 return false;
}
template<> bool CDefClassBase::GetSubDef<CCombatAbilityStrafeDef>(CDefPointer<const CCombatAbilityStrafeDef>* out)const {
 const CSubDefInfo* info;
 {CCharString name("CCombatAbilityStrafeDef",-1);info=GetPSubDefInfo(name);}
 if(info){CDefPointer<const CCombatAbilityStrafeDef> result(PDefManager->GetPDefFromGlobalIndex(info->DefIndex));*out=result;return true;}
 return false;
}
template<> bool CDefClassBase::GetSubDef<CCombatAbilityUseProjectileWeaponDef>(CDefPointer<const CCombatAbilityUseProjectileWeaponDef>* out)const {
 const CSubDefInfo* info;
 {CCharString name("CCombatAbilityUseProjectileWeaponDef",-1);info=GetPSubDefInfo(name);}
 if(info){CDefPointer<const CCombatAbilityUseProjectileWeaponDef> result(PDefManager->GetPDefFromGlobalIndex(info->DefIndex));*out=result;return true;}
 return false;
}
#pragma auto_inline(on)

void CTCCombat::SetCombatType(const CDefPointer<const CCombatTypeDef>& pcombat_type)
{
 const CCombatTypeDef& combat_type=*pcombat_type;
 bool using_xombat_wheel=true;
 CCombatManager& combat_man=GetCombatManager();
 CThingCreatureBase& parent_creature=GetParentCreature();
 CThingCreatureBase* attacked=0;
 if(ParentCreature->IsInMelee()&&ParentCreature->IsInMeleeAsAttacker()){
  const CCombatantInfo& info=combat_man.GetCombatantInfo(parent_creature);
  using_xombat_wheel=info.AttackerInfo.UsingCombatWheel;
  attacked=info.AttackerInfo.Attacked;
  combat_man.DeregisterCombatant(parent_creature);
 }
 PCombatTypeDef=pcombat_type;
 if(attacked&&!attacked->IsDead())combat_man.RegisterCombatant(parent_creature,*attacked,using_xombat_wheel);
 CombatAbilities.clear();
 {
  CDefPointer<const CCombatAbilityBlockUnarmedAttackDef> pdef;
  if(combat_type.GetSubDef(&pdef)){
   CCountedPointer<CCombatAbilityData> ability(new CCombatAbilityBlockingData(pdef->CombatAbility));
   CombatAbilities.insert(std::make_pair(ability->GetCombatAbilityDataType(),std::make_pair(ability,CCombatAbility())));
  }
 }
 {
  CDefPointer<const CCombatAbilityBlockLightWeaponAttackDef> pdef;
  if(combat_type.GetSubDef(&pdef)){
   CCountedPointer<CCombatAbilityData> ability(new CCombatAbilityBlockingData(pdef->CombatAbility));
   CombatAbilities.insert(std::make_pair(ability->GetCombatAbilityDataType(),std::make_pair(ability,CCombatAbility())));
  }
 }
 {
  CDefPointer<const CCombatAbilityBlockHeavyWeaponAttackDef> pdef;
  if(combat_type.GetSubDef(&pdef)){
   CCountedPointer<CCombatAbilityData> ability(new CCombatAbilityBlockingData(pdef->CombatAbility));
   CombatAbilities.insert(std::make_pair(ability->GetCombatAbilityDataType(),std::make_pair(ability,CCombatAbility())));
  }
 }
 {
  CDefPointer<const CCombatAbilityBlockProjectileWeaponAttackDef> pdef;
  if(combat_type.GetSubDef(&pdef)){
   CCountedPointer<CCombatAbilityData> ability(new CCombatAbilityBlockingData(pdef->CombatAbility));
   CombatAbilities.insert(std::make_pair(ability->GetCombatAbilityDataType(),std::make_pair(ability,CCombatAbility())));
  }
 }
 {
  CDefPointer<const CCombatAbilityBlockCounterAttackDef> pdef;
  if(combat_type.GetSubDef(&pdef)){
   CCountedPointer<CCombatAbilityData> ability(new CCombatAbilityMeleeAttackData(pdef->CombatAbility));
   CombatAbilities.insert(std::make_pair(ability->GetCombatAbilityDataType(),std::make_pair(ability,CCombatAbility())));
  }
 }
 {
  CDefPointer<const CCombatAbilityFlourishCounterAttackDef> pdef;
  if(combat_type.GetSubDef(&pdef)){
   CCountedPointer<CCombatAbilityData> ability(new CCombatAbilityMeleeAttackData(pdef->CombatAbility));
   CombatAbilities.insert(std::make_pair(ability->GetCombatAbilityDataType(),std::make_pair(ability,CCombatAbility())));
  }
 }
 {
  CDefPointer<const CCombatAbilityGetHitCounterAttackDef> pdef;
  if(combat_type.GetSubDef(&pdef)){
   CCountedPointer<CCombatAbilityData> ability(new CCombatAbilityMeleeAttackData(pdef->CombatAbility));
   CombatAbilities.insert(std::make_pair(ability->GetCombatAbilityDataType(),std::make_pair(ability,CCombatAbility())));
  }
 }
 {
  CDefPointer<const CCombatAbilityStrafeDef> pdef;
  if(combat_type.GetSubDef(&pdef)){
   CCountedPointer<CCombatAbilityData> ability(new CCombatAbilityStrafingData(pdef->CombatAbility));
   CombatAbilities.insert(std::make_pair(ability->GetCombatAbilityDataType(),std::make_pair(ability,CCombatAbility())));
  }
 }
 {
  CDefPointer<const CCombatAbilityUseProjectileWeaponDef> pdef;
  if(combat_type.GetSubDef(&pdef)){
   CCountedPointer<CCombatAbilityData> ability(new CCombatAbilityProjectileAttackData(pdef->CombatAbility));
   CombatAbilities.insert(std::make_pair(ability->GetCombatAbilityDataType(),std::make_pair(ability,CCombatAbility())));
  }
 }
}
