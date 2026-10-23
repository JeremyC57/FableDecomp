// CCreatureActionBase::GetPrecedingAction(CCreatureActionBase*&) const -- retain carried item and build sheathing action before cloned follow-up.
// Native 0x00692F30 size244; donor vectors/maps16 versus native12 account for tail difference.
// Authentic polymorphic base and typed pointer destructors; no substitute vtable slots.
// Leaf: GetPrecedingAction
#define CCreatureActionBase CCreatureActionBase_PDB
#include "engine/CCreatureActionBase.h"
#undef CCreatureActionBase
#define CBaseObject CBaseObject_PDB
#include "engine/CBaseObject.h"
#undef CBaseObject
#include "fable_string.h"
#include <stdlib.h>
#include <new>
#define CThingCreatureBase CThingCreatureBase_PDB
#include "engine/CThingCreatureBase.h"
#undef CThingCreatureBase
struct CTCWeaponTrail;
struct C3DVector {float X,Y,Z;};
struct CThing;
struct CTCCarrying {CThing* GetThingInPrimarySlot()const;}; //0x006A1440
struct CInterfaceMap {
 struct Entry {long Type;CTCCarrying* Component;};
 Entry* First;Entry* Last;
 Entry* LowerBound(const long*) const; //0x004365B0
};
struct CThingCreatureBase {
 unsigned char _nativeBeforeComponentFlags[0x20];
 unsigned long UsingComponents[9];
 CInterfaceMap Components;
 __forceinline bool GetCarrying(CTCCarrying** out){
  if(UsingComponents[2]&0x40){CInterfaceMap* map=&Components;const long type=0x46;
   CInterfaceMap::Entry* p=map->LowerBound(&type);
   CInterfaceMap::Entry* selected=(p!=map->Last && p->Type<=type)?p:map->Last;
   *out=selected->Component;return true;
  }
  return false;
 }
};
struct CBaseObject {
 virtual ~CBaseObject(); // ordinary 0x0099A430, deleting0x0099A4D0
 virtual void Validate()const; //0x0099A400
 CBaseObjectPointer* IntelligentPointer;
 CBaseObject(); //0x0099A380
};
struct CBaseIntelligentPointer {
 virtual ~CBaseIntelligentPointer(); //0x00A01C10
 void* PData;
 CBaseIntelligentPointer(); //0x00A01B10
 void SetPItem(CThing*); //0x00A01B90
 CThing* GetPItem() const; //0x00A01B50
};
struct CIntelligentPointer_CThing : CBaseIntelligentPointer {
 CIntelligentPointer_CThing() {}
 CIntelligentPointer_CThing(CThing* p){SetPItem(p);}
};
struct CIntelligentPointer_CThingCreatureBase : CBaseIntelligentPointer {
 CIntelligentPointer_CThingCreatureBase() {}
 CThingCreatureBase* GetPItem() const {return reinterpret_cast<CThingCreatureBase*>(CBaseIntelligentPointer::GetPItem());}
};
struct CAnimationEntry {
 long BankIndex; unsigned long AnimName,GroupName; void* Components;
 CAnimationEntry(long,long,long,long); //0x005DED00
};
struct CAnimationEntryPointer {
 CAnimationEntry* Data; CCPPointerInfo* Info;
 CAnimationEntryPointer():Data(0),Info(0){}
 void Reset(CAnimationEntry*); //0x00696830
};
struct CSoundPair {
 long FirstSound,LastSound;
 CSoundPair():FirstSound(0),LastSound(0){}
};
// Native VC6 vector control block, 3 pointers; element storage is external.
struct CActionEventPointer {void* Data; CCPPointerInfo* Info;};
template<class T> struct CActionVector {
 T* First; T* Last; T* End;
 CActionVector():First(0),Last(0),End(0){}
};
struct CCutSceneEventNode {
 unsigned char Color;
 CCutSceneEventNode* Parent; CCutSceneEventNode* Left; CCutSceneEventNode* Right;
 long Key; CCharString Value;
};
struct CCutSceneEventMap {
 CCutSceneEventNode* Head; unsigned long Count; unsigned char Allocator;
 CCutSceneEventMap():Head(0) {
  Head=(CCutSceneEventNode*)malloc(sizeof(CCutSceneEventNode)); //0x00BFEA0E
  Count=0; Head->Color=0; Head->Parent=0; Head->Left=Head; Head->Right=Head;
 }
};
extern long g_NextCreatureActionID; //0x013BA8B4
struct CCreatureActionBase : CBaseObject {
    virtual ~CCreatureActionBase(); // native slot0 0062ED20
    virtual void EditUpdate(float elapsed); // native slot2 00692DB0
    virtual void PerformAction() = 0; // native slot3 00BFEA08
    virtual void FinishAction(); // native slot4 00694430
    virtual bool IsPlayingAnimation() const; // native slot5 00692850
    virtual long GetAnimDelayTime() const; // native slot6 00692460
    virtual long GetAnimationIndex() const; // native slot7 00692870
    virtual long GetAnimationTransitionInTime() const; // native slot8 006929C0
    virtual long GetAnimationTransitionOutTime() const; // native slot9 006924C0
    virtual unsigned long GetAnimExtraFlags() const; // native slot10 006924D0
    virtual CCreatureActionBase* Clone() const = 0; // native slot11 00BFEA08
    virtual void Initialise(); // native slot12 00694D10
    virtual bool IsStrikeEvent(long event) const; // native slot13 006924E0
    virtual bool IsStrikeEventActive() const; // native slot14 006924F0
    virtual bool IsStrikeAction() const; // native slot15 00692500
    virtual bool IsPartOfComboChain() const; // native slot16 006926B0
    virtual bool IsStrikeEventFinishedBeingActive() const; // native slot17 00692510
    virtual bool IsVitalStrike() const; // native slot18 00692530
    virtual bool IsKnockdownStrike() const; // native slot19 00692520
    virtual bool IsBlockCounter() const; // native slot20 006926C0
    virtual bool IsMeleeFlourish() const; // native slot21 006926D0
    virtual bool IsActionWithStrikeEventHelperName() const; // native slot22 006926E0
    virtual CCharString GetStrikeEventHelperName() const; // native slot23 006926F0
    virtual bool IsOverrideableByActionOfSamePriority() const; // native slot24 00692540
    virtual void OnFrameUpdate(float elapsed); // native slot25 00692550
    virtual bool IsQueuable() const; // native slot26 00692560
    virtual bool IsActionReadyForInterruption() const; // native slot27 00692570
    virtual bool IsActionReadyForInterruptionByMovement(const C3DVector& vector) const; // native slot28 00692710
    virtual bool IsBlockable() const; // native slot29 00692580
    virtual bool IsAffectedByHaste() const; // native slot30 00692720
    virtual bool IsAffectedByHeroStats() const; // native slot31 00692730
    virtual bool IsPhysicsPushableByHero() const; // native slot32 00692740
    virtual bool IsAlwaysPhysicsPushableBySelfAvoidance() const; // native slot33 00692750
    virtual bool IsAlwaysUseAnimationMovement() const; // native slot34 00692760
    virtual bool IsClearingMovementVector() const; // native slot35 00692770
    virtual bool IsToHaveEmptyHands() const; // native slot36 00692780
    virtual bool IsToReplaceItemIfRemoved() const; // native slot37 00692790
    virtual bool IsAllowingAnimationMovementToMoveSpeakAndWaitCameras() const; // native slot38 006927A0
    virtual bool IsPreventingQuickAccessItemUsage() const; // native slot39 006927B0
    virtual void SetHitter(const CThing& thing); // native slot40 006927C0
    virtual void SetHitDirection(C3DVector direction); // native slot41 006927D0
    virtual void SetHitPosition(C3DVector position); // native slot42 006927E0
    virtual CTCWeaponTrail* GetWeaponTrailUsedInStrikeAction(long& id) const; // native slot43 00692590
    virtual float GetTotalTimeSeconds() const; // native slot44 00692960
    virtual bool IsToEvadeAllHits() const; // native slot45 00692630
    virtual bool IsToCauseRecoil() const; // native slot46 00692640
    virtual bool IsAbleToBeAttacked() const; // native slot47 00692650
    virtual bool IsPlayHitResponse() const; // native slot48 00692660
    virtual bool IsPreventingNormalMovement() const; // native slot49 00692A20
    virtual bool IsPreventingLookTurning() const; // native slot50 00692670
    virtual void OnBlowBlocked(bool blocked); // native slot51 006927F0
    virtual void OnFollowUpActionFailedToAdd(); // native slot52 00692800
    virtual void UpdateActionRunningTime(); // native slot53 00692480
    virtual bool IsDecapitatingAttack() const; // native slot54 00692810
    virtual bool PreventSneaking() const; // native slot55 00692820
    virtual void FrameUpdate(); // native slot56 00694020
    virtual bool IsUsingAnimationMovement() const; // native slot57 00692680
    virtual bool IsUsingForcedMovement() const; // native slot58 00692690
    virtual bool IsMovementUsingPhysics() const; // native slot59 006926A0
    virtual void UpdateActionMoveToPos(); // native slot60 00692B30
    virtual void InitialiseActionMovement(); // native slot61 00692C80
    virtual float GetFrameUpdateTime() const; // native slot62 00692BC0
    virtual float GetActionSpeedParameter() const; // native slot63 00692C30
    virtual float GetActionStartRunningTimeSeconds() const; // native slot64 006923A0
    virtual bool OverrideFaceWithDefault() const; // native slot65 00692880
    virtual bool AllowsCombinationAnims() const; // native slot66 00692890
    virtual bool CanAnimationTransitionOut() const; // native slot67 006928B0
    virtual void OnAnimationLoaded(); // native slot68 00692830
    virtual bool WaitForMeshToLoad() const; // native slot69 00692840
 CIntelligentPointer_CThingCreatureBase PCreature;
 long TotalTime,ActionTime,AnimDelayTime,FramesRunning,InterruptionGroupID,InterruptionPriority,ActionID;
 mutable CIntelligentPointer_CThing PItemRemoved;
 CSoundPair Sounds;
 unsigned long PlayingSoundID;
 float RunningTimeSeconds,LastRunningTimeSeconds;
 long StartFrame,PauseFrame,PauseTimer;
 float AnimMovementStep_X,AnimMovementStep_Y,AnimMovementStep_Z;
 bool AddedActionAnim,Finished,PerformedAction,Initialised;
 long NextMovementPosWorldFrame;
 float NextMovementPos_X,NextMovementPos_Y,NextMovementPos_Z;
 CAnimationEntryPointer PAnimEntry,PPreceedingPAnimEntry;
 CActionVector<CActionEventPointer> ActionEvents;
 CCutSceneEventMap CutSceneEvents;
 CActionVector<CIntelligentPointer_CThing> DependentThings;
 bool HasPrecedingAction() const; //0x00692EC0
 void GetPrecedingAction(CCreatureActionBase*&) const;
 void SetAnimation(const CAnimationEntry&,float,float); //0x00693030
};
FABLE_STATIC_ASSERT(sizeof(CCreatureActionBase)==0xA8);
FABLE_STATIC_ASSERT(offsetof(CCreatureActionBase,CutSceneEvents)==0x90);

