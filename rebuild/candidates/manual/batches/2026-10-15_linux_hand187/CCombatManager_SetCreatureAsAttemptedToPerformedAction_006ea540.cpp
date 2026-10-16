// CCombatManager::SetCreatureAsAttemptedToPerformedAction(const CThingCreatureBase& creature, bool performed) -- report an attempted action to the creature's current combat sequence: performed -> finish it and notify; not performed -> finish it unless it can continue, else (still running) reset the attacker's sequence
// Leaf: SetCreatureAsAttemptedToPerformedAction
// Retail: Fable.exe (TLC, VC7.1) @ 0x006EA540, 169 bytes (176 with padding), `ret 8`.
// Callees: the creature test 0x00662410, the combatant map's find 0x006F0250, CCombatSequenceBase::OnActionPerformed
//   0x008B0600, CCombatManager 0x006EA2C0 (thiscall, the creature; resets the attacker's sequence), the sequence's virtual
//   CanContinue (slot 14, +0x38) and OnFinish (slot 17, +0x44).
// Layout: as the landed 0x006E9E40 (Combatants +0x10; AttackerInfo.CurrentSequence +0x20, CombatSequences +0x24 of
//   8-byte counted pointers); the sequence's finished flag byte +4.
// Shape: the inline Finish() sets the flag before the virtual call; the flag is read once after CanContinue for both
//   branches; CurrentSequence >= 0 is the `jl` guard.
template<class T> struct CRetailVector {        // retail std::vector: {first, last, end_of_storage}, 12 bytes
    T* first;
    T* last;
    T* end_of_storage;
    T& operator[](unsigned long n) const { return first[n]; }
};

struct CThingCreatureBase {
    unsigned char _pad[0x18];
    unsigned __int64 UniqueID;                  // +0x18
    unsigned __int64 GetUID() const { return UniqueID; }
    bool IsCombatEligible() const;              // 0x00662410
};

struct CCombatSequenceBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13();
    virtual bool CanContinue();                 // slot 14 (+0x38)
    virtual void v15(); virtual void v16();
    virtual void OnFinish();                    // slot 17 (+0x44)
    bool m_finished;                            // +0x04
    void Finish()
    {
        if (!m_finished) {
            m_finished = true;
            OnFinish();
        }
    }
    void OnActionPerformed();                   // 0x008B0600
};

struct CSequencePointer {                       // CCountedPointer<CCombatSequenceBase>
    CCombatSequenceBase* Object;
    void* Count;
};

struct CAttackingCombatantInfo {                // retail 0x30 bytes (PDB 0x34)
    const CThingCreatureBase* Attacked;         // +0x00
    long AttackerNumber;                        // +0x04
    long CombatWheelRingIndex;                  // +0x08
    bool PreferredAttackDollySet;               // +0x0c
    long PreferredAttackDollyTimer;             // +0x10
    float PreferredAttackDollyVector[2];        // +0x14
    bool UsingCombatWheel;                      // +0x1c
    long CurrentSequence;                       // +0x20
    CRetailVector<CSequencePointer> CombatSequences;   // +0x24
};

struct CCombatantInfo {                         // retail layout (Ego_r.pdb names; 12-byte vectors/maps)
    const CThingCreatureBase* Parent;           // +0x00
    CAttackingCombatantInfo AttackerInfo;       // +0x04
    unsigned char _rest[0x70 - 0x34];
};

struct CRetailRbMap_UID_CombatantInfo {         // STLport map<unsigned __int64, CCombatantInfo>
    struct node {
        long  colour; node* parent; node* left; node* right;
        unsigned __int64 key;                   // +0x10
        CCombatantInfo value;                   // +0x18
    };
    node* header;
    unsigned long count;
    unsigned long comp;
    node* find(const unsigned __int64& key);    // 0x006F0250
};

struct CCombatManager {
    virtual void v00();
    unsigned char _sequences[0xc];              // +0x04
    CRetailRbMap_UID_CombatantInfo Combatants;  // +0x10
    CCombatantInfo& GetCombatantInfo(unsigned __int64 uid)
    {
        return Combatants.find(uid)->value;
    }
    void ResetAttackerSequence(const CThingCreatureBase& creature);   // 0x006EA2C0
    void SetCreatureAsAttemptedToPerformedAction(const CThingCreatureBase& creature, bool performed);
};

void CCombatManager::SetCreatureAsAttemptedToPerformedAction(const CThingCreatureBase& creature, bool performed)
{
    if (!creature.IsCombatEligible())
        return;
    CCombatantInfo& info = GetCombatantInfo(creature.GetUID());
    if (info.AttackerInfo.CurrentSequence < 0)
        return;
    CCombatSequenceBase* sequence = info.AttackerInfo.CombatSequences[info.AttackerInfo.CurrentSequence].Object;
    if (performed) {
        sequence->Finish();
        sequence->OnActionPerformed();
    }
    else if (!sequence->CanContinue())
        sequence->Finish();
    else if (!sequence->m_finished)
        ResetAttackerSequence(creature);
}
