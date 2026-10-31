// 007e4530 CTCWillResponse::IsThingLightningable
// Codex parked-50 best attempt: DIFFER(159v159); residue: 159/159 bytes; final CTCTargeted pointer is loaded into EDX instead of retail EAX.
// Flags:
// Leaf: ?IsThingLightningable@CTCWillResponse@@SI_NPBVCThing@@@Z
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
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefPointer.h"
enum ETCInterfaceType { };
namespace std {
template<class T>class allocator { };
template<class V,class T,class A>class _STLP_alloc_proxy:public A { public:V _M_data; };
template<class T,class A>class _Vector_base { public:T* _M_start;T* _M_finish;_STLP_alloc_proxy<T*,T,A> _M_end_of_storage; };
template<class T,class A=allocator<T> >class vector:public _Vector_base<T,A> { };
template<class K,class V>struct pair { K first;V second; };
template<unsigned long N>struct _Base_bitset { unsigned long _M_w[N]; };
template<unsigned long N>class bitset:public _Base_bitset<(N+31)/32> { };
}
template<class T>class CArray:public std::vector<T> { };
template<class K,class V>class CKeyPairCompareLess { };
template<class K,class V,class C=CKeyPairCompareLess<K,V> >class CVectorMap:public CArray<std::pair<K,V> > {
private:
    C Compare;bool Dirty;
public:
    const std::pair<K,V>* LowerBound(const K&)const;
    const std::pair<K,V>* Find(const K& key)const {
        const std::pair<K,V>* i=LowerBound(key);
        if(i==this->_M_finish || key<i->first)return this->_M_finish;
        return i;
    }
};
// Fidelity: explicit specialization is visible before the caller, unlike a delayed template body.
template<> __declspec(noinline) const std::pair<ETCInterfaceType,CTCBase*>*
CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*> >::LowerBound(const ETCInterfaceType&key)const{
 long count=this->_M_finish-this->_M_start;
 const std::pair<ETCInterfaceType,CTCBase*>*first=this->_M_start;
 while(count>0){long half=count/2;const std::pair<ETCInterfaceType,CTCBase*>*middle=first+half;
  if(middle->first<key){first=middle+1;count-=half+1;}else count=half;
 }
 return first;
}

#pragma pack(push,1)
class CClass:public CBaseObject{unsigned char unreadClass[sizeof(CClassLayout)-sizeof(CBaseObject)];};
// Layout: native bitset20, map44, type flags6c, full thing98 from shared header.
class CThing:public CClass{
protected:
 unsigned char unreadIdentity[0x20-sizeof(CClass)];std::bitset<274>UsingComponents;
 CVectorMap<ETCInterfaceType,CTCBase*>Components;
 unsigned char unreadComponents[0x6c-0x54];unsigned long ThingTypeFlags;
 unsigned char unreadThingTail[sizeof(CThingLayout)-0x70];
public:unsigned long GetTypeFlags()const{return ThingTypeFlags;}
 bool HasTC(ETCInterfaceType t)const{if(UsingComponents._M_w[t>>5]&(1UL<<(t&31)))return true;return false;}
 __forceinline const CTCBase&GetTC(ETCInterfaceType t)const{return *Components.Find(t)->second;}
};
#pragma pack(pop)
class CWillResponseDef;
class CTCTargeted:public CTCBase{
 unsigned long TargetingFlags;
public:unsigned long GetTargetingFlags()const{return TargetingFlags;}
};
// Layout: true private def handle0c and bool bitfields10 from raw PDB/native.
class CTCWillResponse:public CTCBase{
 CDefPointer<const CWillResponseDef>PDef;
 bool IsForcePushable:1;bool IsLifeDrainable:1;bool IsLightningable:1;bool IsNotBloodyWillable:1;
 bool IsAbleToBeLifted:1;bool IsEpicPowerExplodable:1;bool IsEpicSpellable:1;
 bool IsThingLightningable()const{if(IsNotBloodyWillable)return false;return IsLightningable;}
public:static bool __fastcall IsThingLightningable(const CThing*);
};
bool __fastcall CTCWillResponse::IsThingLightningable(const CThing*thing){
 if(thing->HasTC((ETCInterfaceType)0x9c))return static_cast<const CTCWillResponse&>(thing->GetTC((ETCInterfaceType)0x9c)).IsThingLightningable();
 if(thing->HasTC((ETCInterfaceType)0x4e))return true;
 if(thing->HasTC((ETCInterfaceType)9)){
  const CTCTargeted&targeted=static_cast<const CTCTargeted&>(thing->GetTC((ETCInterfaceType)9));
  if(targeted.GetTargetingFlags()&0x18)return true;
 }
 if(thing->GetTypeFlags()&8)return true;
 return false;
}
