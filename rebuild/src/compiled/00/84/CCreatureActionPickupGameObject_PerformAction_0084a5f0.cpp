// CCreatureAction_PickupGameObject::PerformAction -- replace carried weapon and equip object.
// Retail0084A5F0,293bytes. Native baseA8 versus donorB4; PObject nativeA8.
#define CCreatureAction_PickupGameObject CCreatureAction_PickupGameObjectPdb
#include "engine/CCreatureAction_PickupGameObject.h"
#undef CCreatureAction_PickupGameObject
#define CCarryableDef CCarryableDefPdb
#include "engine/CCarryableDef.h"
#undef CCarryableDef
#define CDefClassBase CDefClassBasePdb
#include "engine/CDefClassBase.h"
#undef CDefClassBase
// Native combat action projection: base70 genuine slots; combat adds11 =81 slots.
// BaseA8 and Combat114; PDB baseB4/Combat12C. See actual headers and raw vtable audit.
#define CCreatureActionBase CCreatureActionBasePdb
#include "engine/CCreatureActionBase.h"
#undef CCreatureActionBase
#define CThing CThingPdb
#define CThingCreatureBase CThingCreatureBasePdb
#include "engine/_quarantine/CThing.h"
#include "engine/CThingCreatureBase.h"
#undef CThingCreatureBase
#undef CThing
#include "engine/CBaseObject.h"
#include "fable_string.h"
class CThing;class CThingCreatureBase;class CCreatureActionBase;class CTCWeaponTrail;
class C3DVector { public:float x,y,z;};
class CSoundPair { public:long first,last;CSoundPair():first(0),last(0){}};
struct CBaseObjectPointer {CBaseObject* object;unsigned long refcount;};
struct CBaseIntelligentPointer {
 virtual ~CBaseIntelligentPointer();
 CBaseObjectPointer* PData;
 CBaseIntelligentPointer();
 CBaseObject* GetPItem()const;
 void SetPItem(CBaseObject*);
};
template<class T>class CIntelligentPointer:public CBaseIntelligentPointer { public:
 virtual ~CIntelligentPointer();
 CIntelligentPointer():CBaseIntelligentPointer(){}
 explicit CIntelligentPointer(T* p):CBaseIntelligentPointer(){SetPItem(p);}
 T* operator->()const{return static_cast<T*>(GetPItem());}
 T& operator*()const{return *static_cast<T*>(GetPItem());}
 operator T*()const{return static_cast<T*>(GetPItem());}
 bool IsValid()const{return GetPItem()&&!(static_cast<T*>(GetPItem())->DeadFlags&1);}
};
// Retail DeadFlags91 vs donor ThingStateA1: actual PDB containers add16 bytes before this flag.
// Creature view adds only the real native SetCurrentAction API; no additional fields touched here.
class CTCPhysicsBase;struct CDefLookup;
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> Entry;
    Entry *Start,*Finish,*End;
    Entry* LowerBound(const ETCInterfaceType& key); //40F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing:public CBaseObject { public:
    unsigned char BaseState[0x18];
    unsigned long UsingComponents[9];
    CInterfaceMap Components;
    unsigned char UpdateComponentsList[8];
    void* SerialisationSectionName;
    void* PTCDriver;
    CTCPhysicsBase* PhysicsTC;
    void* PTCGraphicAppearance;
    long WorldFrameCreatedAt;
    unsigned long ThingTypeFlags;
    CDefLookup* PBaseDef;
    unsigned char LaterState[0x1d];
    unsigned char DeadFlags;
    void SetInLimbo(bool); //004C8CF0
    void Kill(bool); //004C9B80
    template<class T> __forceinline bool GetInterface(T** out,long key) {
        if(UsingComponents[key>>5]&(1UL<<(key&31))) {
            ETCInterfaceType type = (ETCInterfaceType)(key);
            CInterfaceMap* map=&Components;
            CInterfaceMap::Entry* it=map->LowerBound(type);
            if(it==map->Finish || key<it->first) it=map->Finish;
            *out=(T*)((void*)it->second);return true;
        }
        return false;
    }
};
class CThingCreatureBase:public CThing { public:bool SetCurrentAction(const CCreatureActionBase&);}; //006644F0
struct __declspec(novtable) CCreatureActionBase {
    virtual ~CCreatureActionBase();
    virtual void Validate() const;
    virtual void EditUpdate(float elapsed);
    virtual void PerformAction() = 0;
    virtual void FinishAction();
    virtual bool IsPlayingAnimation() const;
    virtual long GetAnimDelayTime() const;
    virtual long GetAnimationIndex() const;
    virtual long GetAnimationTransitionInTime() const;
    virtual long GetAnimationTransitionOutTime() const;
    virtual unsigned long GetAnimExtraFlags() const;
    virtual CCreatureActionBase* Clone() const = 0;
    virtual void Initialise();
    virtual bool IsStrikeEvent(long event) const;
    virtual bool IsStrikeEventActive() const;
    virtual bool IsStrikeAction() const;
    virtual bool IsPartOfComboChain() const;
    virtual bool IsStrikeEventFinishedBeingActive() const;
    virtual bool IsVitalStrike() const;
    virtual bool IsKnockdownStrike() const;
    virtual bool IsBlockCounter() const;
    virtual bool IsMeleeFlourish() const;
    virtual bool IsActionWithStrikeEventHelperName() const;
    virtual CCharString GetStrikeEventHelperName() const;
    virtual bool IsOverrideableByActionOfSamePriority() const;
    virtual void OnFrameUpdate(float elapsed);
    virtual bool IsQueuable() const;
    virtual bool IsActionReadyForInterruption() const;
    virtual bool IsActionReadyForInterruptionByMovement(const C3DVector& vector) const;
    virtual bool IsBlockable() const;
    virtual bool IsAffectedByHaste() const;
    virtual bool IsAffectedByHeroStats() const;
    virtual bool IsPhysicsPushableByHero() const;
    virtual bool IsAlwaysPhysicsPushableBySelfAvoidance() const;
    virtual bool IsAlwaysUseAnimationMovement() const;
    virtual bool IsClearingMovementVector() const;
    virtual bool IsToHaveEmptyHands() const;
    virtual bool IsToReplaceItemIfRemoved() const;
    virtual bool IsAllowingAnimationMovementToMoveSpeakAndWaitCameras() const;
    virtual bool IsPreventingQuickAccessItemUsage() const;
    virtual void SetHitter(const CThing& thing);
    virtual void SetHitDirection(C3DVector direction);
    virtual void SetHitPosition(C3DVector position);
    virtual CTCWeaponTrail* GetWeaponTrailUsedInStrikeAction(long& id) const;
    virtual float GetTotalTimeSeconds() const;
    virtual bool IsToEvadeAllHits() const;
    virtual bool IsToCauseRecoil() const;
    virtual bool IsAbleToBeAttacked() const;
    virtual bool IsPlayHitResponse() const;
    virtual bool IsPreventingNormalMovement() const;
    virtual bool IsPreventingLookTurning() const;
    virtual void OnBlowBlocked(bool blocked);
    virtual void OnFollowUpActionFailedToAdd();
    virtual void UpdateActionRunningTime();
    virtual bool IsDecapitatingAttack() const;
    virtual bool PreventSneaking() const;
    virtual void FrameUpdate();
    virtual bool IsUsingAnimationMovement() const;
    virtual bool IsUsingForcedMovement() const;
    virtual bool IsMovementUsingPhysics() const;
    virtual void UpdateActionMoveToPos();
    virtual void InitialiseActionMovement();
    virtual float GetFrameUpdateTime() const;
    virtual float GetActionSpeedParameter() const;
    virtual float GetActionStartRunningTimeSeconds() const;
    virtual bool OverrideFaceWithDefault() const;
    virtual bool AllowsCombinationAnims() const;
    virtual bool CanAnimationTransitionOut() const;
    virtual void OnAnimationLoaded();
    virtual bool WaitForMeshToLoad() const;
    unsigned char    _pad_0x04[0x4];
    CIntelligentPointer<CThingCreatureBase> PCreature;
    long             TotalTime;
    long             ActionTime;
    long             AnimDelayTime;
    long             FramesRunning;
    long             InterruptionGroupID;
    long             InterruptionPriority;
    long             ActionID;
    unsigned char    PItemRemoved[0x8];
    long             Sounds_FirstSound;
    long             Sounds_LastSound;
    unsigned long    PlayingSoundID;
    float            RunningTimeSeconds;
    float            LastRunningTimeSeconds;
    long             StartFrame;
    long             PauseFrame;
    long             PauseTimer;
    float            AnimMovementStep_X;
    float            AnimMovementStep_Y;
    float            AnimMovementStep_Z;
    bool             AddedActionAnim;
    bool             Finished;
    bool             PerformedAction;
    bool             Initialised;
    long             NextMovementPosWorldFrame;
    float            NextMovementPos_X;
    float            NextMovementPos_Y;
    float            NextMovementPos_Z;
    CAnimationEntry* PAnimEntry_Data;
    CCPPointerInfo*  PAnimEntry_Info;
    CAnimationEntry* PPreceedingPAnimEntry_Data;
    CCPPointerInfo*  PPreceedingPAnimEntry_Info;
    unsigned char    ActionEvents[0xc];
    unsigned char    CutSceneEvents[0xc];
    unsigned char    DependentThings[0xc];
    CCreatureActionBase(CThingCreatureBase*,float,float,const CAnimationEntry&,const CSoundPair&,long,long); //00693B30 ret1C; actual seven arguments
    CCreatureActionBase(const CCreatureActionBase& other); // native copy ctor @0x0062EBC0
    CCreatureActionBase(CThingCreatureBase* creature, const CSoundPair& sounds, long first, long second); // native @0x00693C70
};
// Real native counted base prefix with Delete slot1; complete host PMF representation.
struct NativeCountedDef {
 struct VTable;const VTable* VPtr;unsigned long IVRefCount;
 void DeletingDestructor(unsigned long);void Delete()const;
 __forceinline void Release(){if(!--IVRefCount)(this->*GetDelete())();}
 typedef void(NativeCountedDef::*DeleteMethod)()const;
 DeleteMethod GetDelete()const;
};
struct NativeCountedDef::VTable {void(NativeCountedDef::*Destructor)(unsigned long);DeleteMethod Delete;};
inline NativeCountedDef::DeleteMethod NativeCountedDef::GetDelete()const{return VPtr->Delete;}
template<class T>struct DefPointer {
 T* Object;DefPointer():Object(0){} explicit DefPointer(T* p):Object(p){if(p)++p->IVRefCount;}
 DefPointer(const DefPointer& p):Object(p.Object){if(Object)++Object->IVRefCount;}
 __forceinline void Assign(const DefPointer& p){if(Object!=p.Object){if(Object){Object->Release();Object=0;}Object=p.Object;if(Object)++Object->IVRefCount;}}
 ~DefPointer(){T* p=Object;if(p)p->Release();}
 T* operator->()const{return Object;}
};
class CCarryableDef:public NativeCountedDef { public:
 unsigned char DefinitionBaseState[0x20];
 long ActiveCarrySlot,SecondaryActiveCarrySlot,PassiveCarrySlot,OnKillFX;
 C3DVector OffsetCoordRelativeToAttachToDummy;float OffsetAmountForThingsOnTopOfMe,PassiveCarrySlotScale;bool IncludeInThingScansWhenCarried;
};
struct DefinitionSubDefInfo {long GlobalIndex;long Flags;};
struct DefinitionString {long TablePos;};
struct CDefLookup:NativeCountedDef {
 unsigned char ResourceState[0x14];void* PDefManager;long GlobalIndex;bool DefaultValsApplied;
 bool GetCarryable(DefPointer<CCarryableDef>*)const; //005CC10E
 void DeletingDestructor(unsigned long);void Delete()const;bool UnloadResource();unsigned long GetUnloadableMemory()const;
 bool IsResourceLoaded()const;void OnResourceLoaded();bool CanAbortResourceLoad(long)const;void OnResourceLoadAborted();unsigned long GetDebugMemoryInUse()const;void ResetDebugMemoryInUse();
 DefinitionString GetInstantiationName()const;void SetInstantiationName(DefinitionString);void AddSubDef(const CCharString&,long,long);void RemoveSubDef(const CCharString&);
 const DefinitionSubDefInfo* GetPSubDefInfo(const CCharString&)const;
};
struct DefinitionVTable:NativeCountedDef::VTable {
 bool(CDefLookup::*UnloadResource)();unsigned long(CDefLookup::*GetUnloadableMemory)()const;
 bool(CDefLookup::*IsResourceLoaded)()const;void(CDefLookup::*OnResourceLoaded)();bool(CDefLookup::*CanAbortResourceLoad)(long)const;void(CDefLookup::*OnResourceLoadAborted)();unsigned long(CDefLookup::*GetDebugMemoryInUse)()const;void(CDefLookup::*ResetDebugMemoryInUse)();
 DefinitionString(CDefLookup::*GetInstantiationName)()const;void(CDefLookup::*SetInstantiationName)(DefinitionString);void(CDefLookup::*AddSubDef)(const CCharString&,long,long);void(CDefLookup::*RemoveSubDef)(const CCharString&);
 const DefinitionSubDefInfo*(CDefLookup::*GetPSubDefInfo)(const CCharString&)const;
};
struct DefinitionManagerAPI {DefPointer<CCarryableDef> GetPDefFromGlobalIndex(long);}; //009ADA10
#pragma auto_inline(off)
bool CDefLookup::GetCarryable(DefPointer<CCarryableDef>* out)const {
 const DefinitionSubDefInfo* info;
 {CCharString type("CCarryableDef",-1);info=(this->*static_cast<const DefinitionVTable*>(VPtr)->GetPSubDefInfo)(type);}
 if(info){DefPointer<CCarryableDef> def=static_cast<DefinitionManagerAPI*>(PDefManager)->GetPDefFromGlobalIndex(info->GlobalIndex);out->Assign(def);return true;}
 return false;
}
#pragma auto_inline(on)