// Donor PFollowUpAction+0xb4, native+0xa8 after compact containers.
struct CCreatureAction_SheatheItemToInventory : CCreatureActionBase {
 CCreatureActionBase* FollowUpData; CCPPointerInfo* FollowUpInfo;
 CCreatureAction_SheatheItemToInventory(CThingCreatureBase&,CCreatureActionBase*,long); //0x0069FFD0
 virtual void PerformAction(); //0x0069F4F0
 virtual CCreatureActionBase* Clone() const; //0x006A0C20
};
FABLE_STATIC_ASSERT(sizeof(CCreatureAction_SheatheItemToInventory)==0xB0);
void CCreatureActionBase::GetPrecedingAction(CCreatureActionBase*& result) const
{
 if(HasPrecedingAction()) {
  CThingCreatureBase* creature=PCreature.GetPItem();
  CTCCarrying* carrying;
  creature->GetCarrying(&carrying);
  {
   CThing* heldThing=carrying->GetThingInPrimarySlot();
   CIntelligentPointer_CThing held(heldThing);
   PItemRemoved.SetPItem(held.GetPItem());
  }
  void* storage=::operator new(sizeof(CCreatureAction_SheatheItemToInventory));
  CCreatureAction_SheatheItemToInventory* next;
  if(storage){
   long group=InterruptionGroupID;
   CThingCreatureBase* owner=PCreature.GetPItem();
   next=::new(storage) CCreatureAction_SheatheItemToInventory(*owner,Clone(),group+1);
  }else next=0;
  result=next;
 } else result=0;
}