// CCombatActionBase::FrameUpdate() -- update facing/strikes, post a miss event, advance and handle interruption transition
// Retail0x00859D80,314 bytes. Actual headers retained as PDB references; native action offsets follow ctor00857F30.
// Virtual prefix0..27 comes from retail vtable01259E44; no placeholder slots. Pointer record is real intelligent-pointer family.
#include "fable_string.h"
#define CCombatActionBase CCombatActionBase_PDB
#include "engine/_quarantine/CCombatActionBase.h"
#undef CCombatActionBase
#define CCreatureActionBase CCreatureActionBase_PDB
#include "engine/CCreatureActionBase.h"
#undef CCreatureActionBase
#include "engine/CTCEntityEvents.h"
#ifndef _MSC_VER
#define __forceinline inline __attribute__((always_inline))
#endif
struct C3DVector;struct CCombatActionBase;
namespace NEntityEvents {
struct CEventBase {
    virtual ~CEventBase();virtual long GetEventType()const=0;
    long WorldFrame;CEventBase():WorldFrame(0){}
};
struct CEventAttackedWithMeleeWeaponWithoutHittingAnything:CEventBase {virtual ~CEventAttackedWithMeleeWeaponWithoutHittingAnything();virtual long GetEventType()const;}; //vtable01276484 type30
struct CEventAttackedWithBareHandsWithoutHittingAnything:CEventBase {virtual ~CEventAttackedWithBareHandsWithoutHittingAnything();virtual long GetEventType()const;}; //vtable0127649C type31
}
struct CTCEntityEventsCalls:CTCEntityEvents {void AddEvent(NEntityEvents::CEventBase* event);}; //008113C0
struct InterfaceEntry {long Type;CTCEntityEventsCalls* Component;};
struct InterfaceMap {InterfaceEntry* First;InterfaceEntry* Last;InterfaceEntry* LowerBound(const long* key);}; //0040F020
struct Creature {
    unsigned char _beforeType[0x10];unsigned char ThingType;unsigned char _beforeFlags[0xf];unsigned long PresentBits[9];InterfaceMap Interfaces;
    __forceinline bool GetInterface(CTCEntityEventsCalls** out,long type){
        if(PresentBits[type>>5]&(1UL<<(type&31))){long key=type;InterfaceMap* map=&Interfaces;InterfaceEntry* p=map->LowerBound(&key);
            if(p==map->Last || type<p->Type)p=map->Last;*out=p->Component;return true;}return false;
    }
};
bool FABLE_FASTCALL IsThingCarryingMeleeWeapon(Creature* creature); //006A15A0
struct CBaseObjectPointer {void* object;unsigned long refcount;};
struct CBaseIntelligentPointer {virtual ~CBaseIntelligentPointer();CBaseObjectPointer* PData;Creature* GetPItem()const;}; //00A01B50
struct CCreatureActionBase {
    virtual ~CCreatureActionBase();
    virtual void Validate()const;virtual void EditUpdate(float);virtual void PerformAction();virtual void FinishAction();
    virtual bool IsPlayingAnimation()const;virtual long GetAnimDelayTime()const;virtual long GetAnimationIndex()const;
    virtual long GetAnimationTransitionInTime()const;virtual long GetAnimationTransitionOutTime()const;virtual unsigned long GetAnimExtraFlags()const;
    virtual CCreatureActionBase* Clone()const;virtual void Initialise();virtual bool IsStrikeEvent(long)const;virtual bool IsStrikeEventActive()const;
    virtual bool IsStrikeAction()const;virtual bool IsPartOfComboChain()const;virtual bool IsStrikeEventFinishedBeingActive()const;
    virtual bool IsVitalStrike()const;virtual bool IsKnockdownStrike()const;virtual bool IsBlockCounter()const;virtual bool IsMeleeFlourish()const;
    virtual bool IsActionWithStrikeEventHelperName()const;virtual CCharString GetStrikeEventHelperName()const;
    virtual bool IsOverrideableByActionOfSamePriority()const;virtual void OnFrameUpdate(float);virtual bool IsQueuable()const;virtual bool IsActionReadyForInterruption()const;
    unsigned char _baseObjectState[4];CBaseIntelligentPointer PCreature;unsigned char _actionBody[0x98];
    void FrameUpdate(); //qualified direct base call00694020
};
struct CCombatActionBase:CCreatureActionBase {
    CBaseIntelligentPointer PTarget,PCameraPoint;void* PMeleeAbilityDef_Object;void* StrikeEventsHead;
    long LastStrikeEventEndTime;float StrikeEventInterruptTime,StrikeEventResetComboTime;long MovementEventInterruptFrame;
    float StrikeSpeedVariationStartTime,StrikeSpeedVariationEndTime;
    bool HasStrikeEventInterrupt,HasStrikeEventResetCombo,HasMovementEventInterrupt,StrikeEventInterruptActive,MovementEventInterruptActive,PreferredFinalPosSet,HasPerformAction,SentMissedEvent,HasResetMeleeCombatAbilityEffects;
    long LastAnimStage;bool HitThing,HitCreature;long FrameUpdatePauseFrames;
    void UpdateFacingDirection(); //00856B20
    void ProcessStrikeEvents(); //008597A0
    void ProcessPlayerControl(); //00857B70
    void ResetMeleeCombatAbilityEffects(); //00859660
    void FrameUpdate();
};
void CCombatActionBase::FrameUpdate(){
    bool wasReady=IsActionReadyForInterruption();
    UpdateFacingDirection();ProcessStrikeEvents();
    if(IsStrikeEventFinishedBeingActive() && !HitThing && !SentMissedEvent){
        CTCEntityEventsCalls* events;PCreature.GetPItem()->GetInterface(&events,66);
        if(IsThingCarryingMeleeWeapon(PCreature.GetPItem()))events->AddEvent(new NEntityEvents::CEventAttackedWithMeleeWeaponWithoutHittingAnything);
        else events->AddEvent(new NEntityEvents::CEventAttackedWithBareHandsWithoutHittingAnything);
        SentMissedEvent=true;
    }
    if(FrameUpdatePauseFrames==0)CCreatureActionBase::FrameUpdate();else --FrameUpdatePauseFrames;
    if(PCreature.GetPItem()->ThingType==2)ProcessPlayerControl();
    if(IsActionReadyForInterruption()&&!wasReady){ResetMeleeCombatAbilityEffects();HasResetMeleeCombatAbilityEffects=true;}
}