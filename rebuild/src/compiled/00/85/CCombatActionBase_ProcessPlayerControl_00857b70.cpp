// CCombatActionBase::ProcessPlayerControl() -- permit late post-strike movement to finish action and reset combo
// Retail0x00857B70,314 bytes. Shared genuine virtual prefix and intelligent-pointer layout from00859D80.
// Component48 is CTCCombat, ResetComboChainPos0x00733210 writes native combo sentinels at+C/+10.
#include "fable_string.h"
#define CCombatActionBase CCombatActionBase_PDB
#include "engine/_quarantine/CCombatActionBase.h"
#undef CCombatActionBase
#define CCreatureActionBase CCreatureActionBase_PDB
#include "engine/CCreatureActionBase.h"
#undef CCreatureActionBase
#ifndef _MSC_VER
#define __forceinline inline __attribute__((always_inline))
#endif
class C3DVector { public:float X,Y,Z;C3DVector(float x,float y,float z):X(x),Y(y),Z(z){} void RotateXY(float angle);}; //0x00A14040
class CCombatActionBase;
extern long g_GameFramesPerSecond; //0x01375550
extern const float g_VectorZeroThreshold; //0x0129BA3C
#include "engine/CTCCombat.h"
struct CCombatComponent:CTCCombat {void ResetComboChainPos();}; //0x00733210
struct InterfaceEntry {long Type;CCombatComponent* Component;};
struct InterfaceMap {InterfaceEntry* First;InterfaceEntry* Last;InterfaceEntry* LowerBound(const long* key);}; //0040F020
struct Creature {
    unsigned char _beforeType[0x10];unsigned char ThingType;unsigned char _beforeFlags[0xf];unsigned long PresentBits[9];InterfaceMap Interfaces;unsigned char _beforeRelativeAcceleration[0x174];C3DVector RelativeMovementAcceleration;
    float GetCurrentRelativeMovementAngleXY()const; //0x006A9640
    __forceinline bool GetInterface(CCombatComponent** out,long type){
        if(PresentBits[type>>5]&(1UL<<(type&31))){long key=type;InterfaceMap* map=&Interfaces;InterfaceEntry* p=map->LowerBound(&key);
            if(p==map->Last || type<p->Type)p=map->Last;*out=p->Component;return true;}return false;
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
    unsigned char _baseObjectState[4];CBaseIntelligentPointer PCreature;long TotalTime,ActionTime,AnimDelayTime,FramesRunning;unsigned char _actionBody[0x88];
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
void CCombatActionBase::ProcessPlayerControl(){
    Creature* creature=PCreature.GetPItem();
    if(IsStrikeEventFinishedBeingActive()){
        float frameRate=(float)g_GameFramesPerSecond;
        long afterStrike=LastStrikeEventEndTime+1;
        if(TotalTime-afterStrike>(long)(frameRate*0.33333f)){
            C3DVector movement(creature->RelativeMovementAcceleration.X,creature->RelativeMovementAcceleration.Y,creature->RelativeMovementAcceleration.Z);
            if(movement.X*movement.X+movement.Y*movement.Y+movement.Z*movement.Z>g_VectorZeroThreshold*g_VectorZeroThreshold){
                float angle=creature->GetCurrentRelativeMovementAngleXY();movement.RotateXY(angle);
                if(FramesRunning-afterStrike>(long)(frameRate*0.4f)){
                    FinishAction();CCombatComponent* combat;
                    PCreature.GetPItem()->GetInterface(&combat,48);combat->ResetComboChainPos();
                }
            }
        }
    }
}