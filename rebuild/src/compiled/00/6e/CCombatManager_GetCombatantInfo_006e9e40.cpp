// CCombatManager::GetCombatantInfo(const CThingCreatureBase&) -- (label of the inlined helper) when the creature is attacking someone, blocks its prong on the victim's combat wheel
// Leaf: BlockAttackerProngOnVictim
// Retail: Fable.exe (TLC, VC7.1) @ 0x006E9E40, 124 bytes of code (`ret 4`). The manifest names the body after the inlined
// GetCombatantInfo lookups it is made of; the method is named descriptively.
// Callees: the combatant map's find @ 0x006F0250 (three times), CCombatWheel::BlockAttackerProng(attacker, attacker info)
//          @ 0x006E9200 (thiscall on the victim's wheel at value +0x40; label "CDefendingCombatantInfo_BlockAttackerProng").
// Shape: the creature's UniqueID is read ONCE into a local (ebx/ebp) and handed BY VALUE to the inline helpers, whose
//          parameter copies share the 8-byte frame slot whose address goes to find (helpers taking the creature by
//          reference re-read the id each time); an inline IsCreatureAttacking (find, `!= end()`, Attacked != 0) guards;
//          the attacker's info is looked up again and the call's arguments are pushed before the victim's lookup that
//          yields `this` (an inline object expression).
struct CThingCreatureBase_Retail {
    unsigned char _pad[0x18];
    unsigned __int64 UniqueID;                  // +0x18
    unsigned __int64 GetUID() const { return UniqueID; }
};

struct CCombatSequenceBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06();
    virtual bool IsMeleeSequence();             // slot 7 (+0x1c): 0x006F0700
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual bool IsValid();                     // slot 12 (+0x30): 0x008B32B0
    bool IsZoneValid();                         // 0x008B0610
};

struct CSequencePointer {                       // CCountedPointer<CCombatSequenceBase>
    CCombatSequenceBase* Object;
    void* Count;
};

template<class T> struct CRetailVector {        // retail std::vector: {first, last, end_of_storage}, 12 bytes
    T* first;
    T* last;
    T* end_of_storage;
};

struct CWorld;
struct C3DVector;
struct CCombatantInfo;

struct CCombatWheel {                           // CDefendingCombatantInfo::CCombatWheel
    unsigned char CombatRings[12];              // +0x00 vector<CCombatRing>
    void BlockAttackerProng(const CThingCreatureBase_Retail& attacker, const void* attackerInfo);   // 0x006E9200
    void ReassignAttacker(CWorld* world, CCombatantInfo* attacker, CCombatantInfo* defender, const C3DVector* position);   // 0x006E80F0
};

struct CAttackingCombatantInfo {                // retail 0x30 bytes (PDB 0x34)
    CThingCreatureBase_Retail* Attacked;        // +0x00
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
    CThingCreatureBase_Retail* Parent;          // +0x00
    CAttackingCombatantInfo AttackerInfo;       // +0x04
    unsigned char Attackers[0x0c];              // +0x34 DefenderInfo.Attackers (map)
    CCombatWheel CombatWheel;                   // +0x40 DefenderInfo.CombatWheel
    unsigned char _rest[0x70 - 0x4c];
};

struct CRetailRbMap_UID_CombatantInfo {         // STLport map<unsigned __int64, CCombatantInfo>: {header, count, comparator}
    struct node {
        long  colour; node* parent; node* left; node* right;
        unsigned __int64 key;                   // +0x10
        CCombatantInfo value;                   // +0x18
    };
    node* header;                               // +0; end() == header
    unsigned long count;
    unsigned long comp;
    node* find(const unsigned __int64& key);    // 0x006F0250
    node* end() { return header; }
};

struct CCombatManager_Retail {
    virtual void v00();
    unsigned char _sequences[0xc];              // +0x04 RegisteredCombatSequences
    CRetailRbMap_UID_CombatantInfo Combatants;  // +0x10
    CCombatantInfo& GetCombatantInfo(unsigned __int64 uid)
    {
        return Combatants.find(uid)->value;
    }
    bool IsCreatureAttacking(unsigned __int64 uid)
    {
        CRetailRbMap_UID_CombatantInfo::node* it = Combatants.find(uid);
        return it != Combatants.end() && it->value.AttackerInfo.Attacked != 0;
    }
    void BlockAttackerProngOnVictim(const CThingCreatureBase_Retail& creature);
};

void CCombatManager_Retail::BlockAttackerProngOnVictim(const CThingCreatureBase_Retail& creature)
{
    unsigned __int64 uid = creature.GetUID();
    if (IsCreatureAttacking(uid)) {
        CAttackingCombatantInfo& attackerInfo = GetCombatantInfo(uid).AttackerInfo;
        GetCombatantInfo(attackerInfo.Attacked->GetUID()).CombatWheel.BlockAttackerProng(creature, &attackerInfo);
    }
}