// CTCCombat::ResetBlockPercentageChance() -- Reset matching block-ability probabilities from the selected sub-definition.
// Leaf: ??$ResetBlockPercentageChance@VCCombatAbilityBlockLightWeaponAttackDef@@@CTCCombat@@QAEXXZ
// Identity: own GetSubDef007381f0 contains exact class-name literal; PDB confirms member template. Retail lower_bound00738d10 and virtual data type slot3.
#define CTCCombat CTCCombat_HeaderLayout
#include "engine/CTCCombat.h"
#undef CTCCombat
#define CDefClassBase CDefClassBase_HeaderLayout
#include "engine/CDefClassBase.h"
#undef CDefClassBase
#define CCombatAbilityData CCombatAbilityData_HeaderLayout
#include "engine/CCombatAbilityData.h"
#undef CCombatAbilityData
#define CCombatAbilityBlockingData CCombatAbilityBlockingData_HeaderLayout
#include "engine/CCombatAbilityBlockingData.h"
#undef CCombatAbilityBlockingData
#define CIVCountedPointeeBase CIVCountedPointeeBase_HeaderLayout
#include "engine/CIVCountedPointeeBase.h"
#undef CIVCountedPointeeBase
#include "engine/CCombatAbility.h"
// Layout: full original RTTI chain CTCCombat/CTCBase/NonCopy/BaseClass; blockdef/BlockDefBase/SubDef/Def/Pointee/Resource/CIV. Typed owner prefix through CombatAbilities112; trailing unused owner/CombatTypeDef state omitted without whole-size claim. Genuine STLport tree/base/proxy, bool color, rebound allocator, two-argument iterator traits and original set. CDefPointer stores CDefPointeeBase*, original Release/AddRef contract. Original shared flattened headers remain evidence only.
#ifndef _MSC_VER
#define __forceinline inline
#define __declspec(x)
#endif
class CMemoryDataInputStream;class CDataOutputStream;class CPersistContext;class CDefinitionManager;class CCPPointerInfo;
enum ECombatAbilityDataType{DescriptiveBlockingData=1};enum EWeaponClass{DescriptiveWeaponZero=0};enum ECombatAbilityInterfaceType{DescriptiveInterfaceZero=0};enum EUnsheatheWeaponType{DescriptiveUnsheatheZero=0};
namespace std{
template<class A,class B>struct pair{A first;B second;pair(){}pair(const A&a,const B&b):first(a),second(b){}};template<class T>class allocator{};template<class T>struct less{bool operator()(const T&a,const T&b)const{return a<b;}};template<class T>struct _Select1st{};template<class T>struct _Identity{};template<class T>struct _Nonconst_traits{typedef T& reference;typedef T* pointer;};
template<class V,class T,class A>class _STLP_alloc_proxy:public A{public:V _M_data;};
template<class T,class A>class _Vector_base{protected:T* _M_start;T* _M_finish;_STLP_alloc_proxy<T*,T,A> _M_end_of_storage;};template<class T,class A=allocator<T> >class vector:protected _Vector_base<T,A>{};
struct _Rb_tree_node_base{bool _M_color;_Rb_tree_node_base* _M_parent;_Rb_tree_node_base* _M_left;_Rb_tree_node_base* _M_right;};template<class V>struct _Rb_tree_node:public _Rb_tree_node_base{V _M_value_field;};
template<class Dummy>class _Rb_global{public:static _Rb_tree_node_base* _M_increment(_Rb_tree_node_base* node){if(node->_M_right){node=node->_M_right;while(node->_M_left)node=node->_M_left;}else{_Rb_tree_node_base* y=node->_M_parent;while(node==y->_M_right){node=y;y=y->_M_parent;}if(node->_M_right!=y)node=y;}return node;}};
struct _Rb_tree_base_iterator{_Rb_tree_node_base* _M_node;};template<class V,class Traits>struct _Rb_tree_iterator:public _Rb_tree_base_iterator{typedef _Rb_tree_iterator<V,_Nonconst_traits<V> > iterator;typedef typename Traits::pointer pointer;typedef _Rb_tree_node<V>* _Link_type;_Rb_tree_iterator(){}_Rb_tree_iterator(_Link_type n){_M_node=n;}_Rb_tree_iterator(const iterator& i){_M_node=i._M_node;}pointer operator->()const{return &((_Rb_tree_node<V>*)_M_node)->_M_value_field;}_Rb_tree_iterator& operator++(){_M_node=_Rb_global<bool>::_M_increment(_M_node);return *this;}bool operator!=(const _Rb_tree_iterator&x)const{return _M_node!=x._M_node;}};
template<class V,class A>class _Rb_tree_base{protected:_STLP_alloc_proxy<_Rb_tree_node<V>*,_Rb_tree_node<V>,allocator<_Rb_tree_node<V> > > _M_header;};
template<class K,class V,class Select,class Compare,class A>class _Rb_tree:protected _Rb_tree_base<V,A>{public:typedef _Rb_tree_iterator<V,_Nonconst_traits<V> > iterator;unsigned int _M_node_count;Compare _M_key_compare;
__declspec(noinline) iterator lower_bound(const K& key){_Rb_tree_node_base* y=this->_M_header._M_data;_Rb_tree_node_base* x=y->_M_parent;while(x){if(!(((_Rb_tree_node<V>*)x)->_M_value_field.first<key)){y=x;x=x->_M_left;}else x=x->_M_right;}return iterator((_Rb_tree_node<V>*)y);}
__forceinline iterator upper_bound(const K& key){_Rb_tree_node_base* y=this->_M_header._M_data;_Rb_tree_node_base* x=y->_M_parent;while(x){if(key<((_Rb_tree_node<V>*)x)->_M_value_field.first){y=x;x=x->_M_left;}else x=x->_M_right;}return iterator((_Rb_tree_node<V>*)y);}};
template<class K,class V,class Compare=less<K>,class A=allocator<pair<const K,V> > >class multimap{public:typedef pair<const K,V> value_type;typedef _Rb_tree<K,value_type,_Select1st<value_type>,Compare,A> tree_type;typedef typename tree_type::iterator iterator;private:tree_type _M_t;public:__forceinline iterator lower_bound(const K& key){return _M_t.lower_bound(key);}__forceinline iterator upper_bound(const K& key){return _M_t.upper_bound(key);}};
template<class K,class Compare=less<K>,class A=allocator<K> >class set{private:_Rb_tree<K,K,_Identity<K>,Compare,A> _M_t;};
}
class CIVCountedPointeeBase{public:CIVCountedPointeeBase():IVRefCount(1){}virtual ~CIVCountedPointeeBase(){}virtual void Delete()const;mutable unsigned long IVRefCount;void AddRef()const{++IVRefCount;}long GetRefCount()const{return (long)IVRefCount;}long Release()const{long result=(long)--IVRefCount;if(result==0)Delete();return result;}};
class CResourceList;class CResource:public CIVCountedPointeeBase{public:CResourceList* ResourceList;CResource* PrevResource;CResource* NextResource;unsigned long ResourceSize,LastUsedFrame;CResource():ResourceList(0),PrevResource(0),NextResource(0),ResourceSize(0),LastUsedFrame(0){}virtual ~CResource();};class CDefPointeeBase:public CResource{public:virtual void Delete()const;};enum ENullDef{ENullDef_DescriptiveNull=0};
template<class T>class CDefPointer{public:CDefPointeeBase* Object;CDefPointer(ENullDef=ENullDef_DescriptiveNull):Object(0){}CDefPointer(const CDefPointer& other):Object(other.Object){if(Object)Object->AddRef();}CDefPointer(T* object):Object((CDefPointeeBase*)object){if(Object)Object->AddRef();}__forceinline ~CDefPointer(){if(Object){Object->Release();Object=0;}}CDefPointer& operator=(const CDefPointer& other){if(Object!=other.Object){if(Object){Object->Release();Object=0;}Object=other.Object;if(Object)Object->AddRef();}return *this;}T* operator->()const{return (T*)Object;}};

