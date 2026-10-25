// CCombatManager::IsAttacker(const CThingCreatureBase& creature) const -- looks the creature's 64-bit UniqueID up in the combatant map and reports whether that entry is attacking someone (AttackerInfo.Attacked non-null)
// Leaf: ?IsAttacker@CCombatManager@@QBE_NABVCThingCreatureBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006E92C0, length 65 bytes (+ int3 padding).
// Identity (FINDINGS chunk 407 chain: 0x00662410/0x00662430 CThingCreatureBase::IsInMeleeAsAttacker/Attacked call
// 0x006E92C0/0x006E9270 on the world's combat manager; labels_chunk282 already): Ego_r.pdb
// `?IsAttacker@CCombatManager@@QBE_NABVCThingCreatureBase@@@Z` (bool, const, the creature by const reference); the
// other twin is 0x006E9270 IsAttacked. Was the descriptive IsCreatureAttacking_006e92c0.
// Callees: the combatant map's find @ 0x006F0250 (STLport red-black tree lookup on an unsigned __int64 key, auto-named
// "RedblackTree_FindLE"). The catalog label "CPersistContext::IsBinary" is propagated: the body is the one-argument twin
// of the landed CCombatManager::IsCreatureFightingThing_006e9310 (same frame, same find, same tail) and reads
// the info's AttackerInfo.Attacked at node +0x1c (value +4).
// Retail CCombatManager (Ego_r minus 4 per map): Combatants (map<unsigned __int64, CCombatantInfo>) at +0x10; the
// creature's UniqueID is CThing +0x18 (PDB). The id is copied into a frame local (the by-value GetUID() result) before
// its address is passed; the node compare against the header stays in memory (`cmp eax,[esi]`); byte result with
// explicit true/false returns; `ret 4`.
class CThingCreatureBase {                                  // the Ego_r class (mangled V)
public:
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

struct CCombatantInfo {                              // PDB CCombatantInfo with 12-byte retail containers
    CThingCreatureBase* Parent;                      // +0x00
    CThingCreatureBase* Attacked;                    // +0x04 AttackerInfo.Attacked
    unsigned char _attackerInfoRest[0x2c];                  // +0x08 AttackerInfo (0x30 bytes in retail)
    CRetailRbMap_UID_Creature Attackers;                    // +0x34 DefenderInfo.Attackers
};

struct CRetailRbMap_UID_CombatantInfo {                     // STLport map: {header, count, comparator}
    struct node {
        long  colour; node* parent; node* left; node* right;
        unsigned __int64 key;                               // +0x10
        CCombatantInfo value;                        // +0x18
    };
    node* header;                                           // +0; end() == header
    unsigned long count;
    unsigned long comp;
    node* find(const unsigned __int64& key) const;                // 0x006F0250
    node* end() const { return header; }
};

class CCombatManager {
public:
    virtual void v00();
    unsigned char _sequences[0xc];                          // +0x04 RegisteredCombatSequences
    CRetailRbMap_UID_CombatantInfo Combatants;              // +0x10
    bool IsAttacker(const CThingCreatureBase& creature) const;
};

bool CCombatManager::IsAttacker(const CThingCreatureBase& creature) const
{
    CRetailRbMap_UID_CombatantInfo::node* it = Combatants.find(creature.GetUID());
    if (it != Combatants.end() && it->value.Attacked != 0)
        return true;
    return false;
}
