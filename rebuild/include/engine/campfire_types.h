#ifndef FABLE_CAMPFIRE_TYPES_H
#define FABLE_CAMPFIRE_TYPES_H
#include "engine/C3DVector.h"
#include "engine/CTCBase.h"
#include "engine/CThingObject.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CCountedPointer.h"
#include "stlport/fable_map.h"
#include "engine/CCombatActionBase.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CSoundPair.h"
enum EIdleStateGroup; enum EMovementType; class CRightHandedSet; class CBrainDef; class CTCUserActionPointBaseDef;
class CNavigationInput {
public: C3DVector Destination;
public: float Proximity;
public: EMovementType MovementType;
public: float Speed;
public: CNavigationInput(const C3DVector&,float,float);
public: CNavigationInput(const C3DVector&,float,EMovementType);
public: CNavigationInput();
public: bool operator==(const CNavigationInput&) const;
public: bool operator!=(const CNavigationInput&) const;
};
class CBrainOutput {
private: unsigned long Types;
private: long LookFlags;
private: CNavigationInput NavigationInput;
private: CCountedPointer<CCreatureActionBase> PAction;
private: C3DVector MoveToPos;
private: C3DVector LookAtPos;
private: CIntelligentPointer<CThing const > LookAtThing;
public: CBrainOutput();
public: void Clear();
public: void SetAsPumpCombatActions();
public: void SetAsFaceMovementDirection();
public: void SetAsLookAtPos(const C3DVector&,long);
public: void SetAsLookAtThing(const CThing&,long);
public: void SetAsMoveToPos(const C3DVector&,const CNavigationInput&);
public: void SetAsPerformAction(const CCreatureActionBase& action) { Types |= 8; PAction.Reset(action.Clone()); }
public: const C3DVector& PeekLookAtPos() const;
public: const CThing* PeekLookAtThing() const;
public: const C3DVector& PeekMoveToPos() const;
public: const CNavigationInput& PeekNavigationInput() const;
public: const CCountedPointer<CCreatureActionBase>& GetAction() const;
public: long GetLookFlags() const;
public: bool IsEmpty() const;
public: bool IsPumpCombatActions() const;
public: bool IsFaceMovementDirection() const;
public: bool IsLookAtPos() const;
public: bool IsLookAtThing() const;
public: bool IsMoveToPos() const;
public: bool IsPerformAction() const;
public: CBrainOutput& operator=(const CBrainOutput&);
};
class CTCUserActionPointBase : public CTCBase {
public: CTCUserActionPointBase(CThing&);
public: bool IsActionPointFree() const;
protected: bool IsActionPointIndexBlocked(long);
public: virtual bool IsActionPointIndexBlocked(CThing&,C3DVector&,CRightHandedSet&,long) const;
public: bool HasBlockedActionPoints() const;
public: long GetNumberOfActionPoints() const;
public: long GetNumberOfActionPointsInUse() const;
public: bool GetFreeActionPoint(CThing&,C3DVector&,CRightHandedSet&,long&);
public: void SetActionPointAsFree(long);
protected: void UpdateBlockedActionPoints();
protected: std::vector<long,std::allocator<long> > FreeActionPointIndexes;
protected: std::vector<long,std::allocator<long> > BlockedActionPointIndexes;
protected: long NumberOfActionPoints;
protected: long FrameOfNextCheck;
protected: CIntelligentPointer<CThing> PThing;
public: virtual ~CTCUserActionPointBase();
public: CTCUserActionPointBase& operator=(const CTCUserActionPointBase&);
};
class CTCSingleUserActionPoints : public CTCUserActionPointBase {
public: static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_SINGLE_USER_ACTION_POINTS; }
public: virtual ETCInterfaceType GetInterfaceType() const;
public: static CTCBase* __fastcall Alloc(CThing&);
public: static CCharString __fastcall StatGetClassTypeName();
public: virtual CCharString GetClassTypeName() const;
public: CTCSingleUserActionPoints(CThing&);
public: virtual void OnCreate();
public: virtual bool IsActionPointIndexBlocked(CThing&,C3DVector&,CRightHandedSet&,long) const;
public: virtual ~CTCSingleUserActionPoints();
public: CTCSingleUserActionPoints& operator=(const CTCSingleUserActionPoints&);
};
class CTCIdleScheduler : public CTCBase {
public: static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_IDLE_SCHEDULER; }
public: virtual ETCInterfaceType GetInterfaceType() const;
public: static CTCBase* __fastcall Alloc(CThing&);
public: static CCharString __fastcall StatGetClassTypeName();
public: virtual CCharString GetClassTypeName() const;
public: CTCIdleScheduler(CThing&);
public: void RegisterIdleStateGroup(CAIStateGroupBase*,EIdleStateGroup);
public: void ChooseNewIdle();
public: bool IsPreferredIdle(CAIStateGroupBase*);
public: void Clear();
protected: std::map<enum EIdleStateGroup,CAIStateGroupBase *,std::less<enum EIdleStateGroup>,std::allocator<std::pair<enum EIdleStateGroup const ,CAIStateGroupBase *> > > IdleStates;
protected: std::map<enum EIdleStateGroup,long,std::less<enum EIdleStateGroup>,std::allocator<std::pair<enum EIdleStateGroup const ,long> > > IdleStatesWeight;
protected: EIdleStateGroup Preferred;
protected: long FrameLastChecked;
public: virtual ~CTCIdleScheduler();
public: CTCIdleScheduler& operator=(const CTCIdleScheduler&);
};
class CCreatureAction_GetUpFromSitting : public CCombatActionBase {
public: CCreatureAction_GetUpFromSitting(CThingCreatureBase& owner,const CCharString& animName,long sittingMode,bool removeOnPerformAction) : CCombatActionBase(owner,50,
#ifdef FABLE_RETAIL_CCREATUREACTION_GETUPFROMSITTING_SOUNDARG
// FableWin02832ef3 calls the one-argument sound constructor with zero.
 CSoundPair(0)
#else
 CSoundPair()
#endif
 ),SittingMode(sittingMode),RemoveOnPerformAction(removeOnPerformAction) { SetCombatAnimation(animName); }
public: virtual bool IsPreventingNormalMovement() const;
public: virtual bool IsPreventingLookTurning() const;
public: virtual bool IsMovementUsingPhysics() const;
public: virtual void Initialise();
public: virtual void PerformAction();
public: virtual void OnAnimationLoaded();
public: virtual CCreatureActionBase* Clone() const;
private: long SittingMode;
private: bool RemoveOnPerformAction;
public: CCreatureAction_GetUpFromSitting& operator=(const CCreatureAction_GetUpFromSitting&);
};
class CAIStateGroup_SitRoundCampfire : public CAIStateGroupBase {
public: virtual void Initialise(CAIBrain&,CThingAICreature&,long,EScriptAIPriority,const CCharString&);
public: virtual void Update();
public: virtual bool IsRunnable();
public: virtual void OnReplace();
public: virtual void InitStateGroup(const CWorld&,CThingAICreature&);
public: virtual bool AllowPriorityOverlap();
public: static CAIStateGroupBase* __fastcall Alloc();
public: static CCharString __fastcall GetName();
private: CThingObject* GetPNearCampfire() const;
private: CIntelligentPointer<CThingObject> PCampfire;
private: long SeatPosIndex;
private: bool GotSeatPos;
private: bool Sitting;
private: unsigned long FrameToNextSitOnSeat;
public: CAIStateGroup_SitRoundCampfire();
public: virtual ~CAIStateGroup_SitRoundCampfire();
public: CAIStateGroup_SitRoundCampfire& operator=(const CAIStateGroup_SitRoundCampfire&);
};
FABLE_STATIC_ASSERT(sizeof(CBrainOutput)==0x48);
FABLE_STATIC_ASSERT(sizeof(CCreatureAction_GetUpFromSitting)==0x11c);
FABLE_STATIC_ASSERT(sizeof(CAIStateGroup_SitRoundCampfire)==0x28);
#endif
