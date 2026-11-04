// 008559e0 CCreatureAction_BlockBroken::Initialise
// Leaf: ?Initialise@CCreatureAction_BlockBroken@@UAEXXZ
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Flags:
// Layout: existing shared layouts provide native storage offsets; typed base projections preserve the vptr.
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

#include "engine/CVectorMap.h"
#include "engine/CBaseClass.h"
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"
#include "engine/CBaseClassNonCopyable.h"
class CClass:public CBaseObject {private:unsigned char unobservedClassState[sizeof(CClassLayout)-sizeof(CBaseObject)];};
enum ETCInterfaceType {};
class CTCBase;
class CThing:public CClass {
public:

    bool HasTC(ETCInterfaceType type)const {
        const unsigned long*bits=reinterpret_cast<const unsigned long*>(reinterpret_cast<const CThingLayout*>(this)->UsingComponents);
        if(bits[(unsigned)type>>5]&(1UL<<((unsigned)type&31)))return true;
        return false;
    }
    template<class T>__forceinline bool GetTC(T**out) {
        const unsigned long*bits=reinterpret_cast<const unsigned long*>(reinterpret_cast<const CThingLayout*>(this)->UsingComponents);
        if(bits[T::StatGetInterfaceType()>>5]&(1UL<<(T::StatGetInterfaceType()&31))) {
            ETCInterfaceType type=T::StatGetInterfaceType();
            CVectorMap<ETCInterfaceType,CTCBase*>&map=*reinterpret_cast<CVectorMap<ETCInterfaceType,CTCBase*>*>(reinterpret_cast<CThingLayout*>(this)->Components);
            std::pair<ETCInterfaceType,CTCBase*>*it=map.LowerBound(type);
            if(it==map.end()||T::StatGetInterfaceType()<it->first)it=map.end();
            *out=(T*)it->second;return true;
        }
        return false;
    }
    bool IsDead()const{return (reinterpret_cast<const unsigned char*>(this)[0x91]&1)!=0;}
    unsigned long GetTypeFlags()const{return reinterpret_cast<const CThingLayout*>(this)->ThingTypeFlags;}
private:unsigned char unobservedThingState[sizeof(CThingLayout)-sizeof(CClass)];
};
class CThingPhysical:public CThing {private:unsigned char unobservedPhysicalState[sizeof(CThingPhysicalLayout)-sizeof(CThing)];};
class CThingGameObject:public CThingPhysical {private:unsigned char unobservedGameObjectState[sizeof(CThingGameObjectLayout)-sizeof(CThingPhysical)];};
enum ECombatAnimationHandedness {};
class CThingCreatureBase:public CThingGameObject {public:
 ECombatAnimationHandedness GetLastActionHandedness()const{return *reinterpret_cast<const ECombatAnimationHandedness*>(reinterpret_cast<const CThingCreatureBaseLayout*>(this)->PreviousActionHandedness);}
private:unsigned char unobservedCreatureState[sizeof(CThingCreatureBaseLayout)-sizeof(CThingGameObject)];};

#define CCreatureActionBase CCreatureActionBaseFields
#include "engine/CCreatureActionBase.h"
#undef CCreatureActionBase
#define CCombatActionBase CCombatActionBaseFields
#include "engine/_quarantine/CCombatActionBase.h"
#undef CCombatActionBase
#define CCreatureAction_BlockBroken CCreatureAction_BlockBrokenFields
#include "engine/_quarantine/CCreatureAction_BlockBroken.h"
#undef CCreatureAction_BlockBroken
#define CTCCombat CTCCombatFields
#include "engine/CTCCombat.h"
#undef CTCCombat
#define CTCBase CTCBaseFields
#include "engine/CTCBase.h"
#undef CTCBase
#define CWorld CWorldFields
#include "engine/CWorld.h"
#undef CWorld
class CWorld:public CWorldFields {public:unsigned long*GetWorldSeed()const;};
class CBaseObjectPointer;
class CBaseIntelligentPointer {public:virtual~CBaseIntelligentPointer();CBaseObject*GetPItem()const;private:CBaseObjectPointer*PData;};
template<class T>class CIntelligentPointer:public CBaseIntelligentPointer {public:T*Get()const{return static_cast<T*>(GetPItem());}};
class CCharStringData;
class CCharString {protected:CCharStringData*PStringData;public:CCharString(const char*,long=-1);~CCharString();};
long __fastcall GFROR13(unsigned long);
__forceinline unsigned long __fastcall GFRandom(unsigned long max,unsigned long*seed){*seed=*seed*0x24a1+0x24df;*seed=GFROR13(*seed);return max?*seed%max:0;}
class CTCBase:public CBaseClassNonCopyable{private:unsigned char unobservedTCState[sizeof(CTCBaseFields)-sizeof(CBaseClassNonCopyable)];};
class CTCCombat:public CTCBase {public:
 static ETCInterfaceType __fastcall StatGetInterfaceType(){return (ETCInterfaceType)0x30;}
 void SetBlockBroken(CThing&);
private:unsigned char unobservedCombatState[sizeof(CTCCombatFields)-sizeof(CTCBase)];};
class CCreatureActionBase:public CBaseObject{public:
 virtual void EditUpdate(float);virtual void PerformAction()=0;virtual void FinishAction();virtual bool IsPlayingAnimation()const;
 virtual long GetAnimDelayTime()const;virtual long GetAnimationIndex()const;virtual long GetAnimationTransitionInTime()const;virtual long GetAnimationTransitionOutTime()const;
 virtual unsigned long GetAnimExtraFlags()const;virtual CCreatureActionBase*Clone()const=0;virtual void Initialise();
protected:CWorld&GetWorld();CIntelligentPointer<CThingCreatureBase>PCreature;
private:unsigned char unobservedActionState[sizeof(CCreatureActionBaseFields)-sizeof(CBaseObject)-sizeof(CIntelligentPointer<CThingCreatureBase>)];};
class CCombatActionBase:public CCreatureActionBase{public:virtual void Initialise();void SetCombatAnimation(const CCharString&);
protected:CIntelligentPointer<CThing>PTarget;
private:unsigned char unobservedCombatActionState[sizeof(CCombatActionBaseFields)-sizeof(CCreatureActionBase)-sizeof(CIntelligentPointer<CThing>)];};
class CCreatureAction_BlockBroken:public CCombatActionBase{public:virtual void Initialise();};
#define IS_THING_ALIVE(p) ((p).Get()!=0 && !(p).Get()->IsDead())
void CCreatureAction_BlockBroken::Initialise(){
 long side=GFRandom(2,GetWorld().GetWorldSeed());
 if(IS_THING_ALIVE(PTarget) && (PTarget.Get()->GetTypeFlags()&8))side=static_cast<CThingCreatureBase*>(PTarget.Get())->GetLastActionHandedness();
 SetCombatAnimation(side==0?"BLOCK_BROKEN_RESPONSE_RIGHT":"BLOCK_BROKEN_RESPONSE_LEFT");
 if(IS_THING_ALIVE(PTarget)){
  CTCCombat*combat;
  if(PCreature.Get()->GetTC(&combat))combat->SetBlockBroken(*PTarget.Get());
 }
 CCombatActionBase::Initialise();
}