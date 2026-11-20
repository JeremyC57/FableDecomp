// CCombatManager::GetNumberOfAttackers(CThingCreatureBase*) -- for a registered combatant, the node count of its defender-side Attackers map (looked up by the creature's unique id); 0 for a non-combatant
// Leaf: ?GetNumberOfAttackers@CCombatManager@@QAEJPAVCThingCreatureBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006E9960, length 70 bytes (+ int3 padding). No callers or
// vtable references (dead in the shipped build).
// Callees: CThingCreatureBase "is combatant" @ 0x00662430 (asks the world's combat manager; landed under the
// propagated label C3DAnimationBlendState::AddCombinationBlend), map<unsigned __int64, CCombatantInfo>
// node lookup @ 0x006F0250 (STLport _M_find: returns the node or the header; manifest
// "RedblackTree_FindLE"). The chunk label "CTCCombat::SetPossibleQueuedAction" is propagated: `this` is
// the combat manager (the map at +0x10, cf. the landed CCombatManager::DeregisterCombatant 0x006EA430).
// Layout (retail, below the PDB): CCombatManager::Combatants +0x10 (PDB +0x14: the retail map before it
// is 12 bytes); CThing::UniqueID +0x18; tree node = colour/parent/left/right, key +0x10, CCombatantInfo
// +0x18, whose word at +0x38 is DefenderInfo.Attackers' node count (retail CAttackingCombatantInfo is
// 0x30 bytes with its 12-byte vector; the STLport map is {header, count, compare}). The name is inferred.
// Shape: the success path is the `if` body (the `xor eax,eax` exit sits at the end); the 64-bit key is a
// local copied high word first into the 8-byte frame and passed by const reference.
// Name fix (NF199, 2026-11-20, q4389): the invented CCombatantMap / SCombatantNode stand-ins are replaced by the STLport
//   map<unsigned __int64, CCombatantInfo> (stlport/fable_map.h); find's out-of-line _M_find<unsigned __int64> 0x006F0250 is
//   declared only (FABLE_RETAIL_STLP_TREE_MFIND_DECL), so the call binds the self row's member-template spelling.
#define FABLE_RETAIL_STLP_TREE_MFIND_DECL
#include "stlport/fable_map.h"
class CThingCreatureBase { public:
    unsigned char    _pad[0x18];
    unsigned __int64 UniqueID;                              // +0x18 (CThing)
public:
    bool IsInMeleeAsAttacked() const;   // 0x00662430
};

struct CAttackerMap {                                       // STLport map<unsigned __int64, CThingCreatureBase*>
    void*         Header;
    unsigned long Count;
    unsigned char Compare[4];
};

struct CCombatantInfo {
    unsigned char _pad[0x34];                               // Parent, AttackerInfo (retail 0x30 bytes)
    CAttackerMap  Attackers;                                // +0x34 DefenderInfo.Attackers
};

class CCombatManager { public:
    void*         __vftable;                                // +0x00
    unsigned char RegisteredCombatSequences[0xc];           // +0x04
    std::map<unsigned __int64, CCombatantInfo> Combatants;  // +0x10
    long GetNumberOfAttackers(CThingCreatureBase* creature);
};

long CCombatManager::GetNumberOfAttackers(CThingCreatureBase* creature)
{
    if (creature->IsInMeleeAsAttacked()) {
        unsigned __int64 id = creature->UniqueID;
        return Combatants.find(id)->second.Attackers.Count;
    }
    return 0;
}