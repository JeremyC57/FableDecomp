// CCombatManager::UpdateAttackingCombatantSequenceResets(CCombatantInfo* info) -- reset the attacker's combat sequence when the current one has run its course (and let the creature finish its action), or when the sequence asks for a reset and the attacked thing can no longer be attacked
// Retail: Fable.exe (TLC, VC7.1) @ 0x006EABB0, 146 bytes (160 with padding).
// Callees: the current sequence's vtable slots 6 (+0x18), 7 (+0x1c) and 5 (+0x14) (bool queries),
//          CCombatSequenceBase::IsZoneValid 0x008B0610, CCombatManager::PerformSequenceReset 0x006EA2C0,
//          CThingCreatureBase::IsPerformingAnyAction 0x006622A0 / IsFreeToFinishAction 0x006621C0 /
//          FinishCurrentAction 0x00662650, IsAbleToBeAttacked 0x0085FB30 (`__fastcall`, the thing in ecx),
//          CCombatManager::UpdateCurrentAction(CThingCreatureBase*) 0x006E8400 (label propagated).
//
// Shape: CCombatantInfo in the retail layout (Parent +0, AttackerInfo.Attacked +4, CurrentSequence +0x24,
// the CombatSequences vector of 8-byte counted pointers with first at +0x28). The three values are read
// into callee-saved registers up front (parent edi, attacked ebp, sequence esi). Two exits: the first
// condition's body returns, the second is the fall-through `if`.
class CThing;

class CThingCreatureBase { public:
    bool IsFreeToFinishAction();            // 0x006621C0
    void FinishCurrentAction();             // 0x00662650
public:
    bool IsPerformingAnyAction() const;   // 0x006622A0
};

bool __fastcall IsAbleToBeAttacked(CThing* thing);   // 0x0085FB30

class CCombatSequenceBase { public:
    virtual void v000();
    virtual void v001();
    virtual void v002();
    virtual void v003();
    virtual void v004();
    virtual bool IsReadyToReset();                       // slot 5 (+0x14)
    virtual bool IsFinished();                           // slot 6 (+0x18)
    virtual bool IsResetOnTargetLost();                  // slot 7 (+0x1c)
public:
    bool IsZoneValid() const;   // 0x008B0610
};

struct CCountedSequencePointer {            // CCountedPointer<CCombatSequenceBase>
    CCombatSequenceBase* m_pObject;
    void* m_pCount;
};

struct CCombatantInfo {
    CThingCreatureBase* Parent;             // +0x00
    CThing* Attacked;                       // +0x04 AttackerInfo.Attacked
    unsigned char _attacker08[0x1c];        // +0x08 AttackerNumber .. UsingCombatWheel
    long CurrentSequence;                   // +0x24
    CCountedSequencePointer* CombatSequences_first;   // +0x28 (retail 12-byte vector)
    CCountedSequencePointer* CombatSequences_last;
    CCountedSequencePointer* CombatSequences_end;
};

class CCombatManager { public:
    void PerformSequenceReset(CThingCreatureBase* creature);   // 0x006EA2C0
    void UpdateCurrentAction(CThingCreatureBase* creature);    // 0x006E8400
    void UpdateAttackingCombatantSequenceResets(CCombatantInfo* info);
};

void CCombatManager::UpdateAttackingCombatantSequenceResets(CCombatantInfo* info)
{
    CThingCreatureBase* parent = info->Parent;
    CThing* attacked = info->Attacked;
    CCombatSequenceBase* sequence = info->CombatSequences_first[info->CurrentSequence].m_pObject;
    if (sequence->IsFinished() && !sequence->IsZoneValid()) {
        PerformSequenceReset(parent);
        if (parent->IsPerformingAnyAction() && parent->IsFreeToFinishAction())
            parent->FinishCurrentAction();
        return;
    }
    if (sequence->IsResetOnTargetLost() && sequence->IsReadyToReset() && !IsAbleToBeAttacked(attacked)) {
        PerformSequenceReset(parent);
        UpdateCurrentAction(parent);
    }
}