// CCombatSequenceGeneric::IsSequenceWithActions() -- true when the def's ActionList holds any non-zero registered action type
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008B06B0, length 34 bytes (+ int3 padding). No callees.
// Vtable 0x01261D74 slot 14 (rtti-vtables.tsv). The def pointer is at +0x28 in retail (PDB +0x2c: the
// base's vector is 16 bytes there, 12 here) and the def's ActionList vector<EActionRegisteredType> at
// +0x54 (PDB +0x58, the same one-container drift). The loop keeps `last` in a register since nothing in
// it stores to memory; an early `return true` gives the two bool tails.
// Retail std::vector is {first, last, end_of_storage} (12 bytes, no allocator slot; the Ego_r
// PDB layouts carry a 16-byte vector, so every member after one sits 4 bytes lower in retail).
template<class T> struct CRetailVector {
    T* first;
    T* last;
    T* end_of_storage;
    unsigned long size() const { return (unsigned long)(last - first); }
};
struct CThingCreatureBase;
struct CCombatActionBase;
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
};
struct CCombatSequenceDef_Retail {
    unsigned char       _pad_0x00[0x3c];
    long                ValidCombatZones;              // +0x3c (PDB +0x40)
    long                Type;                          // +0x40 ECombatSequenceType
    long                InterruptionType;              // +0x44 ECombatSequenceInterruptionType
    long                IsValidType;                   // +0x48 ECombatSequenceIsValidType (PDB +0x4c)
    long                OnStartType;                   // +0x4c ECombatSequenceOnStartModuleType
    long                OnStopType;                    // +0x50 ECombatSequenceOnStopModuleType
    CRetailVector<long> ActionList;                    // +0x54 vector<EActionRegisteredType>
};
struct CCombatSequenceGeneric_Retail : CCombatSequenceBase_Retail {   // vtable 0x01261D74
    const CCombatSequenceDef_Retail* Def_Object;       // +0x28 CDefPointer<CCombatSequenceDef const>::Object (PDB +0x2c)
    virtual bool IsSequenceWithActions();
};

bool CCombatSequenceGeneric_Retail::IsSequenceWithActions()
{
    const CCombatSequenceDef_Retail* def = Def_Object;
    for (const long* action = def->ActionList.first; action != def->ActionList.last; ++action) {
        if (*action != 0)
            return true;
    }
    return false;
}