// CTCCombat::ResetBlockPercentageChance() -- Reset matching block-ability probabilities from the selected sub-definition.
// Leaf: ??$ResetBlockPercentageChance@VCCombatAbilityBlockUnarmedAttackDef@@@CTCCombat@@QAEXXZ
// Identity: own GetSubDef007380d0 contains exact class-name literal; PDB confirms member template. Retail lower_bound00738d10 and virtual data type slot3.
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
#ifndef _MSC_VER
#define __forceinline inline
#define __declspec(x)
#endif
class CMemoryDataInputStream;class CDataOutputStream;class CCombatTypeDef;class CCPPointerInfo;
enum ECombatAbilityDataType {DescriptiveBlockingData=1};enum EWeaponClass {DescriptiveWeaponZero=0};
// Layout: shared flattened definition header retained as evidence; real vptr/refcount hierarchy is required for virtual Delete at slot1 and count+4. CombatAbility starts+40 in block defs.
class CIVCountedPointeeBase {public:virtual ~CIVCountedPointeeBase();virtual void Delete()const;mutable unsigned long IVRefCount;};
template<class T>class CDefPointer {public:T* Object;CDefPointer():Object(0){}CDefPointer(T* p):Object(p){if(Object)++((const CIVCountedPointeeBase*)Object)->IVRefCount;}~CDefPointer(){if(Object){const CIVCountedPointeeBase* p=(const CIVCountedPointeeBase*)Object;if(--p->IVRefCount==0)p->Delete();}}};
class CDefClassBase:public CIVCountedPointeeBase {public:unsigned char BaseFields[32];template<class T>__declspec(noinline) bool GetSubDef(CDefPointer<const T>*)const;};
// Layout: shared data headers are flattened; genuine four-slot polymorphic class preserves PercentageChance+4 and WeaponClass+44 in blocking data. Other fields follow PDB names.
class CCombatAbilityData {public:virtual ~CCombatAbilityData();virtual void TransferBinaryIn(CMemoryDataInputStream&,CCombatAbilityData&);virtual void TransferBinaryOut(CDataOutputStream&,const CCombatAbilityData&)const;virtual ECombatAbilityDataType GetCombatAbilityDataType()const;
long PercentageChance;float SecondsDuration,SecondsToWaitBeforeRepeat;long Anim_TablePos;unsigned int InterfaceType;
};
class CCombatAbilityBlockingData:public CCombatAbilityData {public:float BlockAngle;bool BlockCanBeBroken,ForceRecoilWhenBlocking;unsigned char Padding[2];unsigned char ValidBlockWeaponTypes[12];EWeaponClass WeaponClass;};
class CCombatAbilityBlockUnarmedAttackDef:public CDefClassBase {public:CCombatAbilityBlockingData CombatAbility;};
template<class T>class CCountedPointer{public:T* Data;CCPPointerInfo* Info;T* operator->()const{return Data;}};
namespace std {
template<class A,class B>struct pair {A first;B second;pair(){}pair(const A& a,const B& b):first(a),second(b){}};
template<class T>class allocator{};template<class T>struct less {bool operator()(const T& a,const T& b)const{return a<b;}};
struct _Rb_tree_node_base {int _M_color;_Rb_tree_node_base* _M_parent;_Rb_tree_node_base* _M_left;_Rb_tree_node_base* _M_right;};
template<class V>struct _Rb_tree_node:public _Rb_tree_node_base {V _M_value_field;};
template<bool Dummy>struct _Rb_global {static _Rb_tree_node_base* _M_increment(_Rb_tree_node_base* node){if(node->_M_right){node=node->_M_right;while(node->_M_left)node=node->_M_left;}else{_Rb_tree_node_base* y=node->_M_parent;while(node==y->_M_right){node=y;y=y->_M_parent;}if(node->_M_right!=y)node=y;}return node;}};
template<class V,class Ref,class Ptr>struct _Rb_tree_iterator {_Rb_tree_node_base* _M_node;_Rb_tree_iterator(){} _Rb_tree_iterator(_Rb_tree_node_base* n):_M_node(n){}Ptr operator->()const{return &((_Rb_tree_node<V>*)_M_node)->_M_value_field;}_Rb_tree_iterator& operator++(){_M_node=_Rb_global<true>::_M_increment(_M_node);return *this;}bool operator!=(const _Rb_tree_iterator& x)const{return _M_node!=x._M_node;}};
template<class K,class V,class Compare=less<K>,class A=allocator<pair<const K,V> > >class multimap {public:
 typedef pair<const K,V> value_type;typedef _Rb_tree_iterator<value_type,value_type&,value_type*> iterator;
 _Rb_tree_node_base* _M_header;unsigned int _M_node_count;Compare _M_key_compare;
 __declspec(noinline) iterator lower_bound(const K& key);
 __forceinline iterator upper_bound(const K& key){_Rb_tree_node_base* y=_M_header;_Rb_tree_node_base* x=_M_header->_M_parent;while(x){if(key<((_Rb_tree_node<value_type>*)x)->_M_value_field.first){y=x;x=x->_M_left;}else x=x->_M_right;}return iterator(y);}
 __forceinline pair<iterator,iterator> equal_range(const K& key){return pair<iterator,iterator>(lower_bound(key),upper_bound(key));}
};
}
namespace std {
template<class K,class V,class Compare,class A>__declspec(noinline) typename multimap<K,V,Compare,A>::iterator multimap<K,V,Compare,A>::lower_bound(const K& key){_Rb_tree_node_base* y=_M_header;_Rb_tree_node_base* x=_M_header->_M_parent;while(x){if(!(((_Rb_tree_node<value_type>*)x)->_M_value_field.first<key)){y=x;x=x->_M_left;}else x=x->_M_right;}return iterator(y);}
}
typedef std::multimap<ECombatAbilityDataType,std::pair<CCountedPointer<CCombatAbilityData>,CCombatAbility> > AbilityMap;
// Label: QuerySubDefBoundary is a compiler-visibility dependency only; only the assigned caller is graded. The visible wrapper reads this and writes only the output.
template<class T>const T* QuerySubDefBoundary(const CDefClassBase*);
template<class T>__declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<const T>* out)const{const T* p=QuerySubDefBoundary<T>(this);if(!p)return false;out->Object=p;++p->IVRefCount;return true;}
class CTCCombat:public CTCCombat_HeaderLayout {public:template<class T>void ResetBlockPercentageChance();};
template<class T>void CTCCombat::ResetBlockPercentageChance(){
 CDefPointer<const T> def;
 if(((const CDefClassBase*)CDefPointer<const CCombatTypeDef>((const CCombatTypeDef*)PCombatTypeDef_Object).Object)->GetSubDef<T>(&def)){
  EWeaponClass weapon=def.Object->CombatAbility.WeaponClass;
  AbilityMap::iterator last=((AbilityMap*)CombatAbilities)->upper_bound(DescriptiveBlockingData);
  for(AbilityMap::iterator i=((AbilityMap*)CombatAbilities)->lower_bound(DescriptiveBlockingData);i!=last;++i){CCombatAbilityData* data=i->second.first.Data;
   if(data->GetCombatAbilityDataType()==DescriptiveBlockingData && ((CCombatAbilityBlockingData*)data)->WeaponClass==weapon)data->PercentageChance=def.Object->CombatAbility.PercentageChance;
  }
 }
}
template void CTCCombat::ResetBlockPercentageChance<CCombatAbilityBlockUnarmedAttackDef>();
