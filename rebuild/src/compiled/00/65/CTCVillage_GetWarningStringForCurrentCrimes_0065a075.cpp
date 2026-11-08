// 0065a075 CTCVillage::GetWarningStringForCurrentCrimes
// Leaf: ?GetWarningStringForCurrentCrimes@CTCVillage@@QBE?AVCCharString@@JAA_NAAJ@Z
// Flags: /O1 /Oy-
#include "fable_compiler.h"
#include <string.h>
#pragma intrinsic(strcmp)
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefPointer.h"
#include "engine/CParentDefClassBase.h"
#define CTCVillage CTCVillageFields
#include "engine/CTCVillage.h"
#undef CTCVillage
#define CThing CThingFields
#include "engine/_quarantine/CThing.h"
#undef CThing
#define CPlayer CPlayerFields
#include "engine/CPlayer.h"
#undef CPlayer
#define CWorld CWorldFields
#include "engine/CWorld.h"
#undef CWorld
#define COpinionDeedEffectsDef COpinionDeedEffectsDefFields
#include "engine/_quarantine/COpinionDeedEffectsDef.h"
#undef COpinionDeedEffectsDef

template<class T>class CBasicString{protected:T*PData;unsigned long StringLength;public:__declspec(noinline)bool operator==(const T*s)const{return Compare(PData,s)==0;}protected:static long __fastcall Compare(const T*a,const T*b){while(*a||*b){if(*a<*b)return -1;if(*a>*b)return 1;++a;++b;}return 0;}};
// Layout: only original string-data Data prefix is read; complete allocation metadata is not materialized.
class CCharStringData{public:CBasicString<char>Data;};
class CCharString{protected:CCharStringData*PStringData;public:CCharString();CCharString(const CCharString&);~CCharString();const CCharString&operator=(const CCharString&);const CCharString&operator=(const char*);FABLE_FORCEINLINE bool operator==(const char*s)const{if(!PStringData){if(strcmp(s,"")==0)return true;return false;}return PStringData->Data==s;}};
namespace std{
template<class T>class allocator{};
template<class V,class T,class A>class _STLP_alloc_proxy:public A{public:V _M_data;};
template<class A,class B>struct pair{typedef A first_type;typedef B second_type;A first;B second;};
template<class A,class R>struct unary_function{};template<class A,class B,class R>struct binary_function{};
template<class T>struct less:binary_function<T,T,bool>{bool operator()(const T&a,const T&b)const{return a<b;}};
template<class T>struct _Identity:unary_function<T,T>{const T&operator()(const T&x)const{return x;}};
struct _Rb_tree_node_base{bool _M_color;_Rb_tree_node_base*_M_parent,*_M_left,*_M_right;};
template<class T>struct _Rb_tree_node:public _Rb_tree_node_base{T _M_value_field;};
template<class T>struct _Const_traits{typedef const T&reference;typedef const T*pointer;};
template<class T>struct _Nonconst_traits{typedef T&reference;typedef T*pointer;};
struct _Rb_tree_base_iterator{_Rb_tree_node_base*_M_node;bool operator!=(const _Rb_tree_base_iterator&x)const{return _M_node!=x._M_node;}};
template<class T>class _Rb_global{public:static _Rb_tree_node_base*__cdecl _M_increment(_Rb_tree_node_base*);};
template<class T,class Traits>struct _Rb_tree_iterator:public _Rb_tree_base_iterator{_Rb_tree_iterator(){_M_node=0;}_Rb_tree_iterator(_Rb_tree_node<T>*p){_M_node=p;}_Rb_tree_iterator(const _Rb_tree_iterator<T,_Nonconst_traits<T> >&x){_M_node=x._M_node;}typename Traits::reference operator*()const{return static_cast<_Rb_tree_node<T>*>(_M_node)->_M_value_field;}_Rb_tree_iterator&operator++(){_M_node=_Rb_global<bool>::_M_increment(_M_node);return *this;}};
template<class V,class A>struct _Rb_tree_base{protected:_STLP_alloc_proxy<_Rb_tree_node<V>*,_Rb_tree_node<V>,allocator<_Rb_tree_node<V> > >_M_header;};
template<class K,class V,class Select,class Compare,class A>class _Rb_tree:public _Rb_tree_base<V,A>{protected:unsigned int _M_node_count;Compare _M_key_compare;public:typedef _Rb_tree_iterator<V,_Const_traits<V> >const_iterator;const_iterator begin()const{return const_iterator(static_cast<_Rb_tree_node<V>*>(this->_M_header._M_data->_M_left));}const_iterator end()const{return const_iterator(this->_M_header._M_data);}};
template<class K,class C=less<K>,class A=allocator<K> >class set{_Rb_tree<K,K,_Identity<K>,C,A>_M_t;public:typedef typename _Rb_tree<K,K,_Identity<K>,C,A>::const_iterator const_iterator;const_iterator begin()const{return _M_t.begin();}const_iterator end()const{return _M_t.end();}};
template<class T,class A>class _Vector_base{protected:T*_M_start,*_M_finish;_STLP_alloc_proxy<T*,T,A>_M_end_of_storage;};
template<class T,class A=allocator<T> >class vector:public _Vector_base<T,A>{public:unsigned int size()const{return this->_M_finish-this->_M_start;}const T&operator[](unsigned int n)const{return this->_M_start[n];}T*begin(){return this->_M_start;}T*end(){return this->_M_finish;}};
template<unsigned int N>struct _Base_bitset{unsigned long _M_w[N];};
template<unsigned int N>class bitset:public _Base_bitset<(N+31)/32>{public:bool test(unsigned int)const;};
}
template<class T>class CArray:public std::vector<T>{};
template<class K,class V>class CKeyPairCompareLess{public:bool operator()(const std::pair<K,V>&a,const std::pair<K,V>&b)const{return a.first<b.first;}};
class CTCBase;enum ETCInterfaceType{};
template<class K,class V,class C=CKeyPairCompareLess<K,V> >class CVectorMap:public CArray<std::pair<K,V> >{C Compare;bool Dirty;public:std::pair<K,V>*LowerBound(const K&);};
class CTCHeroOpinionDeedLog;
// Layout: imported bitset20 and Components44 retain the original nonconst GetTC template.
class CThing:public CThingFields{public:template<class T>FABLE_FORCEINLINE bool GetTC(T**out){ETCInterfaceType type=T::StatGetInterfaceType();if(reinterpret_cast<const std::bitset<274>&>(UsingComponents).test(static_cast<unsigned int>(type))){CVectorMap<ETCInterfaceType,CTCBase*>&map=reinterpret_cast<CVectorMap<ETCInterfaceType,CTCBase*>&>(Components);ETCInterfaceType key=type;std::pair<ETCInterfaceType,CTCBase*>*i=map.LowerBound(key);std::pair<ETCInterfaceType,CTCBase*>*result=map.end();if(i!=result&&!(type<i->first))result=i;*out=reinterpret_cast<T*>(result->second);return true;}return false;}};
class CThingPlayerCreature:public CThing{};
class CPlayer:public CPlayerFields{public:CThingPlayerCreature*GetPControlledCreature()const;};
class CWorld:public CWorldFields{public:const CPlayer*PeekMyPlayer()const;};
namespace boost{template<class T>class scoped_ptr{T*px;public:T&operator*()const{return *px;}};}
// Layout: original PWorld24 and static singleton identity independently proven by raw TPI and native callers.
class CMainGameComponent{protected:static CMainGameComponent*PMainGameComponent;unsigned char _pad_0[0x24];boost::scoped_ptr<CWorld>PWorld;public:static CMainGameComponent*__fastcall Get(){return PMainGameComponent;}CWorld&GetWorld(){return *PWorld;}};
enum EOpinionDeedType{};
class COpinionDeed{public:EOpinionDeedType DeedType;};
// Layout: original parent definition39 followed by fields; WarningText64 and original bool bitfield7c.
class COpinionDeedEffectsDef:public CParentDefClassBase{unsigned char _pad_0x39[0x64-0x39];public:std::vector<CCharString>WarningText;unsigned char _pad_0x70[12];bool CanBeHeard:1,ReactionRestrictToTarget:1,AllowSimultaneousReactionDialogue:1,CanApologise:1,MustNotRunWhenWarned:1,OnlyGetFinedForOneInstance:1,IsFlirting:1,OnReactSetGreeted:1;};
class CTCHeroOpinionDeedLog{public:static ETCInterfaceType __fastcall StatGetInterfaceType(){return static_cast<ETCInterfaceType>(0x51);}const COpinionDeed*GetDeedFromHandle(long)const;CDefPointer<const COpinionDeedEffectsDef>PeekPDeedEffectsDef(EOpinionDeedType)const;};
class CTCVillage:public CTCVillageFields{private:using CTCVillageFields::PlayersCrimes;public:CCharString GetWarningStringForCurrentCrimes(long,bool&,long&)const;};
CCharString CTCVillage::GetWarningStringForCurrentCrimes(long index,bool&severe,long&deedHandle)const{
 CCharString warning;severe=false;deedHandle=-1;
 CThingPlayerCreature*hero=CMainGameComponent::Get()->GetWorld().PeekMyPlayer()->GetPControlledCreature();
 if(hero){CTCHeroOpinionDeedLog*log;hero->GetTC(&log);
 const std::set<long>&crimes=reinterpret_cast<const std::set<long>&>(PlayersCrimes);
 std::set<long>::const_iterator last=crimes.end();
 for(std::set<long>::const_iterator it=crimes.begin();it!=last;++it){const COpinionDeed*deed=log->GetDeedFromHandle(*it);
 if(deed){CDefPointer<const COpinionDeedEffectsDef>effects=log->PeekPDeedEffectsDef(deed->DeedType);
 if(index>=0&&static_cast<unsigned int>(index)<effects->WarningText.size()){deedHandle=*it;warning=effects->WarningText[index];if(effects->MustNotRunWhenWarned)severe=true;}else warning="";
 if(warning==""){severe=false;deedHandle=-1;break;}
 }
 }
 }
 return warning;
}