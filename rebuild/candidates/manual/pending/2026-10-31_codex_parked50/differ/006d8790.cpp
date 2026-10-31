// 006d8790 CTCHeroExperience::AddSuccessfulStrikeOfType
// Codex parked-50 best attempt: DIFFER(124v124); residue: 124/124 bytes; tree iterator now uses retail EBX, but timer store/parent reload and subsequent scratch-register allocation still differ.
// Flags:
// Leaf: ?AddSuccessfulStrikeOfType@CTCHeroExperience@@AAEXW4EDamageAttribute@@M_N@Z
#define CClass CClassLayout
#include "engine/CClass.h"
#undef CClass
#define CThing CThingLayout
#define CTCGraphicAppearance CTCGraphicAppearanceLayout
#include "engine/_quarantine/CThing.h"
#undef CThing
#undef CTCGraphicAppearance
#define CThingPhysical CThingPhysicalLayout
#include "engine/CThingPhysical.h"
#undef CThingPhysical
#define CThingGameObject CThingGameObjectLayout
#include "engine/CThingGameObject.h"
#undef CThingGameObject
#define CThingCreatureBase CThingCreatureBaseLayout
#include "engine/CThingCreatureBase.h"
#undef CThingCreatureBase
#define CThingAICreature CThingAICreatureLayout
#include "engine/CThingAICreature.h"
#undef CThingAICreature

#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"
#define CCreatureActionBase CCreatureActionBaseFields
#include "engine/CCreatureActionBase.h"
#undef CCreatureActionBase
#define CCombatActionBase CCombatActionBaseFields
#include "engine/_quarantine/CCombatActionBase.h"
#undef CCombatActionBase
#include "engine/CBaseClassNonCopyable.h"
#include "engine/CCountedPointer.h"
#define CTCBase CTCBaseFields
#include "engine/CTCBase.h"
#undef CTCBase
class C3DVector;class CCharString;class CTCWeaponTrail;
class CThing;
class CTCBase:public CBaseClassNonCopyable{protected:CThing&ParentThing;private:bool ToBeRemoved,IsInGlobalUpdateTCs;};

