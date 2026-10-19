// CCreatureAction_UnsheatheItemFromInventory::CCreatureAction_UnsheatheItemFromInventory -- configure unsheathe animation, item and owned follow-up action
// Retail 0069F0F0,252 bytes. PDB signature confirms creature&, string&, item&, long,long, action*.
// Base nativeA8 with genuine70-slot API; animation entry native16 follows PDB fields. No leaf generated header exists.
// Leaf: ??0CCreatureAction_UnsheatheItemFromInventory@@
#include <new>
#define CCreatureActionBase CCreatureActionBasePdb
#include "engine/CCreatureActionBase.h"
#undef CCreatureActionBase
#include "engine/CTCPhysicsBase.h"
#include "engine/CBaseObject.h"
#include "fable_string.h"
struct CThing;
struct CThingCreatureBase;
struct CTCWeaponTrail;
struct C3DVector { float x, y, z; };
struct CThingCreatureBase;
struct CSoundPair { long first; long last; CSoundPair():first(0),last(0) {} };
struct CAnimationEntry;
struct CCreatureActionBase {
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
    unsigned char nativeFields[0xa4]; // actual native state after its vptr; native size 0xA8
    CCreatureActionBase(CThingCreatureBase*,float,float,const CAnimationEntry&,const CSoundPair&,long,long); // 00693B30
    CCreatureActionBase(const CCreatureActionBase& other); // native copy ctor @0x0062EBC0
    CCreatureActionBase(CThingCreatureBase* creature, const CSoundPair& sounds, long first, long second); // native @0x00693C70
};
struct CBaseObjectPointer { CBaseObject* object; unsigned long refcount; };
struct CBaseIntelligentPointer {
    virtual ~CBaseIntelligentPointer(); // ordinary destructor @0x00A01C10; deleting destructor @0x00A01C50
    CBaseObjectPointer* PData;
    CBaseIntelligentPointer();
    CBaseObject* GetPItem() const;
    void SetPItem(CBaseObject* item);
};
template<class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    virtual ~CIntelligentPointer(); // CIntelligentPointer<CThing> vtable 0x01238C6C, slot 0 @0x004A9710
    CIntelligentPointer(CThing* item) : CBaseIntelligentPointer() { SetPItem(reinterpret_cast<CBaseObject*>(item)); } // native initializer used by overload @0x0069F230
    CIntelligentPointer(const CIntelligentPointer& other)
        : CBaseIntelligentPointer()
    {
        SetPItem(other.GetPItem());
    }
};

#define CIVCountedPointeeBase CIVCountedPointeeBaseLayout
#include "engine/CIVCountedPointeeBase.h"
#undef CIVCountedPointeeBase
struct CIVCountedPointeeBase {
    virtual ~CIVCountedPointeeBase(); // slot0
    virtual void Delete() const; // slot1 native0042D930 deletes this through slot0
    unsigned long IVRefCount;
    void Release() { if(--IVRefCount==0) Delete(); }
};
struct CAnimComponentMap : CIVCountedPointeeBase {};
struct CAnimationEntry {
    long BankIndex,AnimName,GroupName;
    CAnimComponentMap* Components;
    CAnimationEntry(long bank,long transitionIn,long transitionOut,long flags); //005DED00 real seed overload
    CAnimationEntry(const CAnimationEntry&,long transitionInTime); //005DEC70
    ~CAnimationEntry() { if(Components) Components->Release(); }
};
const CAnimationEntry& __fastcall FindAnimation(CThingCreatureBase*,const CCharString&); //006924B0
void __fastcall DeleteAction(CCreatureActionBase*); //00668610, deleting destructor dispatch
struct ActionPointerInfo {
    unsigned long RefCount;
    void (__fastcall *DeleteData)(CCreatureActionBase*);
    CCreatureActionBase* Data;
    ActionPointerInfo(CCreatureActionBase* p):RefCount(1),DeleteData(&DeleteAction),Data(p) {}
};
struct ActionPointer {
    CCreatureActionBase* Data;
    ActionPointerInfo* Info;
    ActionPointer(CCreatureActionBase* p):Data(p) { if(Data) Info=new ActionPointerInfo(Data); else Info=0; }
    ~ActionPointer() { if(Info && --Info->RefCount==0) { Info->DeleteData(Info->Data); delete Info; } }
};
struct CCreatureAction_UnsheatheItemFromInventory : CCreatureActionBase {
    CIntelligentPointer<CThing> Item;
    ActionPointer FollowUp;
    virtual ~CCreatureAction_UnsheatheItemFromInventory();
    virtual void Initialise();
    virtual void PerformAction();
    virtual void FinishAction();
    virtual CCreatureActionBase* Clone() const;
    CCreatureAction_UnsheatheItemFromInventory(CThingCreatureBase&,const CCharString&,CThing&,long,long,CCreatureActionBase*);
};
CCreatureAction_UnsheatheItemFromInventory::CCreatureAction_UnsheatheItemFromInventory(CThingCreatureBase& creature,const CCharString& animation,CThing& item,long first,long second,CCreatureActionBase* followUp)
 : CCreatureActionBase(&creature,-1.0f,0.0f,CAnimationEntry(FindAnimation(&creature,animation),0),CSoundPair(),first,second), Item(&item), FollowUp(followUp) {}