class CTCCarrying { public:
 bool IsCarrySlotEmpty(long); //006A1230: absent entry returns false; present entry tests null intelligent pointer
 void PutThingInCarrySlot(CThing*,long,bool); //006A1F80
public:
    CThing* GetThingInCarrySlot(long) const;   // 0x006A11F0
};
void __fastcall DropWeapon(CThing*,CTCCarrying*,CThingCreatureBase*); //00845CE0
class CCreatureAction_PickupGameObject:public CCreatureActionBase { public:
    virtual ~CCreatureAction_PickupGameObject();
    virtual void PerformAction();
    virtual CCreatureActionBase* Clone()const;
    virtual bool IsToHaveEmptyHands()const;
    virtual bool IsToReplaceItemIfRemoved()const;
 CIntelligentPointer<CThing> PObject;
 CCreatureAction_PickupGameObject(CThingCreatureBase*,CThing*); //007EB030
};
void CCreatureAction_PickupGameObject::PerformAction(){
 if(PObject.IsValid() && (PObject->UsingComponents[2]&0x80)) {
  DefPointer<CCarryableDef> carryable;CThing* object=PObject;object->PBaseDef->GetCarryable(&carryable);
  long slot=carryable->ActiveCarrySlot;
  CTCCarrying* carrying;
  if(PCreature->GetInterface(&carrying,70)) {
   if(PObject->UsingComponents[2]&0x100) {
    CThing* existing;
    if(carrying->IsCarrySlotEmpty(slot))existing=carrying->GetThingInCarrySlot(carryable->PassiveCarrySlot);
    else existing=carrying->GetThingInCarrySlot(slot);
    if(existing)DropWeapon(existing,carrying,PCreature);
    carrying->PutThingInCarrySlot(PObject,slot,true);
   }else carrying->PutThingInCarrySlot(PObject,slot,true);
  }
 }
}