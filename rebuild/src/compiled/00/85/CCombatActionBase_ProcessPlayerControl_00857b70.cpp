// CCombatActionBase::ProcessPlayerControl() -- permit late post-strike movement to finish action and reset combo
// Naming wave 18: the shared const float 1e-4f at 0x0129BA3C is ?DELTA_NOTIONAL_ZERO@@3MB (raw Ego_r public + typed data record; was EPSILON).
// Naming wave 18: the global stand-in g_GameFramesPerSecond (0x01375550) is the protected static long CMainGameComponent::ConstantFPS, read through the
//   inline static accessor GetConstantFPS() as retail code outside the class does (name-only: the bytes are unchanged).
// Retail0x00857B70,314 bytes. Shared genuine virtual prefix and intelligent-pointer layout from00859D80.
// Component48 is CTCCombat, ResetComboChainPos0x00733210 writes native combo sentinels at+C/+10.
#include "fable_compiler.h"
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
class CMainGameComponent {
protected:
    static long ConstantFPS;                             // 0x01375550 ?ConstantFPS@CMainGameComponent@@1JA (protected static)
public:
    static long __fastcall GetConstantFPS() { return ConstantFPS; }   // Ego_r / FableWin ?GetConstantFPS@CMainGameComponent@@SIJXZ (inline)
};
extern const float DELTA_NOTIONAL_ZERO;              //0x0129BA3C
#include "engine/CTCCombat.h"
struct CCombatComponent:CTCCombat {void ResetComboChainPos();}; //0x00733210
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
    unsigned char _beforeType[0x10];unsigned char ThingType;unsigned char _beforeFlags[0xf];unsigned long PresentBits[9];InterfaceMap Interfaces;unsigned char _beforeRelativeAcceleration[0x174];C3DVector RelativeMovementAcceleration;
    float GetCurrentRelativeMovementAngleXY()const; //0x006A9640
    FABLE_FORCEINLINE bool GetInterface(CCombatComponent** out,long type){
        if(PresentBits[type>>5]&(1UL<<(type&31))){ETCInterfaceType key=(ETCInterfaceType)type;InterfaceMap* map=&Interfaces;InterfaceMap::value_type* p=map->LowerBound(key);
            if(p==map->_M_finish || type<p->first)p=map->_M_finish;*out=(CCombatComponent*)p->second;return true;}return false;
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
        float frameRate=(float)CMainGameComponent::GetConstantFPS();
        long afterStrike=LastStrikeEventEndTime+1;
        if(TotalTime-afterStrike>(long)(frameRate*0.33333f)){
            C3DVector movement(creature->RelativeMovementAcceleration.X,creature->RelativeMovementAcceleration.Y,creature->RelativeMovementAcceleration.Z);
            if(movement.X*movement.X+movement.Y*movement.Y+movement.Z*movement.Z>DELTA_NOTIONAL_ZERO*DELTA_NOTIONAL_ZERO){
                float angle=creature->GetCurrentRelativeMovementAngleXY();movement.RotateXY(angle);
                if(FramesRunning-afterStrike>(long)(frameRate*0.4f)){
                    FinishAction();CCombatComponent* combat;
                    PCreature.GetPItem()->GetInterface(&combat,48);combat->ResetComboChainPos();
                }
            }
        }
    }
}