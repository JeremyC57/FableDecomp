// CCombatManager::IsCreatureFightingThing_006e9310(const CThingCreatureBase&, const CThing*) const -- looks the creature's 64-bit UniqueID up in the combatant map and reports whether that entry's opponent is the given thing
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006E9310, length 64 bytes.
// Callees: the combatant map's find @ 0x006F0250 (`__fastcall node* (const unsigned __int64&)`, an
// STLport red-black tree lookup comparing the high dword first; auto-named "RedblackTree_FindLE").
// The manifest's "CCodeSectionManager::IsSectionRegisteredLongName" label is propagated: the only
// caller is CAIStateGroup_SummonerCharge::IsRunnable (0x0096FC0A), which passes the creature that
// just returned this CCombatManager from GetCombatManager() and a CThing it then hands to
// CCombatManager::CheckSummonerChargeable. Retail CCombatManager (Ego_r minus 4 per map):
// RegisteredCombatSequences +4, Combatants (map<unsigned __int64, CCombatantInfo>) +0x10. The
// creature's UniqueID is CThing +0x18 (PDB); the STLport node is {colour, parent, left, right} + the
// pair at +0x10, so the id is at +0x10 and the info's thing pointer at +0x1c. The id is copied into a
// local (the by-value GetUID() result) before its address is passed, which is why both dwords are
// stored to the frame first. Byte result with explicit true/false returns; the argument is the left
// operand of the pointer compare (it is loaded into a register, the node field stays in memory).
struct CThing_Calls;

struct CThingCreatureBase_Retail {
    unsigned char _pad[0x18];
    unsigned __int64 UniqueID;                              // +0x18
    unsigned __int64 GetUID() const { return UniqueID; }
};

struct CCombatantInfo_Retail {
    long          Mode;                                     // +0 (not read here)
    CThing_Calls* PTarget;                                  // +4
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
    bool IsCreatureFightingThing_006e9310(const CThingCreatureBase_Retail& creature, const CThing_Calls* pThing);
};

bool CCombatManager_Retail::IsCreatureFightingThing_006e9310(const CThingCreatureBase_Retail& creature, const CThing_Calls* pThing)
{
    CRetailRbMap_UID_CombatantInfo::node* it = Combatants.find(creature.GetUID());
    if (it != Combatants.end() && pThing == it->value.PTarget)
        return true;
    return false;
}