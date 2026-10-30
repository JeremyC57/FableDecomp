// CAIStateGroup_ServeFoodAtTavern::IsTime() -- Check whether the creature village is in its eating period.
// Leaf: ?IsTime@CAIStateGroup_ServeFoodAtTavern@@MAE_NXZ
// Identity: Own RTTI0128aa7c IsTime slot14 and PDB signature; TC35 CTCVillageMember GetPVillage007ee540.00658bb9 is IsEatTime: CVillageDef EatStartTimes retail132/EatDuration144 (donor152/168 reduced by five/four-byte vector changes), not catalog IsCookTime.
#include "engine/CTCBase.h"
#include "engine/CAIBrain.h"
#define CTCVillage CTCVillage_HeaderLayout
#include "engine/CTCVillage.h"
#undef CTCVillage
#define CAIStateGroupBase CAIStateGroupBase_HeaderLayout
#include "engine/CAIStateGroupBase.h"
#undef CAIStateGroupBase
#define CAIStateGroup_ServeFoodAtTavern CAIStateGroup_ServeFoodAtTavern_HeaderLayout
#include "engine/CAIStateGroup_ServeFoodAtTavern.h"
#undef CAIStateGroup_ServeFoodAtTavern
#ifndef _MSC_VER
#define __forceinline inline
#define __declspec(x)
#endif
enum ETCInterfaceType{TC_VILLAGE_MEMBER=35};
namespace std{template<class K,class V>struct pair{K first;V second;};template<class T>class allocator{};template<class V,class T,class A>class _STLP_alloc_proxy:public A{public:V _M_data;};template<class T,class A>class _Vector_base{public:T* _M_start;T* _M_finish;_STLP_alloc_proxy<T*,T,A> _M_end_of_storage;};template<class T,class A=allocator<T> >class vector:public _Vector_base<T,A>{public:const T* begin()const{return this->_M_start;}const T* end()const{return this->_M_finish;}};}
template<class T>class CArray:public std::vector<T>{};template<class K,class V>class CKeyPairCompareLess{};
template<class K,class V,class Cmp=CKeyPairCompareLess<K,V> >class CVectorMap{public:typedef std::pair<K,V> value_type;typedef const value_type* const_iterator;CArray<value_type> Data;Cmp Compare;bool Dirty;__declspec(noinline) const_iterator LowerBound(const K& key)const{const_iterator first=Data.begin();long count=Data.end()-first;while(count>0){long half=count/2;const_iterator middle=first+half;if(middle->first<key){first=middle+1;count-=half+1;}else count=half;}return first;}V Get(const K key)const{const_iterator p=LowerBound(key);const_iterator last=Data.end();return (p==last || key<p->first?last:p)->second;}};
// Layout: no active CThing header. Original UsingComponents+32 and Components+68; actual const LowerBound overload004365b0.
class CThing{public:unsigned char Prefix[32];unsigned int UsingComponents[9];CVectorMap<ETCInterfaceType,CTCBase*> Components;template<class T>__forceinline bool GetTC(T** out)const{if(!(UsingComponents[1]&8))return false;*out=(T*)Components.Get(TC_VILLAGE_MEMBER);return true;}};class CThingCreatureBase:public CThing{};class CThingAICreature:public CThingCreatureBase{};
class CTCVillage:public CTCVillage_HeaderLayout{public:bool IsEatTime()const;};class CTCVillageMember:public CTCBase{public:CTCVillage* GetPVillage()const;};
// Layout: genuine state-group polymorphism replaces flat header vptr; PBrain+4. Other inherited owner fields are unused.
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CAIStateGroupBase:public CBaseClass{public:CAIBrain* PBrain;unsigned char BaseFields[12];virtual void UnusedSlot01(){};virtual void UnusedSlot02(){};virtual void UnusedSlot03(){};virtual void UnusedSlot04(){};virtual void UnusedSlot05(){};virtual void UnusedSlot06(){};virtual void UnusedSlot07(){};virtual void UnusedSlot08(){};virtual void UnusedSlot09(){};virtual void UnusedSlot10(){};virtual void UnusedSlot11(){};virtual void UnusedSlot12(){};virtual void UnusedSlot13(){};};
class CAIStateGroup_ServeFood:public CAIStateGroupBase{public:unsigned char RemainingFields[56]; protected:virtual bool IsTime()=0;};
class CAIStateGroup_ServeFoodAtTavern:public CAIStateGroup_ServeFood{protected:virtual bool IsTime();friend struct TestAccess;};
bool CAIStateGroup_ServeFoodAtTavern::IsTime(){const CThingAICreature* creature=PBrain->Creature;CTCVillageMember* member;return creature->GetTC(&member) && member->GetPVillage() && member->GetPVillage()->IsEatTime();}