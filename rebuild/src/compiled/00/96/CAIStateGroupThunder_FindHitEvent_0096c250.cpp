// CAIStateGroup_Thunder::FindHitEvent() -- Prefer a ranged hit, otherwise find a melee hit.
// Leaf: ?FindHitEvent@CAIStateGroup_Thunder@@IAE_NXZ
// Identity: Own owner bIsRangeAttack offset, original PDB protected FindHitEvent; helper953000 calls IsHitByShot00810f70,953090 IsHitByMelee00810f80; actual const event/filter template types, not duplicate manifest labels.
#include "engine/CTCBase.h"
#include "engine/CAIBrain.h"
#define CAIStateGroupBase CAIStateGroupBase_HeaderLayout
#include "engine/CAIStateGroupBase.h"
#undef CAIStateGroupBase
#define CTCEntityEvents CTCEntityEvents_HeaderLayout
#include "engine/CTCEntityEvents.h"
#undef CTCEntityEvents
#define CAIStateGroup_Thunder CAIStateGroup_Thunder_HeaderLayout
#include "engine/CAIStateGroup_Thunder.h"
#undef CAIStateGroup_Thunder
#ifndef _MSC_VER
#define __forceinline inline
#define __declspec(x)
#endif
enum ETCInterfaceType{TC_ENTITY_EVENTS=66};
namespace std{template<class K,class V>struct pair{K first;V second;};template<class T>class allocator{};template<class V,class T,class A>class _STLP_alloc_proxy:public A{public:V _M_data;};template<class T,class A>class _Vector_base{public:T* _M_start;T* _M_finish;_STLP_alloc_proxy<T*,T,A> _M_end_of_storage;};template<class T,class A=allocator<T> >class vector:public _Vector_base<T,A>{public:const T* begin()const{return this->_M_start;}const T* end()const{return this->_M_finish;}};}
template<class T>class CArray:public std::vector<T>{};template<class K,class V>class CKeyPairCompareLess{};
template<class K,class V,class Cmp=CKeyPairCompareLess<K,V> >class CVectorMap{public:typedef std::pair<K,V> value_type;typedef const value_type* const_iterator;CArray<value_type> Data;Cmp Compare;bool Dirty;__declspec(noinline) const_iterator LowerBound(const K& key)const{const_iterator first=Data.begin();long count=Data.end()-first;while(count>0){long half=count/2;const_iterator middle=first+half;if(middle->first<key){first=middle+1;count-=half+1;}else count=half;}return first;}V Get(const K key)const{const_iterator p=LowerBound(key);const_iterator last=Data.end();return (p==last || key<p->first?last:p)->second;}};
// Layout: no active CThing header. Original UsingComponents+32 and Components+68; actual const LowerBound overload004365b0.
class CThing{public:unsigned char Prefix[32];unsigned int UsingComponents[9];CVectorMap<ETCInterfaceType,CTCBase*> Components;template<class T>__forceinline bool GetTC(T** out)const{if(!(UsingComponents[2]&4))return false;*out=(T*)Components.Get(TC_ENTITY_EVENTS);return true;}};class CThingCreatureBase:public CThing{};class CThingAICreature:public CThingCreatureBase{};
namespace NEntityEvents{class CEventHitBy;class CIsHitEventHitByShot{public:CIsHitEventHitByShot(){}};class CIsHitEventHitByMelee{public:CIsHitEventHitByMelee(){}};}
class CTCEntityEvents:public CTCEntityEvents_HeaderLayout{public:template<class Event,class Filter>bool GetEvent(Event**,const Filter&)const;};
// Layout: active owner and base/brain headers retained; inherited PBrain+4 projected through shared CAIStateGroupBase layout. Helpers953000/953090 are Shot/Melee const CEventHitBy instantiations.
class CAIStateGroup_Thunder:public CAIStateGroup_Thunder_HeaderLayout{protected:bool FindHitEvent();friend struct TestAccess;};
bool CAIStateGroup_Thunder::FindHitEvent(){bIsRangeAttack=false;const CThingAICreature* creature=reinterpret_cast<CAIStateGroupBase_HeaderLayout*>(this)->PBrain->Creature;CTCEntityEvents* events;if(creature->GetTC(&events)){const NEntityEvents::CEventHitBy* event;if(events->GetEvent(&event,NEntityEvents::CIsHitEventHitByShot())){bIsRangeAttack=true;return true;}if(events->GetEvent(&event,NEntityEvents::CIsHitEventHitByMelee()))return true;}return false;}