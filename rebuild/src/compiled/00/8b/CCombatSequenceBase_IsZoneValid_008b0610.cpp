// CCombatSequenceBase::IsZoneValid() -- does the defender's predicted combat zone intersect this sequence's zone mask
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008B0610, length 47 bytes (+ int3 padding).
// Callees: GetFramesToPredictTargetPosition (vslot 16, `call [eax+40h]`), GetCombatManager @ 0x00661ED0
// (no arguments: the world's combat manager), CCombatManager::GetDefenderZone @ 0x006E99B0
// (defender, attacker, frames ahead), GetZone (vslot 15, `call [eax+3Ch]`).
// Argument evaluation is right to left (frames first), then the manager, then the zone mask; the bool
// is `(zone & GetZone()) != 0` (zone first: retail tests `eax, edi`) written with the ternary so VC7.1
// emits the bare `setne al`.
// Retail std::vector is {first, last, end_of_storage} (12 bytes, no allocator slot; the Ego_r
// PDB layouts carry a 16-byte vector, so every member after one sits 4 bytes lower in retail).
template<class T> struct CRetailVector {
    T* first;
    T* last;
    T* end_of_storage;
    unsigned long size() const { return (unsigned long)(last - first); }
};
class CThingCreatureBase;
class CCombatActionBase;
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

    bool IsZoneValid();
};

class CCombatManager { public:
    long GetDefenderZone(CThingCreatureBase* defender, CThingCreatureBase* attacker, long framesAhead);   // 0x006E99B0
};

CCombatManager* GetCombatManager();            // 0x00661ED0

bool CCombatSequenceBase_Retail::IsZoneValid()
{
    return (GetZone() & GetCombatManager()->GetDefenderZone(Target, Creature, GetFramesToPredictTargetPosition())) != 0 ? true : false;
}