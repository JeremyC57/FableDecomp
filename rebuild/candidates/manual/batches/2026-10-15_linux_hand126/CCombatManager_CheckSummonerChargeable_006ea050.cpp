// CCombatManager::CheckSummonerChargeable(CThingCreatureBase* summoner) -- false unless the summoner is a registered combatant that currently has attackers; then the verdict of the chargeability test on its combatant info
// Retail: Fable.exe (TLC, VC7.1) @ 0x006EA050, 106 bytes (112 with padding).
// Callees: the combatant map's find @ 0x006F0250 (STLport red-black tree lookup on the unsigned __int64
//          UniqueID; called twice), CCombatManager::IsDefenderDueForAttack(CCombatantInfo*) @ 0x006E97F0.
//
// Shape: the landed CCombatManager rows' model (IsCreatureBeingAttacked 0x006E9270): Combatants at +0x10,
// node value (CCombatantInfo) at node +0x18, its DefenderInfo.Attackers count at value +0x38. The id
// is read ONCE into a register pair (ebx:ebp) and re-stored into the frame slot before each find: each
// lookup goes through an inline helper that takes the key BY VALUE (its parameter is the frame
// temporary whose address find receives). The second lookup is unchecked (`add eax,18h`).
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
    unsigned char _attackerInfo[0x30];                      // +0x04 AttackerInfo (0x30 bytes in retail)
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
    node* Find(unsigned __int64 key) { return find(key); }
};

struct CCombatManager_Retail {
    virtual void v00();
    unsigned char _sequences[0xc];                          // +0x04 RegisteredCombatSequences
    CRetailRbMap_UID_CombatantInfo Combatants;              // +0x10
    bool IsDefenderDueForAttack(CCombatantInfo_Retail* info);   // 0x006E97F0
    bool CheckSummonerChargeable(CThingCreatureBase_Retail* summoner);
};

bool CCombatManager_Retail::CheckSummonerChargeable(CThingCreatureBase_Retail* summoner)
{
    unsigned __int64 id = summoner->GetUID();
    CRetailRbMap_UID_CombatantInfo::node* it = Combatants.Find(id);
    if (it != Combatants.end() && it->value.Attackers.size() != 0)
        return IsDefenderDueForAttack(&Combatants.Find(id)->value);
    return false;
}
