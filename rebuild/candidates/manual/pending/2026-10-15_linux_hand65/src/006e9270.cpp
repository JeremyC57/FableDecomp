// CCombatManager::IsCreatureBeingAttacked_006e9270(const CThingCreatureBase& creature) -- looks the creature's 64-bit UniqueID up in the combatant map and reports whether that entry has attackers (DefenderInfo.Attackers non-empty)
// Leaf: IsCreatureBeingAttacked_006e9270
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006E9270, length 65 bytes (+ int3 padding).
// Callees: the combatant map's find @ 0x006F0250 (STLport red-black tree lookup on an unsigned __int64 key, auto-named
// "RedblackTree_FindLE"). The catalog label "CPersistContext::IsBinary" is propagated: the body is the one-argument twin
// of the landed CCombatManager::IsCreatureFightingThing_006e9310 (same frame, same find, same tail) and reads
// the count of the info's DefenderInfo.Attackers map at node +0x50 (value +0x38: retail AttackerInfo is 0x30 bytes, so DefenderInfo sits at +0x34 and the STLport map count at +0x38).
// Retail CCombatManager (Ego_r minus 4 per map): Combatants (map<unsigned __int64, CCombatantInfo>) at +0x10; the
// creature's UniqueID is CThing +0x18 (PDB). The id is copied into a frame local (the by-value GetUID() result) before
// its address is passed; the node compare against the header stays in memory (`cmp eax,[esi]`); byte result with
// explicit true/false returns; `ret 4`.
struct CThingCreatureBase_Retail {
    unsigned char _pad[0x18];
    unsigned __int64 UniqueID;                              // +0x18
    unsigned __int64 GetUID() const { return UniqueID; }
};

struct CRetailRbMap_UID_Creature {                          // STLport map<unsigned __int64, CThingCreatureBase*>
    void*         header;                                   // +0
    unsigned long count;                                    // +4
    unsigned long comp;                                     // +8
    unsigned long size() const { return count; }
};

struct CCombatantInfo_Retail {                              // PDB CCombatantInfo with 12-byte retail containers
    CThingCreatureBase_Retail* Parent;                      // +0x00
    CThingCreatureBase_Retail* Attacked;                    // +0x04 AttackerInfo.Attacked
    unsigned char _attackerInfoRest[0x2c];                  // +0x08 AttackerInfo (0x30 bytes in retail)
    CRetailRbMap_UID_Creature Attackers;                    // +0x34 DefenderInfo.Attackers
};

struct CRetailRbMap_UID_CombatantInfo {                     // STLport map: {header, count, comparator}
    struct node {
        long  colour; node* parent; node* left; node* right;
        unsigned __int64 key;                               // +0x10
        CCombatantInfo_Retail value;                        // +0x18
    };
    node* header;                                           // +0; end() == header
    unsigned long count;
    unsigned long comp;
    node* find(const unsigned __int64& key);                // 0x006F0250
    node* end() { return header; }
};

struct CCombatManager_Retail {
    virtual void v00();
    unsigned char _sequences[0xc];                          // +0x04 RegisteredCombatSequences
    CRetailRbMap_UID_CombatantInfo Combatants;              // +0x10
    bool IsCreatureBeingAttacked_006e9270(const CThingCreatureBase_Retail& creature);
};

bool CCombatManager_Retail::IsCreatureBeingAttacked_006e9270(const CThingCreatureBase_Retail& creature)
{
    CRetailRbMap_UID_CombatantInfo::node* it = Combatants.find(creature.GetUID());
    if (it != Combatants.end() && it->value.Attackers.size() != 0)
        return true;
    return false;
}
