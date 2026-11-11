// CCombatActionBase::FrameUpdate() -- update facing/strikes, post a miss event, advance and handle interruption transition
// Retail0x00859D80,314 bytes. Actual headers retained as PDB references; native action offsets follow ctor00857F30.
// Virtual prefix0..27 comes from retail vtable01259E44; no placeholder slots. Pointer record is real intelligent-pointer family.
#include "fable_compiler.h"
#include "fable_string.h"
#define CCombatActionBase CCombatActionBase_PDB
#include "engine/_quarantine/CCombatActionBase.h"
#undef CCombatActionBase
#define CCreatureActionBase CCreatureActionBase_PDB
#include "engine/CCreatureActionBase.h"
#undef CCreatureActionBase
#define FABLE_RETAIL_CTCENTITYEVENTS_ADDEVENT
#include "engine/CTCEntityEvents.h"
#ifndef _MSC_VER
#define __forceinline inline __attribute__((always_inline))
#endif
class C3DVector;class CCombatActionBase;
namespace NEntityEvents {
class CEventBase { public:
    virtual ~CEventBase();virtual long GetEventType()const=0;
    long WorldFrame;CEventBase():WorldFrame(0){}
};
struct CEventAttackedWithMeleeWeaponWithoutHittingAnything:CEventBase {virtual ~CEventAttackedWithMeleeWeaponWithoutHittingAnything();virtual long GetEventType()const;}; //vtable01276484 type30
struct CEventAttackedWithBareHandsWithoutHittingAnything:CEventBase {virtual ~CEventAttackedWithBareHandsWithoutHittingAnything();virtual long GetEventType()const;}; //vtable0127649C type31
}
// agentNF17: the InterfaceEntry / InterfaceMap stand-ins are the explicit specialisation CVectorMap<ETCInterfaceType, CTCBase*> (the
//   landed 0x0074EE90 idiom) and its std::pair value_type; 0x0040F020 is its LowerBound (NF14 / established spelling).
class CTCBase;
enum ETCInterfaceType { };
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class C> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> value_type;
    value_type* _M_start;                                   // +0x00 (the STLport vector)
    value_type* _M_finish;                                  // +0x04
    value_type* LowerBound(const ETCInterfaceType& key);    // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > InterfaceMap;
struct Creature {
    unsigned char _beforeType[0x10];unsigned char ThingType;unsigned char _beforeFlags[0xf];unsigned long PresentBits[9];InterfaceMap Interfaces;
    FABLE_FORCEINLINE bool GetInterface(CTCEntityEvents** out,long type){
        if(PresentBits[type>>5]&(1UL<<(type&31))){ETCInterfaceType key=(ETCInterfaceType)type;InterfaceMap* map=&Interfaces;InterfaceMap::value_type* p=map->LowerBound(key);
            if(p==map->_M_finish || type<p->first)p=map->_M_finish;*out=(CTCEntityEvents*)p->second;return true;}return false;
    }
};
bool FABLE_FASTCALL IsThingCarryingMeleeWeapon(Creature* creature); //006A15A0
struct CBaseObjectPointer {void* object;unsigned long refcount;};
struct CBaseIntelligentPointer {virtual ~CBaseIntelligentPointer();CBaseObjectPointer* PData;Creature* GetPItem()const;}; //00A01B50
class CCreatureActionBase { public:
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
class CCombatActionBase:public CCreatureActionBase { public:
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
        CTCEntityEvents* events;PCreature.GetPItem()->GetInterface(&events,66);
        if(IsThingCarryingMeleeWeapon(PCreature.GetPItem()))events->AddEvent(new NEntityEvents::CEventAttackedWithMeleeWeaponWithoutHittingAnything);
        else events->AddEvent(new NEntityEvents::CEventAttackedWithBareHandsWithoutHittingAnything);
        SentMissedEvent=true;
    }
    if(FrameUpdatePauseFrames==0)CCreatureActionBase::FrameUpdate();else --FrameUpdatePauseFrames;
    if(PCreature.GetPItem()->ThingType==2)ProcessPlayerControl();
    if(IsActionReadyForInterruption()&&!wasReady){ResetMeleeCombatAbilityEffects();HasResetMeleeCombatAbilityEffects=true;}
}