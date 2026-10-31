// 00809030 CThingFilter_IsInTargetingRangeAndFOV::CThingFilter_IsInTargetingRangeAndFOV
// Codex parked-50 best attempt: DIFFER(169v166); residue: 169/166 bytes; original reference members and targeting interface corrected; vector-copy LEA/store ordering and component lookup scheduling remain different.
// Flags:
// Leaf: ??0CThingFilter_IsInTargetingRangeAndFOV@
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
class CCharStringData;
class CCharString {CCharStringData*PStringData;public:CCharString(const char*,long=-1);~CCharString();};
#include "engine/CBaseClassNonCopyable.h"
class CThing;class CTCParamsBase;
class C3DVector {public:float X,Y,Z;C3DVector(const C3DVector&v):X(v.X),Y(v.Y),Z(v.Z){}C3DVector&Normalise();};
class C2DVector;class CRightHandedSet;class CWorld;enum ETCInterfaceType{};
class CPersistContext;class CEnginePrimitivePrepareInfo;namespace NTCMessages{class CMessageBase;}
class CThingManager;
class CTCBase:public CBaseClassNonCopyable{
protected:virtual void OnSerialise(CPersistContext&,bool);CWorld&GetWorld()const;CThingManager&GetThingManager();
public:
    virtual void OnRemove();
    virtual void ConstructFromParams(const CTCParamsBase&);
    virtual void OnCreate();virtual void InitialActivate();virtual void OnInitialActivate();
    virtual void RenderUpdate(float);virtual bool HasFrameUpdate()const;virtual void FrameUpdate();virtual void BulletTimeFrameUpdate();
    virtual void DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo&);
    virtual void Copy(const CTCBase&);virtual void OnDie();virtual void OnKill();virtual void OnLevelUnload();virtual void OnSetInLimbo(bool);
    virtual bool Validate(CCharString&)const;virtual bool IsFreeForDeletion()const;virtual bool IsSerialiseCreatable()const;virtual bool IsSerialisableForHeroState()const;
    virtual ETCInterfaceType GetInterfaceType()const=0;virtual CCharString GetClassTypeName()const=0;
    virtual bool IsComponentSerialisable()const;virtual bool IsDriver()const;virtual bool IsCopyable()const;virtual void GlobalUpdate(bool);
    virtual void HandleMessage(const NTCMessages::CMessageBase&);
protected:CThing&ParentThing;
private:bool ToBeRemoved,IsInGlobalUpdateTCs;unsigned char unobservedAlignment[2];
};

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
 bool HasTC(ETCInterfaceType t)const{return (UsingComponents._M_w[t>>5]&(1UL<<(t&31)))!=0;}
 __forceinline const CTCBase&GetTC(ETCInterfaceType t)const{return *Components.Find(t)->second;}
};
#pragma pack(pop)

enum ECreatureInteractionType{};class CThingCreatureBase;
// Layout: targeting interface extends28-slot CTCBase; GetPTargetLockedThing slot36.
class CTCTargetingBase:public CTCBase{
public:virtual CThing*GetPBestThingForMeleeAttack()const;
 virtual CThingCreatureBase*GetPBestEnemyCreatureForAttack()const;
 virtual CThing*GetPBestThingForProjectileTargetingLock()const;
 virtual CThingCreatureBase*GetPBestCreatureForInteractionTypeWithinRangeLimit(ECreatureInteractionType,float,float)const;
 virtual CThingCreatureBase*GetPBestCreatureForInteractionType(ECreatureInteractionType)const;
 virtual CThing*GetPTargetedThing()const;virtual void SetPTargetedThing(CThing*);
 virtual CThing*GetPProjectileTargetedThing()const;virtual CThing*GetPTargetLockedThing()const;
};
class CTargetingDef;
namespace {
// Layout: raw PDB48 bytes; Position0 FacingVector12 Targeter reference24, lock28,
// TargetingDef reference32, carried36, FOV40 and 3D flag44.
class CThingFilter_IsInTargetingRangeAndFOV{
 C3DVector Position,FacingVector;const CThing&Targeter;const CThing*PTargetLockThing;
 const CTargetingDef&TargetingDef;const CThing*PCarriedThing;float TargetingFOV;bool AngleTest3D;
public:CThingFilter_IsInTargetingRangeAndFOV(const C3DVector&,const C3DVector&,const CThing&,const CTargetingDef&,const CThing*,float,bool);
};
CThingFilter_IsInTargetingRangeAndFOV::CThingFilter_IsInTargetingRangeAndFOV(const C3DVector&position,const C3DVector&facing,const CThing&targeter,const CTargetingDef&def,const CThing*carried,float fov,bool angle3D)
 :Position(position),FacingVector(facing),Targeter(targeter),PTargetLockThing(0),TargetingDef(def),PCarriedThing(carried),TargetingFOV(fov),AngleTest3D(angle3D){
 if(!AngleTest3D)FacingVector.Z=0;
 FacingVector.Normalise();
 if(targeter.HasTC((ETCInterfaceType)8))PTargetLockThing=static_cast<const CTCTargetingBase&>(targeter.GetTC((ETCInterfaceType)8)).GetPTargetLockedThing();
}
}
