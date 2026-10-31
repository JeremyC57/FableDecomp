// 0085fb30 NCreatureMode::IsAbleToBeAttacked
// Flags:
// Leaf: ?IsAbleToBeAttacked@NCreatureMode@@YI_NABVCThing@@@Z
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
class CCreatureActionBase:public CBaseObject{
 unsigned char unreadAction[0xa8-8];
public:
virtual void EditUpdate(float);
virtual void PerformAction();
virtual void FinishAction();
virtual bool IsPlayingAnimation()const;
virtual long GetAnimDelayTime()const;
virtual long GetAnimationIndex()const;
virtual long GetAnimationTransitionInTime()const;
virtual long GetAnimationTransitionOutTime()const;
virtual unsigned long GetAnimExtraFlags()const;
virtual CCreatureActionBase* Clone()const;
virtual void Initialise();
virtual bool IsStrikeEvent(long)const;
virtual bool IsStrikeEventActive()const;
virtual bool IsStrikeAction()const;
virtual bool IsPartOfComboChain()const;
virtual bool IsStrikeEventFinishedBeingActive()const;
virtual bool IsVitalStrike()const;
virtual bool IsKnockdownStrike()const;
virtual bool IsBlockCounter()const;
virtual bool IsMeleeFlourish()const;
virtual bool IsActionWithStrikeEventHelperName()const;
virtual CCharString GetStrikeEventHelperName()const;
virtual bool IsOverrideableByActionOfSamePriority()const;
virtual void OnFrameUpdate(float);
virtual bool IsQueuable()const;
virtual bool IsActionReadyForInterruption()const;
virtual bool IsActionReadyForInterruptionByMovement(const C3DVector&)const;
virtual bool IsBlockable()const;
virtual bool IsAffectedByHaste()const;
virtual bool IsAffectedByHeroStats()const;
virtual bool IsPhysicsPushableByHero()const;
virtual bool IsAlwaysPhysicsPushableBySelfAvoidance()const;
virtual bool IsAlwaysUseAnimationMovement()const;
virtual bool IsClearingMovementVector()const;
virtual bool IsToHaveEmptyHands()const;
virtual bool IsToReplaceItemIfRemoved()const;
virtual bool IsAllowingAnimationMovementToMoveSpeakAndWaitCameras()const;
virtual bool IsPreventingQuickAccessItemUsage()const;
virtual void SetHitter(const CThing&);
virtual void SetHitDirection(C3DVector);
virtual void SetHitPosition(C3DVector);
virtual CTCWeaponTrail* GetWeaponTrailUsedInStrikeAction(long&)const;
virtual float GetTotalTimeSeconds()const;
virtual bool IsToEvadeAllHits()const;
virtual bool IsToCauseRecoil()const;
virtual bool IsAbleToBeAttacked()const;
};
class CThing;
class CTCBase:public CBaseClassNonCopyable{protected:CThing&ParentThing;private:bool ToBeRemoved,IsInGlobalUpdateTCs;};
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
// Layout: native physical bitfieldbc has EvadeAllHits at bit3.
class CThingPhysical:public CThing{
 unsigned char unreadPhysicalPrefix[0xbc-sizeof(CThing)];
protected:unsigned char unreadPhysicalFlags:3;unsigned char EvadeAllHits:1;unsigned char unreadRemainingFlags:4;
private:unsigned char unreadPhysicalTail[sizeof(CThingPhysicalLayout)-0xbd];
public:bool IsToEvadeAllHits()const{return EvadeAllHits!=0;}
};
class CThingGameObject:public CThingPhysical{unsigned char unreadGameObject[sizeof(CThingGameObjectLayout)-sizeof(CThingPhysical)];};
// Layout: native protected PCurrentAction100 is a genuine counted pointer8.
class CThingCreatureBase:public CThingGameObject{
 unsigned char unreadCreaturePrefix[0x100-sizeof(CThingGameObject)];
protected:CCountedPointer<CCreatureActionBase>PCurrentAction;
private:unsigned char unreadCreatureTail[sizeof(CThingCreatureBaseLayout)-0x108];
public:const CCreatureActionBase*PeekPCurrentAction()const{return PCurrentAction.Get();}
};
#pragma pack(pop)
class CTCCreatureModeManager:public CTCBase{
public:class CModeBase{public:virtual~CModeBase();virtual unsigned long GetType()const;private:unsigned char unreadMode[24];};
 CModeBase&GetCurrentMode()const;bool IsModeActive(unsigned long)const;
};
namespace NCreatureMode{
bool __fastcall IsKnockedDown(const CThing&);bool __fastcall IsUnderground(const CThing&);
}
namespace NCreatureMode{bool __fastcall IsAbleToBeAttacked(const CThing&thing){
 if(thing.GetTypeFlags()&8){const CCreatureActionBase*action=static_cast<const CThingCreatureBase&>(thing).PeekPCurrentAction();if(action&&!action->IsAbleToBeAttacked())return false;}
 if(IsKnockedDown(thing))return false;
 if(IsUnderground(thing))return false;
 if(thing.HasTC((ETCInterfaceType)0x31)){
  unsigned long mode=static_cast<const CTCCreatureModeManager&>(thing.GetTC((ETCInterfaceType)0x31)).GetCurrentMode().GetType();
  if(mode==0x45)return false;
  if(mode==0x51)return false;
 }
 return true;
}}