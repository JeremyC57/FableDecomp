// CCreatureAction_UseObject::CCreatureAction_UseObject(const CCreatureAction_UseObject&) -- copies the action base, counted used-item pointer and trailing state
// Leaf: CCreatureAction_UseObject
// Retail: Fable.exe (TLC, VC7.1), @ 0x006A0D50, 247 bytes, ret 4.
// Callees: CCreatureActionBase copy ctor @ 0x0062EBC0; CBaseIntelligentPointer ctor/GetPItem/SetPItem @ 0x00A01B10/0x00A01B50/0x00A01B90.
// The production PDB headers are included under aliases for names and donor layout. Retail native CCreatureActionBase is 0xA8
// (the sibling native ctor at 0x0069F230 proves PUsedThing +0xA8); the projection below keeps its actual virtual API and native size.
// The pointer owns CBaseObjectPointer* PData at +4; PData holds object/refcount. Dummy is CRightHandedSet (Up, Forward).
#define CCreatureAction_UseObject CCreatureAction_UseObjectPdb
#include "engine/CCreatureAction_UseObject.h"
#undef CCreatureAction_UseObject
#define CCreatureActionBase CCreatureActionBasePdb
#include "engine/CCreatureActionBase.h"
#undef CCreatureActionBase
#include "engine/CTCPhysicsBase.h"
#include "engine/CBaseObject.h"
#include "fable_string.h"
class CThing;
class CThingCreatureBase;
class CTCWeaponTrail;
class C3DVector { public: float x, y, z; };
class CThingCreatureBase;
class CSoundPair { public: long first; long last; };
class CCreatureActionBase { public:
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
template<class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    virtual ~CIntelligentPointer(); // CIntelligentPointer<CThing> vtable 0x01238C6C, slot 0 @0x004A9710
    CIntelligentPointer(CThing* item) : CBaseIntelligentPointer() { SetPItem(reinterpret_cast<CBaseObject*>(item)); } // native initializer used by overload @0x0069F230
    CIntelligentPointer(const CIntelligentPointer& other)
        : CBaseIntelligentPointer()
    {
        SetPItem(other.GetPItem());
    }
};
class CCreatureAction_UseObject : public CCreatureActionBase { public:
    CIntelligentPointer<CThing> PUsedThing; // retail +0xA8, PDB +0xB4
    CTCPhysicsBase* PreviousPhysicsComponent; // retail +0xB0, PDB +0xBC
    float DummyUpX, DummyUpY, DummyUpZ; // CRightHandedSet::Up, retail +0xB4
    float DummyForwardX, DummyForwardY, DummyForwardZ; // CRightHandedSet::Forward, retail +0xC0
    float DummyPos_X, DummyPos_Y, DummyPos_Z; // retail +0xCC
    float Proximity; // retail +0xD8
    bool UsingDummy, MovingToDummy, TurningToDummy, AllowTurn; // retail +0xDC..+0xDF
    virtual ~CCreatureAction_UseObject();
    virtual void PerformAction();
    virtual void FinishAction();
    virtual bool IsPlayingAnimation() const;
    virtual CCreatureActionBase* Clone() const;
    virtual void Initialise();
    virtual bool IsOverrideableByActionOfSamePriority() const;
    virtual bool IsPreventingNormalMovement() const;
    virtual bool IsPreventingLookTurning() const;
    virtual void FrameUpdate();
    virtual bool IsUsingForcedMovement() const;
    CCreatureAction_UseObject(CThingCreatureBase* creature, CThing* usedThing, bool allowTurn); // native @0x0069F230
    CCreatureAction_UseObject(const CCreatureAction_UseObject& other);
};


CCreatureAction_UseObject::CCreatureAction_UseObject(const CCreatureAction_UseObject& other)
    : CCreatureActionBase(other)
    , PUsedThing(other.PUsedThing)
    , PreviousPhysicsComponent(other.PreviousPhysicsComponent)
    , DummyUpX(other.DummyUpX), DummyUpY(other.DummyUpY), DummyUpZ(other.DummyUpZ)
    , DummyForwardX(other.DummyForwardX), DummyForwardY(other.DummyForwardY), DummyForwardZ(other.DummyForwardZ)
    , DummyPos_X(other.DummyPos_X), DummyPos_Y(other.DummyPos_Y), DummyPos_Z(other.DummyPos_Z)
    , Proximity(other.Proximity)
    , UsingDummy(other.UsingDummy), MovingToDummy(other.MovingToDummy), TurningToDummy(other.TurningToDummy), AllowTurn(other.AllowTurn)
{
}