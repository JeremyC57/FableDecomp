// CCombatSequenceGeneric::IsValid() -- dispatch to the def's registered IsValid module, if any
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008B07E0, length 68 bytes (+ int3 padding).
// Callees: the module map's find @ 0x008B2400 (the landed CRetailTree_longkey::find shape, iterator through
// a hidden pointer, key by reference), then the stored `__fastcall` module function (creature in ecx,
// target in edx).
// Vtable 0x01261D74. The def (+0x28 in retail) carries the module type at +0x48 (PDB +0x4c); a zero
// type means "no module" and the sequence counts as valid. The found node is tested against the map head (end()) and
// for a null function before the call; the one-dword frame holds the iterator.
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
typedef bool (__fastcall* FnModule)(CThingCreatureBase* creature, CThingCreatureBase* target);

struct CRetailTree_longkey {                      // std::map<long, IsValid module>
    struct node {
        unsigned char flag; unsigned char _pad[3];
        node* parent;                          // +0x04
        node* left;                            // +0x08
        node* right;                           // +0x0c
        long  key;                             // +0x10
        FnModule value;                              // +0x14
    };
    struct iterator { node* current; iterator(node* n) : current(n) { } };
    node*         head;                        // +0 (end() node)
    unsigned long size;                        // +4
    iterator find_008b2400(const long& key);            // 0x008B2400
};

extern CRetailTree_longkey g_IsValidModules;                 // 0x013BAFA8
struct CCombatSequenceGeneric_Retail : CCombatSequenceBase_Retail {   // vtable 0x01261D74
    const CCombatSequenceDef_Retail* Def_Object;       // +0x28 CDefPointer<CCombatSequenceDef const>::Object (PDB +0x2c)
    virtual bool IsValid();
};

bool CCombatSequenceGeneric_Retail::IsValid()
{
    const CCombatSequenceDef_Retail* def = Def_Object;
    if (def->IsValidType != 0) {
        CRetailTree_longkey::iterator it = g_IsValidModules.find_008b2400(def->IsValidType);
        if (it.current != g_IsValidModules.head && it.current->value)
            return it.current->value(Creature, Target);
    }
    return true;
}