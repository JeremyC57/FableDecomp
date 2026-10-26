// CCombatSequenceBase::GetNextActionInSequence() -- the action at CurrentActionIndex (re-initialised), unless an incapacitated target cancels an interruptable melee sequence
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008B0780, length 94 bytes (+ int3 padding).
// Callees: IsIncapacitated @ 0x0085F0C0 (`__fastcall`, the thing in ecx), IsMeleeSequence (vslot 7,
// `call [eax+1Ch]`), IsInterruptable (vslot 5, `call [edx+14h]`), CCreatureActionBase::ReinitialiseActionID
// @ 0x00692450 on the selected action.
// The target test is one `&&` chain (non-null, not dead = bit 0 of the flag byte at +0x91, incapacitated,
// melee, interruptable) that returns null; otherwise the index is range checked as in
// IsNextActionInSequenceStrikeAction and the counted pointee is returned after its id is refreshed.
// Retail std::vector is {first, last, end_of_storage} (12 bytes, no allocator slot; the Ego_r
// PDB layouts carry a 16-byte vector, so every member after one sits 4 bytes lower in retail).
template<class T> struct CRetailVector {
    T* first;
    T* last;
    T* end_of_storage;
    unsigned long size() const { return (unsigned long)(last - first); }
};

class CThing { public:
    unsigned char _pad[0x91];
    unsigned char m_flags91;                   // +0x91 bit 0: dead
};

bool __fastcall IsIncapacitated(CThing* thing);   // 0x0085F0C0

class CCombatActionBase { public:
    void ReinitialiseActionID();               // 0x00692450 (CCreatureActionBase)
};
typedef CThing CThingCreatureBase;
struct SCountBlock;

class CCombatAction { public:                     // CCountedPointer<CCombatActionBase>, 8 bytes
    CCombatActionBase* CreatureAction;     // +0 the pointee
    SCountBlock*       m_pCount;           // +4 the shared count block
};

struct CCombatSequenceBase_Retail {        // vtable 0x01278FEC
    virtual ~CCombatSequenceBase_Retail();                      // slot 0
    virtual void OnCreate();                                    // slot 1
    virtual void GetName();                                     // slot 2 (CCharString by value in the real class)
    virtual long GetCombatSequenceType();                       // slot 3
    virtual long GetCombatSequenceInterruptionType();           // slot 4
    virtual bool IsInterruptable();                             // slot 5
    virtual bool IsInterruptableDueToZoneChange();              // slot 6
    virtual bool IsMeleeSequence();                             // slot 7
    virtual bool IsBreakIntoMeleeSequence();                    // slot 8
    virtual bool IsBoastSequence();                             // slot 9
    virtual bool IsOuterRingSequence();                         // slot 10
    virtual bool IsAllowedOnOuterRing();                        // slot 11
    virtual bool IsValid();                                     // slot 12
    virtual bool IsToPreferSequenceWhenValid();                 // slot 13
    virtual bool IsSequenceWithActions();                       // slot 14
    virtual long GetZone();                                     // slot 15 (ECombatZone bit mask)
    virtual long GetFramesToPredictTargetPosition();            // slot 16
    virtual void OnStart();                                     // slot 17
    virtual void OnStop();                                      // slot 18

    bool                         Running;              // +0x04
    long                         NoAvailableActions;   // +0x08
    long                         CurrentActionIndex;   // +0x0c
    CRetailVector<CCombatAction> CombatActions;        // +0x10 (PDB +0x10, 16 bytes there)
    CThingCreatureBase*          Creature;             // +0x1c (PDB +0x20)
    CThingCreatureBase*          Target;               // +0x20 (PDB +0x24)
    bool                         CreateCalled;         // +0x24 (PDB +0x28)

    CCombatActionBase* GetNextActionInSequence();
};

CCombatActionBase* CCombatSequenceBase_Retail::GetNextActionInSequence()
{
    CThing* target = Target;
    if (target && (target->m_flags91 & 1) == 0 && IsIncapacitated(target) && IsMeleeSequence() && IsInterruptable())
        return 0;
    long index = CurrentActionIndex;
    if (index >= 0 && (unsigned long)index < CombatActions.size()) {
        CCombatActionBase* action = CombatActions.first[index].CreatureAction;
        if (action)
            action->ReinitialiseActionID();
        return action;
    }
    return 0;
}