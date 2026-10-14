// CCombatSequenceBase::IsNextActionInSequenceStrikeAction() -- is the action at CurrentActionIndex a strike action
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008B0750, length 41 bytes (+ int3 padding).
// Callees: CCombatActionBase vslot 16 (`call [edx+40h]`, IsStrikeAction) on the counted pointee.
// A negative or out-of-range index (unsigned compare against `(last - first) >> 3`) is false; the
// three conditions are one `&&` chain behind `if (..) return true; return false;` (a `return a && b` would
// widen the result to eax).
// Retail std::vector is {first, last, end_of_storage} (12 bytes, no allocator slot; the Ego_r
// PDB layouts carry a 16-byte vector, so every member after one sits 4 bytes lower in retail).
template<class T> struct CRetailVector {
    T* first;
    T* last;
    T* end_of_storage;
    unsigned long size() const { return (unsigned long)(last - first); }
};

struct CCombatActionBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual bool IsStrikeAction();                 // slot 16
};
struct CThingCreatureBase;
struct SCountBlock;

struct CCombatAction {                     // CCountedPointer<CCombatActionBase>, 8 bytes
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

    bool IsNextActionInSequenceStrikeAction();
};

bool CCombatSequenceBase_Retail::IsNextActionInSequenceStrikeAction()
{
    long index = CurrentActionIndex;
    if (index >= 0 && (unsigned long)index < CombatActions.size()
        && CombatActions.first[index].CreatureAction->IsStrikeAction())
        return true;
    return false;
}
