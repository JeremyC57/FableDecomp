// CAIStateGroup_MeleeCombatNearbyBase::UpdateCurrentAction() -- let the creature finish its current action
// once it is free to do so.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0093D140, length 47 bytes (+ int3 padding).
// Callees on the brain's creature (PBrain->Creature, re-read for every call): IsPerformingAnyAction
// @ 0x006622A0, IsFreeToFinishAction @ 0x006621C0, FinishCurrentAction @ 0x00662650 (tail jump).
// Shape: `if (a() && b()) c();` with the creature read through PBrain each time (no local), which is what
// gives the three separate `mov [esi+4] ; mov [+18h]` pairs and the sibling-call `jmp` at the end.
class CThingCreatureBase { public:
    bool IsPerformingAnyAction();           // 0x006622A0
    bool IsFreeToFinishAction();            // 0x006621C0
    void FinishCurrentAction();             // 0x00662650
};

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThingCreatureBase* Creature;           // +0x18
};

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                       // +0x04
    unsigned char _pad[0xc];                // +0x08..+0x13
};

struct CAIStateGroup_MeleeCombatNearbyBase : CAIStateGroupBase {
    unsigned char PEnemy[8];                // +0x14 CIntelligentPointer<CThingCreatureBase>
    void UpdateCurrentAction();
};

void CAIStateGroup_MeleeCombatNearbyBase::UpdateCurrentAction()
{
    if (PBrain->Creature->IsPerformingAnyAction() && PBrain->Creature->IsFreeToFinishAction())
        PBrain->Creature->FinishCurrentAction();
}