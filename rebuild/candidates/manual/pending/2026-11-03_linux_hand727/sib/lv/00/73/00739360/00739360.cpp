// 00739360 CTCCombat::ResetBlockPercentageChance<CCombatAbilityBlockProjectileWeaponAttackDef>
// Leaf: ??$ResetBlockPercentageChance@VCCombatAbilityBlockProjectileWeaponAttackDef@@@CTCCombat@@QAEXXZ
// Flags:
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefPointer.h"
#include "engine/CCountedPointer.h"
#include "engine/CSubDefClassBase.h"
#include "engine/CParentDefClassBase.h"
#define CCombatAbility CCombatAbilityFields
#include "engine/CCombatAbility.h"
#undef CCombatAbility
// Layout: original private timers retain the supplied 8-byte storage.
class CCombatAbility:private CCombatAbilityFields{};
#define CTCCombat CTCCombatFields
#include "engine/CTCCombat.h"
#undef CTCCombat
#define CCombatAbilityData CCombatAbilityDataFields
#include "engine/CCombatAbilityData.h"
#undef CCombatAbilityData
#define CCombatAbilityBlockingData CCombatAbilityBlockingDataFields
#include "engine/CCombatAbilityBlockingData.h"
#undef CCombatAbilityBlockingData
#define CCombatAbilityBlockProjectileWeaponAttackDef CCombatAbilityBlockProjectileWeaponAttackDefFields
#include "engine/_quarantine/CCombatAbilityBlockProjectileWeaponAttackDef.h"
#undef CCombatAbilityBlockProjectileWeaponAttackDef
#define CCombatTypeDef CCombatTypeDefFields
#include "engine/CCombatTypeDef.h"
#undef CCombatTypeDef
#define CDefinitionManager CDefinitionManagerFields
#include "engine/CDefinitionManager.h"
#undef CDefinitionManager
class CCharStringData;class CCharString{protected:CCharStringData*PStringData;public:CCharString(const char*,long=-1);~CCharString();};
class CDefinitionManager:public CDefinitionManagerFields{public:CDefPointer<CDefClassBase>GetPDefFromGlobalIndex(long);};
namespace std{
template<class A,class B>struct pair{typedef A first_type;typedef B second_type;A first;B second;pair():first(),second(){} pair(const A&a,const B&b):first(a),second(b){}};
template<class A,class B,class R>struct binary_function{};
template<class A,class R>struct unary_function{};
template<class T>struct less:binary_function<T,T,bool>{bool operator()(const T&a,const T&b)const{return a<b;}};
template<class P>struct _Select1st:unary_function<P,typename P::first_type>{const typename P::first_type&operator()(const P&p)const{return p.first;}};
template<class T>class allocator{};
template<class V,class T,class A>class _STLP_alloc_proxy:public A{public:V _M_data;};
struct _Rb_tree_node_base{bool _M_color;_Rb_tree_node_base*_M_parent,*_M_left,*_M_right;};
template<class T>struct _Rb_tree_node:public _Rb_tree_node_base{T _M_value_field;};
struct _Rb_tree_base_iterator{_Rb_tree_node_base*_M_node;};
template<class T>struct _Nonconst_traits;
template<class T>struct _Const_traits{typedef const T*pointer;typedef const T&reference;};
template<class T,class Traits>struct _Rb_tree_iterator:public _Rb_tree_base_iterator{_Rb_tree_iterator(){_M_node=0;} _Rb_tree_iterator(_Rb_tree_node<T>*p){_M_node=p;} _Rb_tree_iterator(const _Rb_tree_iterator<T,_Nonconst_traits<T> >&it){_M_node=it._M_node;} typename Traits::pointer operator->()const{return &static_cast<_Rb_tree_node<T>*>(_M_node)->_M_value_field;}};
template<class V,class A>struct _Rb_tree_base{protected:_STLP_alloc_proxy<_Rb_tree_node<V>*,_Rb_tree_node<V>,allocator<_Rb_tree_node<V> > >_M_header;};

template<class T>struct _Nonconst_traits{typedef T*pointer;typedef T&reference;};
template<class B>class _Rb_global{public:static _Rb_tree_node_base*_M_increment(_Rb_tree_node_base*x){if(x->_M_right){x=x->_M_right;while(x->_M_left)x=x->_M_left;}else{_Rb_tree_node_base*y=x->_M_parent;while(x==y->_M_right){x=y;y=y->_M_parent;}if(x->_M_right!=y)x=y;}return x;}};
template<class K,class V,class Select,class Compare,class A>class _Rb_tree:public _Rb_tree_base<V,A>{protected:unsigned long _M_node_count;Compare _M_key_compare;public:typedef _Rb_tree_iterator<V,_Nonconst_traits<V> >iterator;
 __declspec(noinline)iterator lower_bound(const K&k){_Rb_tree_node_base*y=this->_M_header._M_data;_Rb_tree_node_base*x=y->_M_parent;while(x){if(!_M_key_compare(Select()(static_cast<_Rb_tree_node<V>*>(x)->_M_value_field),k)){y=x;x=x->_M_left;}else x=x->_M_right;}return iterator(static_cast<_Rb_tree_node<V>*>(y));}
 __forceinline iterator upper_bound(const K&k){_Rb_tree_node_base*y=this->_M_header._M_data;_Rb_tree_node_base*x=y->_M_parent;while(x){if(_M_key_compare(k,Select()(static_cast<_Rb_tree_node<V>*>(x)->_M_value_field))){y=x;x=x->_M_left;}else x=x->_M_right;}return iterator(static_cast<_Rb_tree_node<V>*>(y));}
 __forceinline pair<iterator,iterator>equal_range(const K&k){return pair<iterator,iterator>(lower_bound(k),upper_bound(k));}
};
template<class K,class V,class C=less<K>,class A=allocator<pair<const K,V> > >class multimap{private:typedef pair<const K,V>value_type;typedef _Rb_tree<K,value_type,_Select1st<value_type>,C,A>_Rep_type;_Rep_type _M_t;public:typedef typename _Rep_type::iterator iterator;__forceinline pair<iterator,iterator>equal_range(const K&k){return _M_t.equal_range(k);}__forceinline iterator lower_bound(const K&k){return _M_t.lower_bound(k);}__forceinline iterator upper_bound(const K&k){return _M_t.upper_bound(k);}};
}

