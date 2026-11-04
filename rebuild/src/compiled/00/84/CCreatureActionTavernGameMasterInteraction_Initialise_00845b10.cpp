// 00845b10 CCreatureAction_TavernGameMasterInteraction::Initialise
// Flags:
// Leaf: ?Initialise@CCreatureAction_TavernGameMasterInteraction@@UAEXXZ
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: the empty local std::bitset stand-in is the STLport bitset<274> (36 bytes) of stlport/fable_containers.h, test() declared only under FABLE_RETAIL_STLP_BITSET_TEST_DECL (retail calls 0x00410DE0).
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
class C3DVector {public:float X,Y,Z;};
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

#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#include "engine/CVectorMap.h"
// Fidelity: explicit specialization is visible before the caller, unlike a delayed template body.
template<> __declspec(noinline) std::pair<ETCInterfaceType,CTCBase*>*
CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*> >::LowerBound(const ETCInterfaceType&key){
 long count=this->_M_finish-this->_M_start;
 std::pair<ETCInterfaceType,CTCBase*>*first=this->_M_start;
 while(count>0){long half=count/2;std::pair<ETCInterfaceType,CTCBase*>*middle=first+half;
  if(middle->first<key){first=middle+1;count-=half+1;}else count=half;
 }
 return first;
}


#pragma pack(push,1)
class CClass:public CBaseObject{unsigned char unreadClass[sizeof(CClassLayout)-sizeof(CBaseObject)];};
// Layout: native shared CThing bitset20, map44 and Dead bit91.
class CThing:public CClass{
 unsigned char unreadIdentity[0x20-sizeof(CClass)];std::bitset<274>UsingComponents;
 CVectorMap<ETCInterfaceType,CTCBase*>Components;
 unsigned char unreadThing[0x91-0x54];bool Dead:1;unsigned char unreadFlags:7;
 unsigned char unreadTail[sizeof(CThingLayout)-0x92];
public:bool IsDead()const{return Dead;}
 template<class T>__forceinline bool GetTC(T**p){
  ETCInterfaceType t=T::StatGetInterfaceType();
  if((UsingComponents._M_w[t>>5]&(1UL<<(t&31)))!=0){ETCInterfaceType key=T::StatGetInterfaceType();*p=static_cast<T*>(Components.Find(key)->second);return true;}
  return false;
 }
};
class CThingPhysical:public CThing{unsigned char unreadPhysical[sizeof(CThingPhysicalLayout)-sizeof(CThing)];};
class CThingGameObject:public CThingPhysical{unsigned char unreadObject[sizeof(CThingGameObjectLayout)-sizeof(CThingPhysical)];};
class CThingCreatureBase:public CThingGameObject{unsigned char unreadCreature[sizeof(CThingCreatureBaseLayout)-sizeof(CThingGameObject)];};
#pragma pack(pop)
class CBaseIntelligentPointer {
 CBaseObjectPointer*PData;
public:virtual~CBaseIntelligentPointer();CBaseObject*GetPItem()const;
};
template<class T>class CIntelligentPointer:public CBaseIntelligentPointer {
public:T*Get()const{return static_cast<T*>(GetPItem());}T*operator->()const{return Get();}
};
// Layout: CBaseObject8 then protected intelligent pointer8; native total168.
class CCreatureActionBase:public CBaseObject {
protected:CIntelligentPointer<CThingCreatureBase>PCreature;
private:unsigned char unreadAction[0xa8-16];
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
};

#define CTCTavernGame CTCTavernGameLayout
#include "engine/CTCTavernGame.h"
#undef CTCTavernGame
#define CCreatureAction_TavernGameMasterInteraction CCreatureAction_TavernGameMasterInteractionLayout
#include "engine/CCreatureAction_TavernGameMasterInteraction.h"
#undef CCreatureAction_TavernGameMasterInteraction
class CInputEvent;class CProcessedInput;class CInputProcessTavernGame;
// Layout: original protected UIComponents12, LastRenderTime24, IsBeingPlayed32.
class CTCTavernGame:public CTCBase{
protected:unsigned char unreadGame[0x20-0x0c];bool IsBeingPlayed;
public:virtual void ProcessInput(const CInputEvent&,CProcessedInput&,CInputProcessTavernGame*);
 virtual void EnterGame();bool GetIsBeingPlayed(){return IsBeingPlayed;}
};
class CTCTavernGameMaster:public CTCBase{
protected:CTCTavernGame*TavernGame;
public:static ETCInterfaceType __fastcall StatGetInterfaceType(){return (ETCInterfaceType)0xd2;}
 CTCTavernGame*GetGame(){return TavernGame;}
};
template<class T>inline bool __fastcall IS_THING_ALIVE(const T&p){return p.Get()!=0&&!p->IsDead();}
class CCreatureAction_TavernGameMasterInteraction:public CCreatureActionBase{
 CIntelligentPointer<CThing>PTarget;
public:virtual void Initialise();
};
void CCreatureAction_TavernGameMasterInteraction::Initialise(){
 CCreatureActionBase::Initialise();
 if(IS_THING_ALIVE(PCreature)&&IS_THING_ALIVE(PTarget)){
  CTCTavernGameMaster*master;PTarget->GetTC(&master);
  CTCTavernGame*game=master->GetGame();
  if(game&&!game->GetIsBeingPlayed())game->EnterGame();
 }
}