#include <math.h>
enum EDamageAttribute{};enum ETCInterfaceType{};
namespace std {
template<class T>class allocator{};
template<class V,class T,class A>class _STLP_alloc_proxy:public A{public:V _M_data;};
template<class T,class A>class _Vector_base{protected:T*_M_start,*_M_finish;_STLP_alloc_proxy<T*,T,A>_M_end_of_storage;};
template<class T,class A=allocator<T> >class vector:public _Vector_base<T,A>{};
template<class A,class B>struct pair{typedef A first_type;A first;B second;};
template<class A,class R>struct unary_function{};
template<class A,class B,class R>struct binary_function{};
template<class T>struct less:public binary_function<T,T,bool>{bool operator()(const T&a,const T&b)const{return a<b;}};
template<class P>struct _Select1st:public unary_function<P,typename P::first_type>{const typename P::first_type&operator()(const P&p)const{return p.first;}};
template<class T>struct _Const_traits{typedef const T*pointer;typedef const T&reference;};
template<class T>struct _Nonconst_traits{typedef T*pointer;typedef T&reference;};
struct _Rb_tree_node_base{bool _M_color;_Rb_tree_node_base*_M_parent,*_M_left,*_M_right;};
template<class T>struct _Rb_tree_node:public _Rb_tree_node_base{T _M_value_field;};
struct _Rb_tree_base_iterator{_Rb_tree_node_base*_M_node;bool operator!=(const _Rb_tree_base_iterator&i)const{return _M_node!=i._M_node;}};
template<class V,class Traits>struct _Rb_tree_iterator:public _Rb_tree_base_iterator{
 _Rb_tree_iterator(_Rb_tree_node<V>*n){_M_node=n;}
 typename Traits::pointer operator->()const{return &((_Rb_tree_node<V>*)_M_node)->_M_value_field;}
};
template<class T,class A>struct _Rb_tree_base{protected:_STLP_alloc_proxy<_Rb_tree_node<T>*,_Rb_tree_node<T>,allocator<_Rb_tree_node<T> > >_M_header;};
template<class K,class V,class Select,class Compare,class A=allocator<V> >class _Rb_tree:public _Rb_tree_base<V,A>{
 unsigned int _M_node_count;Compare _M_key_compare;
public:typedef _Rb_tree_iterator<V,_Const_traits<V> >const_iterator;
 const_iterator end()const{return this->_M_header._M_data;}
 const_iterator find(const K&)const;
};
template<class K,class T,class C=less<K>,class A=allocator<pair<const K,T> > >class map{
 typedef _Rb_tree<K,pair<const K,T>,_Select1st<pair<const K,T> >,C,A>_Rep_type;_Rep_type _M_t;
public:typedef typename _Rep_type::const_iterator const_iterator;
 const_iterator find(const K&k)const{return _M_t.find(k);}const_iterator end()const{return _M_t.end();}
};
// Fidelity: input-reading analysis body for the hidden-result iterator callee.
template<> __declspec(noinline)
_Rb_tree<EDamageAttribute,pair<const EDamageAttribute,float>,_Select1st<pair<const EDamageAttribute,float> >,less<EDamageAttribute> >::const_iterator
_Rb_tree<EDamageAttribute,pair<const EDamageAttribute,float>,_Select1st<pair<const EDamageAttribute,float> >,less<EDamageAttribute> >::find(const EDamageAttribute&key)const{
 typedef _Rb_tree_node<pair<const EDamageAttribute,float> >Node;
 Node*answer=this->_M_header._M_data;Node*node=(Node*)answer->_M_parent;
 while(node){if(node->_M_value_field.first<key)node=(Node*)node->_M_right;else{answer=node;node=(Node*)node->_M_left;}}
 if(answer==this->_M_header._M_data||key<answer->_M_value_field.first)return end();return answer;
}
template<unsigned long N>struct _Base_bitset{unsigned long _M_w[N];};
template<unsigned long N>class bitset:public _Base_bitset<(N+31)/32>{};
}
#pragma pack(push,1)
class CClass:public CBaseObject{unsigned char unreadClass[sizeof(CClassLayout)-sizeof(CBaseObject)];};
// Layout: CThing original UsingComponents begins at20; shared header gives physical prefix.
class CThing:public CClass{
 unsigned char unreadIdentity[0x20-sizeof(CClass)];std::bitset<274>UsingComponents;
public:bool HasTC(ETCInterfaceType t)const{if(UsingComponents._M_w[t>>5]&(1UL<<(t&31)))return true;return false;}
};
#pragma pack(pop)
class CPlayerManager;
class CDeviceResetCallback{public:virtual void OnPreDeviceReset()=0;virtual bool OnPostDeviceReset()=0;};
class CGame;
class CGameComponent:public CBaseClassNonCopyable,public CDeviceResetCallback{
public:virtual~CGameComponent();
protected:bool Quit,Running;CGame*Game;
};
namespace boost{template<class T>class scoped_ptr{T*px;public:T&operator*()const{return *px;}};}
class CASoundBank;namespace NGameText{class CDataBank;}
// Layout: original game base16, PSampleBank16, PTextBank20/24, PPlayerManager28.
class CMainGameComponent:public CGameComponent{
protected:static CMainGameComponent*PMainGameComponent;static long ConstantFPS;
 CASoundBank*PSampleBank;CCountedPointer<NGameText::CDataBank>PTextBank;boost::scoped_ptr<CPlayerManager>PPlayerManager;
public:static long __fastcall GetConstantFPS(){return ConstantFPS;}
 static CMainGameComponent*__fastcall Get(){return PMainGameComponent;}
 CPlayerManager&GetPlayerManager(){return *PPlayerManager;}
};

class CComboMultiplierData{public:std::map<EDamageAttribute,float>ComboMultiplier;float ExperienceMultiplierNumSecondsForAttenuation,ExperienceMultiplierParamDecayDeltaPerSecond;};
#define CTCHeroExperience CTCHeroExperienceLayout
#include "engine/CTCHeroExperience.h"
#undef CTCHeroExperience
// Layout: original private vectors18/24, multiplier30, last34, timer38.
class CTCHeroExperience:public CTCBase{
 long FishingLevel,TotalExperiencePoints,ExperiencePointsAvailableToSpend;
 std::vector<long>StatExperiencePoints,TrainableStatLevels;
 float ExperienceMultiplierParam,LastExperienceMultiplierParam;long ExperienceAttenuationTimer;
 const CComboMultiplierData&GetActiveComboMultiplierData()const;
 float GetAttackVariationMultiplier()const;void AddAttackOfTypeToAttackHistory(EDamageAttribute);
 void UpdateComboMultiplier(bool);void AddSuccessfulStrikeOfType(EDamageAttribute,float,bool);
};
void CTCHeroExperience::AddSuccessfulStrikeOfType(EDamageAttribute type,float damage,bool special){
 AddAttackOfTypeToAttackHistory(type);
 const CComboMultiplierData&data=GetActiveComboMultiplierData();
 ExperienceAttenuationTimer=(long)(CMainGameComponent::GetConstantFPS()*data.ExperienceMultiplierNumSecondsForAttenuation);
 if(!ParentThing.HasTC((ETCInterfaceType)0x87)){
  std::map<EDamageAttribute,float>::const_iterator it=data.ComboMultiplier.find(type);
  if(it!=data.ComboMultiplier.end()){
   float magnitude=(float)fabs(damage);
   ExperienceMultiplierParam+=GetAttackVariationMultiplier()*magnitude*it->second;
   UpdateComboMultiplier(special);
  }
 }
}
