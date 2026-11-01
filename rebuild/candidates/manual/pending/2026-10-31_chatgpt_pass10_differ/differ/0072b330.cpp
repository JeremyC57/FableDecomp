// 0072b330 CTCSpecialAbilities::SetupSummonSpell
// Leaf: ?SetupSummonSpell@CTCSpecialAbilities@@AAE_NXZ
// Flags:
#define CThing CThingFields
#include "engine/_quarantine/CThing.h"
#undef CThing
#define CThingCreatureBase CThingCreatureBaseFields
#include "engine/CThingCreatureBase.h"
#undef CThingCreatureBase
#define CThingPhysical CThingPhysicalFields
#include "engine/CThingPhysical.h"
#undef CThingPhysical
#define CThingGameObject CThingGameObjectFields
#include "engine/CThingGameObject.h"
#undef CThingGameObject
#include "engine/CTCBase.h"
class CCharStringData;class CCharString{protected:CCharStringData*PStringData;public:CCharString(const char*,long=-1);~CCharString();};
class CTCParamsBase;
enum ETCInterfaceType{};enum EHeroAbility{};
namespace std{
template<class T>class allocator{};
template<class A,class B>struct pair{A first;B second;};
template<class V,class T,class A>class _STLP_alloc_proxy:public A{public:V _M_data;};
template<class T,class A>class _Vector_base{protected:T*_M_start,*_M_finish;_STLP_alloc_proxy<T*,T,A>_M_end_of_storage;};
template<class T,class A=allocator<T> >class vector:public _Vector_base<T,A>{public:typedef T*iterator;iterator begin(){return this->_M_start;}iterator end(){return this->_M_finish;}};
}
template<class T>class CArray:public std::vector<T>{};
template<class K,class V>class CKeyPairCompareLess{};
template<class K,class V,class C=CKeyPairCompareLess<K,V> >class CVectorMap:public CArray<std::pair<K,V> >{private:C Compare;bool Dirty;public:typedef typename std::vector<std::pair<K,V> >::iterator iterator;iterator LowerBound(const K&);};

#pragma auto_inline(off)
template<class K,class V,class C>typename CVectorMap<K,V,C>::iterator CVectorMap<K,V,C>::LowerBound(const K&key){iterator first=this->begin();long count=this->end()-first;while(count>0){long half=count>>1;iterator middle=first+half;if(middle->first<key){first=middle+1;count-=half+1;}else count=half;}return first;}
#pragma auto_inline(on)
class CTCSummonSpell:public CTCBase{public:void ResetMinionPos();static ETCInterfaceType __fastcall StatGetInterfaceType(){return static_cast<ETCInterfaceType>(0x91);}};
// Layout: original CThing storage imported; Components+44 and UsingComponents+20.
class CThing:public CThingFields{public:CTCBase*AddTC(const CCharString&,bool,const CTCParamsBase*);

 bool HasTC(ETCInterfaceType type)const{return (reinterpret_cast<const unsigned long*>(UsingComponents)[static_cast<unsigned int>(type)>>5]&(1UL<<(static_cast<unsigned int>(type)&31)))!=0;}
 __forceinline CTCBase&GetTC(ETCInterfaceType type){CVectorMap<ETCInterfaceType,CTCBase*>&map=reinterpret_cast<CVectorMap<ETCInterfaceType,CTCBase*>&>(Components);std::pair<ETCInterfaceType,CTCBase*>*it=map.LowerBound(type);if(it==map.end()||type<it->first)it=map.end();return *it->second;}
 template<class T>__forceinline bool GetTC(T**out){if(HasTC(T::StatGetInterfaceType())){*out=static_cast<T*>(&GetTC(T::StatGetInterfaceType()));return true;}return false;}

};
// Layout: original three-step thing inheritance; shared flattened headers supply unread tail extents.
class CThingPhysical:public CThing{private:unsigned char unread[sizeof(CThingPhysicalFields)-sizeof(CThing)];};
class CThingGameObject:public CThingPhysical{private:unsigned char unread[sizeof(CThingGameObjectFields)-sizeof(CThingPhysical)];};
class CThingCreatureBase:public CThingGameObject{private:unsigned char unread[sizeof(CThingCreatureBaseFields)-sizeof(CThingGameObject)];};
// Layout: shared CTCBase0xc, original private flags+0xc/+0xd, ability+10, list4-byte storage+14, Creature reference+18.
class CTCSpecialAbilities:public CTCBase{private:bool AbilityMutexLock,UsingBed;EHeroAbility CurrentAbility;unsigned char UndeadFollowers[4];CThingCreatureBase&Creature;bool SetupSummonSpell();};
bool CTCSpecialAbilities::SetupSummonSpell(){CTCSummonSpell*spell;if(static_cast<CThing&>(Creature).GetTC(&spell)){spell->ResetMinionPos();return true;}{CCharString name("CTCSummonSpell");static_cast<CThing&>(Creature).AddTC(name,true,0);}return true;}
