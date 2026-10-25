// CCombatManager::IsCreaturePerformingNonInterruptableSequence(const CThingCreatureBase& creature) const -- whether the creature's current combat sequence is running and cannot be interrupted
// Leaf: ?IsCreaturePerformingNonInterruptableSequence@CCombatManager@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x006E97A0, 80 bytes, thiscall, `ret 4`.
// Identity: Ego_r.pdb name set `?IsCreaturePerformingNonInterruptableSequence@CCombatManager@@QBE_NABVCThingCreatureBase@@@Z`;
//   the virtual is CCombatSequenceBase slot 5 = IsInterruptable (PDB order IsInterruptable, IsInterruptableDueToZoneChange,
//   IsMeleeSequence = slots 5/6/7; the landed 0x006E9450 calls IsMeleeSequence at slot 7). Single caller 0x0093DB09.
//   The catalog's "CombatSequence_CheckActionPending" is descriptive.
// Callees: the combatant map find(const unsigned __int64&) @ 0x006F0250, CCombatSequenceBase::IsInterruptable (vtable +0x14).
// Layout: as the landed 0x006E9450: CCombatManager Combatants +0x10 (node value +0x18); CAttackingCombatantInfo
//   CurrentSequence +0x20, CombatSequences +0x24 (vector<CCountedPointer<CCombatSequenceBase> >, 8-byte elements);
//   CCombatSequenceBase Running +4.
// Shape: the inline GetCombatantInfo (the key temporary in the frame, no end() test); the sequence index is only tested
//   for >= 0; the sequence is read as a raw pointer (`mov ecx,[eax+ecx*8]`; a reference to the counted pointer adds a
//   `lea`); the PDB return type is bool (QBE_N): `return sequence->IsInterruptable() ? false : true;` gives retail's
//   byte-wide `neg al ; sbb al,al ; inc al` (as does `(bool)!x`), while a plain `return !x;` from a bool function widens
//   it to `sbb eax,eax ; inc eax` in this lane (the earlier landing used an `unsigned char` result for that reason).
class CThingCreatureBase {
public:
    unsigned char    _pad[0x18];
    unsigned __int64 UniqueID;                              // +0x18
    unsigned __int64 GetUID() const { return UniqueID; }
};

class CCombatSequenceBase {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual bool IsInterruptable() const;                   // slot 5 (+0x14)
    bool Running;                                           // +0x04
};

template <class T> struct CCountedPointer {
    T*    Data;                                             // +0x00
    void* Info;                                             // +0x04
    T* operator->() const { return Data; }
};

template <class T> struct CRetailVector {                   // retail std::vector: {first, last, end_of_storage}
    T* first;
    T* last;
    T* end_of_storage;
    T& operator[](unsigned long n) const { return first[n]; }
};

struct CAttackingCombatantInfo {                            // retail 0x30 bytes (PDB 0x34)
    const CThingCreatureBase* Attacked;                     // +0x00
    long AttackerNumber;                                    // +0x04
    long CombatWheelRingIndex;                              // +0x08
    bool PreferredAttackDollySet;                           // +0x0c
    long PreferredAttackDollyTimer;                         // +0x10
    float PreferredAttackDollyVector[2];                    // +0x14
    bool UsingCombatWheel;                                  // +0x1c
    long CurrentSequence;                                   // +0x20
    CRetailVector<CCountedPointer<CCombatSequenceBase> > CombatSequences;   // +0x24
};

struct CCombatantInfo {                                     // retail layout (Ego_r.pdb names; 12-byte vectors/maps)
    const CThingCreatureBase* Parent;                       // +0x00
    CAttackingCombatantInfo AttackerInfo;                   // +0x04
};

struct CRetailRbMap_UID_CombatantInfo {                     // STLport map<unsigned __int64, CCombatantInfo>
    struct node {
        long  colour; node* parent; node* left; node* right;
        unsigned __int64 key;                               // +0x10
        CCombatantInfo value;                               // +0x18
    };
    node* header;
    unsigned long count;
    unsigned long comp;
    node* find(const unsigned __int64& key) const;          // 0x006F0250
};

class CCombatManager {
public:
    virtual void v00();
    unsigned char _sequences[0xc];                          // +0x04
    CRetailRbMap_UID_CombatantInfo Combatants;              // +0x10
    const CCombatantInfo& GetCombatantInfo(const CThingCreatureBase& creature) const
    {
        return Combatants.find(creature.GetUID())->value;
    }
    bool IsCreaturePerformingNonInterruptableSequence(const CThingCreatureBase& creature) const;
};

bool CCombatManager::IsCreaturePerformingNonInterruptableSequence(const CThingCreatureBase& creature) const
{
    const CCombatantInfo& info = GetCombatantInfo(creature);
    if (info.AttackerInfo.CurrentSequence >= 0) {
        const CCombatSequenceBase* sequence = info.AttackerInfo.CombatSequences[info.AttackerInfo.CurrentSequence].Data;
        if (sequence->Running)
            return sequence->IsInterruptable() ? false : true;
    }
    return false;
}