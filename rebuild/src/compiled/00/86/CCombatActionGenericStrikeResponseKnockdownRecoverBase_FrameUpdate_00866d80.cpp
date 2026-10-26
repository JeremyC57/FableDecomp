// CCombatAction_GenericStrikeResponseKnockdownRecoverBase::FrameUpdate() -- finish when moving and enable hits after recovery percentage
// Retail0x00866D80,249 bytes. Action native base0x114 vs donor0x12c; PCreature+8, FramesRunning+1c,
// MovementEventInterruptActive+dc, CanBeHit+114. Native ThingCreatureDef RecoverEvadeHitsPercent+140 vs donor148.
// CBaseIntelligentPointer is real {vptr,CBaseObjectPointer*PData}, record{object,refcount}; never a counted pointer.
#include "fable_string.h"
#define CCombatActionBase CCombatActionBase_PDB
#include "engine/_quarantine/CCombatActionBase.h"
#undef CCombatActionBase
#define CCombatAction_GenericStrikeResponseKnockdownRecoverBase CKnockdownRecover_PDB
#include "engine/CCombatAction_GenericStrikeResponseKnockdownRecoverBase.h"
#undef CCombatAction_GenericStrikeResponseKnockdownRecoverBase
#define CThingCreatureDef CThingCreatureDef_PDB
#include "engine/_quarantine/CThingCreatureDef.h"
#undef CThingCreatureDef
#include <math.h>
#ifndef _MSC_VER
#define __forceinline inline __attribute__((always_inline))
#endif
class C3DVector { public: float X,Y,Z;float GetLength()const {return (float)sqrt(X*X+Y*Y+Z*Z);} };
class CTCCreatureModeManager { public:void RemoveMode(unsigned long mode);}; //0x0073ABA0
struct InterfaceEntry {long Type;CTCCreatureModeManager* Component;};
struct InterfaceMap {InterfaceEntry* First;InterfaceEntry* Last;InterfaceEntry* LowerBound(const long* type);}; //0x0040F020
class CThingCreatureDef { public:unsigned char _beforeRecoveryPercent[0x140];float RecoverEvadeHitsPercent;};
struct Creature {
    unsigned char _beforeType[0x10];unsigned char ThingType;unsigned char _beforeFlags[0xf];
    unsigned long PresentBits[9];InterfaceMap Interfaces;unsigned char _beforeDefinition[0x24];
    CThingCreatureDef* PThingCreatureDef;unsigned char _beforeMovement[0x14c];C3DVector RelativeMovementAcceleration;
    __forceinline bool GetInterface(CTCCreatureModeManager** out,long type){
        if(PresentBits[type>>5]&(1UL<<(type&31))){
            long key=type;InterfaceMap* map=&Interfaces;InterfaceEntry* p=map->LowerBound(&key);
            if(p==map->Last || type<p->Type)p=map->Last;
            *out=p->Component;return true;
        }return false;
    }
};
struct CBaseObjectPointer {void* object;unsigned long refcount;};
struct CBaseIntelligentPointer {virtual ~CBaseIntelligentPointer();CBaseObjectPointer* PData;Creature* GetPItem()const;}; //0x00A01B50
class CCombatActionBase { public:
    virtual ~CCombatActionBase();
    virtual void Validate()const;
    virtual void EditUpdate(float);
    virtual void PerformAction();
    virtual void FinishAction(); //real slot4
    unsigned char _baseObjectState[4];CBaseIntelligentPointer PCreature;
    long TotalTime,ActionTime,AnimDelayTime,FramesRunning;
    unsigned char _beforeMovementInterrupt[0xbc];bool MovementEventInterruptActive;
    unsigned char _combatTail[0x37];
    void FrameUpdate(); //direct qualified base call0x00859D80; unused later virtual slots omitted
};
class CCombatAction_GenericStrikeResponseKnockdownRecoverBase:public CCombatActionBase { public:
    bool CanBeHit,Flip;
    void FrameUpdate();
};
void CCombatAction_GenericStrikeResponseKnockdownRecoverBase::FrameUpdate(){
    CCombatActionBase::FrameUpdate();
    if(MovementEventInterruptActive){Creature* creature=PCreature.GetPItem();
        if(creature->ThingType==2 && creature->RelativeMovementAcceleration.GetLength()>0.1f)FinishAction();
    }
    if(!CanBeHit){
        float percent=(float)FramesRunning*100.0f/TotalTime;
        if(percent>PCreature.GetPItem()->PThingCreatureDef->RecoverEvadeHitsPercent){
            CanBeHit=true;CTCCreatureModeManager* modes;
            PCreature.GetPItem()->GetInterface(&modes,49);modes->RemoveMode(31);
        }
    }
}