enum ECombatAbilityDataType{};enum ECombatAbilityInterfaceType{};enum EWeaponClass{};
class CMemoryDataInputStream;class CDataOutputStream;class CDefString{private:long TablePos;};
// Layout: shared flattened CCombatAbilityData restored to its four original virtual slots; member offsets4..14.
class CCombatAbilityData{public:virtual ~CCombatAbilityData();long PercentageChance;float SecondsDuration,SecondsToWaitBeforeRepeat;CDefString Anim;private:ECombatAbilityInterfaceType InterfaceType;public:virtual void TransferBinaryIn(CMemoryDataInputStream&,CCombatAbilityData&);virtual void TransferBinaryOut(CDataOutputStream&,const CCombatAbilityData&)const;virtual ECombatAbilityDataType GetCombatAbilityDataType()const;};
// Layout: shared blocking data has WeaponClass2c after the 12-byte set at20.
class CCombatAbilityBlockingData:public CCombatAbilityData{public:float BlockAngle;bool BlockCanBeBroken,ForceRecoilWhenBlocking;private:unsigned char _pad_0x1e[2];public:unsigned char ValidBlockWeaponTypes[12];private:EWeaponClass WeaponClass;public:static ECombatAbilityDataType __fastcall StatGetCombatAbilityDataType(){return static_cast<ECombatAbilityDataType>(1);}EWeaponClass GetWeaponClass()const{return WeaponClass;}};
// Layout: shared quarantined def58: original CSubDefClassBase25, aggregate aligned28, WeaponClass54.
class CCombatAbilityBlockDefBase:public CSubDefClassBase{public:CCombatAbilityBlockingData CombatAbility;};
class CCombatAbilityBlockProjectileWeaponAttackDef:public CCombatAbilityBlockDefBase{public:static CCharString __fastcall StatGetTypeName(){return CCharString("CCombatAbilityBlockProjectileWeaponAttackDef");}};
class CCombatTypeDef:public CParentDefClassBase{};
template<class T>__declspec(noinline)bool CDefClassBase::GetSubDef(CDefPointer<const T>*out)const{
 const CSubDefInfo*info=GetPSubDefInfo(T::StatGetTypeName());if(!info)return false;CDefPointer<const T>value(PDefManager->GetPDefFromGlobalIndex(info->DefIndex));out->SetDynamicCast(value);return true;
}
// Layout: original shared field PCombatTypeDef28 and CombatAbilities70 are projected to their original template types.
class CTCCombat:public CTCCombatFields{private:using CTCCombatFields::PCombatTypeDef_Object;using CTCCombatFields::CombatAbilities;public:CDefPointer<const CCombatTypeDef>GetPCombatTypeDef()const{return reinterpret_cast<const CDefPointer<const CCombatTypeDef>&>(PCombatTypeDef_Object);}template<class T>void ResetBlockPercentageChance();};
template<class T>void CTCCombat::ResetBlockPercentageChance(){
 CDefPointer<const T>def;bool found=GetPCombatTypeDef()->GetSubDef(&def);
 if(found){EWeaponClass weaponClass=def->CombatAbility.GetWeaponClass();
 typedef std::multimap<ECombatAbilityDataType,std::pair<CCountedPointer<CCombatAbilityData>,CCombatAbility> > TMap;
 TMap::iterator last=reinterpret_cast<TMap&>(CombatAbilities).upper_bound(CCombatAbilityBlockingData::StatGetCombatAbilityDataType());
 TMap::iterator first=reinterpret_cast<TMap&>(CombatAbilities).lower_bound(CCombatAbilityBlockingData::StatGetCombatAbilityDataType());
 std::pair<TMap::iterator,TMap::iterator>range(first,last);
 for(TMap::iterator it=range.first;it._M_node!=range.second._M_node;it._M_node=std::_Rb_global<bool>::_M_increment(it._M_node)){
 CCombatAbilityData*data=it->second.first.Get();
 if(data->GetCombatAbilityDataType()==1&&static_cast<CCombatAbilityBlockingData*>(data)->GetWeaponClass()==weaponClass)data->PercentageChance=def->CombatAbility.PercentageChance;
 }
 }
}
template void CTCCombat::ResetBlockPercentageChance<CCombatAbilityBlockProjectileWeaponAttackDef>();