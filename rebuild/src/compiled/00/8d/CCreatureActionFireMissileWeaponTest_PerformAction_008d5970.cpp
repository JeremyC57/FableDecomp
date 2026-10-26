// CCreatureAction_FireMissileTest::PerformAction -- emit simple noise and optionally fire a configured shot at target.
// Retail008D5970,291bytes; leaf donor header unavailable; nativebaseA8,targetA8,shotB0.
#define CMeleeCombatAbilityDef CMeleeCombatAbilityDefPdb
#include "engine/_quarantine/CMeleeCombatAbilityDef.h"
#undef CMeleeCombatAbilityDef
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
struct CInterfaceMap {
    struct Entry { long type; void* component; };
    Entry *Start,*Finish,*End;
    Entry* LowerBound(const long*); //40F020
    Entry* LowerBound(const long*)const; //4365B0
};
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
            long type=key;
            CInterfaceMap* map=&Components;
            CInterfaceMap::Entry* it=map->LowerBound(&type);
            if(it==map->Finish || key<it->type) it=map->Finish;
            *out=(T*)it->component;return true;
        }
        return false;
    }
    template<class T> __forceinline bool GetInterface(T** out,long key) const {
        if(UsingComponents[key>>5]&(1UL<<(key&31))) {
            long type=key;
            const CInterfaceMap* map=&Components;
            CInterfaceMap::Entry* it=map->LowerBound(&type);
            if(it==map->Finish || key<it->type) it=map->Finish;
            *out=(T*)it->component;return true;
        }
        return false;
    }
};
class CThingCreatureBase:public CThing { public:void MakeNoise(const CCharString&); /*00662110*/ long GetShotAccuracyPercentage()const; /*00663B10*/ CThing* GetPrimaryHeldThing()const; bool SetCurrentAction(const CCreatureActionBase&);}; //006644F0
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
 ~DefPointer(){if(Object)Object->Release();}
 T* operator->()const{return Object;}
};

class CMeleeCombatAbilityDef:public NativeCountedDef { public:}; // borrowed native lifetime view, no definition fields accessed

class CTCCarrying { public:CThing* GetThingInPrimarySlot();}; //006A1440
__forceinline CThing* CThingCreatureBase::GetPrimaryHeldThing()const {CThing* held=0;CTCCarrying* carrying;if(GetInterface(&carrying,70))held=carrying->GetThingInPrimarySlot();return held;}
class CRightHandedSet;
CThing* __fastcall CreateShot(long,CThing*,CThing*,const CThing*,float,float,float,const DefPointer<CMeleeCombatAbilityDef>&,const C3DVector*,const CRightHandedSet*); //007C0AD0
class CCreatureAction_FireMissileTest:public CCreatureActionBase { public:
    virtual ~CCreatureAction_FireMissileTest();
    virtual CCreatureActionBase* Clone()const;
    virtual void PerformAction();
 CIntelligentPointer<const CThing> PTarget;long Shot;
 CCreatureAction_FireMissileTest(CThingCreatureBase*,CThing*,long,const CAnimationEntry&,float,long,long); //008D5200 ret1C
};
void CCreatureAction_FireMissileTest::PerformAction(){
 if(PCreature.IsValid()){
  {CCharString name("SIMPLE_NOISE",-1);PCreature->MakeNoise(name);}
  if(Shot){
   long accuracy=PCreature->GetShotAccuracyPercentage();
   CThing* weapon=0;
   {const CThingCreatureBase* shooter=PCreature;CTCCarrying* carrying;
   if(shooter->GetInterface(&carrying,70))weapon=carrying->GetThingInPrimarySlot();}
   CreateShot(Shot,PCreature,weapon,PTarget,float(accuracy),1.0f,1.0f,DefPointer<CMeleeCombatAbilityDef>(),0,0);
  }
 }
}