class CDefString{public:long TablePos;};template<class T>class CArray:public std::vector<T>{};template<class K,class V>class CKeyPairCompareLess{};template<class K,class V,class C=CKeyPairCompareLess<K,V> >class CVectorMap{public:CArray<std::pair<K,V> > Data;C Compare;bool Dirty;};
#pragma pack(push,1)
class CDefClassBase:public CDefPointeeBase{public:class CSubDefInfo;CDefinitionManager* PDefManager;unsigned long GlobalIndex;unsigned char Setup:1;unsigned char Template:1;unsigned char DefaultValsApplied:1;private:friend class CSubDefClassBase;friend class CParentDefClassBase;CDefClassBase(){}public:virtual void UnknownDefSlot2(){}virtual void UnknownDefSlot3(){}virtual void UnknownDefSlot4(){}virtual void UnknownDefSlot5(){}virtual void UnknownDefSlot6(){}virtual void UnknownDefSlot7(){}virtual void UnknownDefSlot8(){}virtual void UnknownDefSlot9(){}virtual void UnknownDefSlot10(){}virtual void UnknownDefSlot11(){}virtual void UnknownDefSlot12(){}virtual void UnknownDefSlot13(){}virtual void UnknownDefSlot14(){}virtual void UnknownDefSlot15(){}virtual void UnknownDefSlot16(){}virtual void UnknownDefSlot17(){}virtual void UnknownDefSlot18(){}virtual void UnknownDefSlot19(){}virtual void UnknownDefSlot20(){} template<class T>__declspec(noinline) bool GetSubDef(CDefPointer<const T>*)const;};class CSubDefClassBase:public CDefClassBase{};class CParentDefClassBase:public CDefClassBase{public:CDefString InstantiationName;CVectorMap<unsigned long,CDefClassBase::CSubDefInfo> SubDefInfoMap;};
#pragma pack(pop)
class CCombatTypeDef:public CParentDefClassBase{};
class CCombatAbilityData{public:virtual ~CCombatAbilityData();virtual void TransferBinaryIn(CMemoryDataInputStream&,CCombatAbilityData&);virtual void TransferBinaryOut(CDataOutputStream&,const CCombatAbilityData&)const;virtual ECombatAbilityDataType GetCombatAbilityDataType()const;long PercentageChance;float SecondsDuration,SecondsToWaitBeforeRepeat;CDefString Anim;ECombatAbilityInterfaceType InterfaceType;};
class CCombatAbilityBlockingData:public CCombatAbilityData{public:float BlockAngle;bool BlockCanBeBroken,ForceRecoilWhenBlocking;std::set<EWeaponClass> ValidBlockWeaponTypes;EWeaponClass WeaponClass;};class CCombatAbilityBlockDefBase:public CSubDefClassBase{public:CCombatAbilityBlockingData CombatAbility;};class CCombatAbilityBlockLightWeaponAttackDef:public CCombatAbilityBlockDefBase{};
template<class T>class CCountedPointer{public:T* Data;CCPPointerInfo* Info;T* operator->()const{return Data;}};
typedef std::multimap<ECombatAbilityDataType,std::pair<CCountedPointer<CCombatAbilityData>,CCombatAbility> > AbilityMap;
template<class T>const T* QuerySubDefBoundary(const CDefClassBase*);
template<class T>__declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<const T>* out)const{const T* p=QuerySubDefBoundary<T>(this);if(!p)return false;out->Object=(CDefPointeeBase*)p;p->AddRef();return true;}
class CBaseClass{public:virtual ~CBaseClass(){}};class CBaseClassNonCopyable:public CBaseClass{protected:CBaseClassNonCopyable(){}~CBaseClassNonCopyable(){}private:CBaseClassNonCopyable(const CBaseClassNonCopyable&);CBaseClassNonCopyable& operator=(const CBaseClassNonCopyable&);};class CThing;class CTCBase:public CBaseClassNonCopyable{public:CThing* ParentThing;bool ToBeRemoved,IsInGlobalUpdateTCs;};class CCombatManager;class CThingCreatureBase;class CMeleeCombatAbilityDef;class CBaseObject;struct CBaseObjectPointer;class CBaseIntelligentPointer{public:CBaseObjectPointer* PData;};template<class T>class CIntelligentPointer:public CBaseIntelligentPointer{};
class CTCCombat:public CTCBase{public:long ComboChainPos,ComboChainID,ComboChainGracePeriodCountdown,HandednessGracePeriodCountdown,UnarmedCombatIdleCountdown;CCombatManager* CombatManager;CThingCreatureBase* ParentCreature;CDefPointer<const CCombatTypeDef> PCombatTypeDef;std::vector<CDefPointer<const CMeleeCombatAbilityDef> > AbilityList,UsableAbilityList;long CurrentAbilityIndex;CDefPointer<const CMeleeCombatAbilityDef> PBlockCounterAbility,PFlourish360Ability;long BlockCounterTimer;std::vector<CIntelligentPointer<const CThing> > AlwaysBlockThings;bool PerformingAction,PerformingStrikeAction,StartingToBlock,Blocking;long BlockHoldTimer,BlockingNumFrames;EUnsheatheWeaponType UnsheathingWeaponType;AbilityMap CombatAbilities;template<class T>void ResetBlockPercentageChance();};
template<class T>void CTCCombat::ResetBlockPercentageChance(){CDefPointer<const T> def;if(((const CDefClassBase*)CDefPointer<const CCombatTypeDef>(PCombatTypeDef).Object)->GetSubDef<T>(&def)){EWeaponClass weapon=def->CombatAbility.WeaponClass;AbilityMap::iterator last=CombatAbilities.upper_bound(DescriptiveBlockingData);for(AbilityMap::iterator i=CombatAbilities.lower_bound(DescriptiveBlockingData);i!=last;++i){CCombatAbilityData* data=i->second.first.Data;if(data->GetCombatAbilityDataType()==DescriptiveBlockingData&&((CCombatAbilityBlockingData*)data)->WeaponClass==weapon)data->PercentageChance=def->CombatAbility.PercentageChance;}}}
template void CTCCombat::ResetBlockPercentageChance<CCombatAbilityBlockLightWeaponAttackDef>();
#ifdef _MSC_VER
typedef char ProperCombatMapOffset[offsetof(CTCCombat,CombatAbilities)==112?1:-1];typedef char ProperBlockWeaponOffset[offsetof(CCombatAbilityBlockingData,WeaponClass)==44?1:-1];typedef char ProperDefinitionBase[sizeof(CDefClassBase)==37?1:-1];typedef char ProperBlockDefOffset[offsetof(CCombatAbilityBlockDefBase,CombatAbility)==40?1:-1];
#endif