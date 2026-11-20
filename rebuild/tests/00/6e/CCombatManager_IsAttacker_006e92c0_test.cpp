// Test for 0x006e92c0 CCombatManager::IsAttacker(const CThingCreatureBase&) const: links the source object; the combatant
// map is the real STLport map<unsigned __int64, CCombatantInfo> (stlport/fable_map.h, NF195 2026-11-20 q4389: the
// CRetailRbMap_UID_CombatantInfo find stub is gone; this TU emits the _M_find<unsigned __int64> body the row calls) and is
// searched by the creature's 64-bit UniqueID; unknown creatures answer false, known ones answer from their combatant info
// (AttackerInfo.Attacked for IsAttacker, the DefenderInfo.Attackers count for IsAttacked).
#include <stdio.h>
#include <string.h>
#define FABLE_RETAIL_STLP_TREE_MFIND_NOINLINE
#define FABLE_RETAIL_STLP_TREE_CONSTFIND
#define FABLE_RETAIL_STLP_MAP                         // the map insert / node bodies the test needs
#define FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY
#include "stlport/fable_map.h"
namespace std {
// Test-only body of the out-of-line STLport _Rb_global<bool>::_Rebalance (retail 0x0042971D, not under test): _M_insert has
// already linked the node, so the two-node tree stays a valid (unbalanced) search tree for find.
template<> void __cdecl _Rb_global<bool>::_Rebalance(_Rb_tree_node_base*, _Rb_tree_node_base*&) { }
}
class CThingCreatureBase {
public:
    unsigned char _pad[0x18];
    unsigned __int64 UniqueID;
};
struct CRetailRbMap_UID_Creature { void* header; unsigned long count; unsigned long comp; };
struct CCombatantInfo {
    CThingCreatureBase* Parent;
    CThingCreatureBase* Attacked;
    unsigned char _attackerInfoRest[0x2c];
    CRetailRbMap_UID_Creature Attackers;
};
class CCombatManager {
public:
    virtual void v00();
    unsigned char _sequences[0xc];
    std::map<unsigned __int64, CCombatantInfo> Combatants;
    bool IsAttacker(const CThingCreatureBase& creature) const;
};
void CCombatManager::v00() { }
int main()
{
    CThingCreatureBase c[3];
    memset(c, 0, sizeof(c));
    c[0].UniqueID = (((unsigned __int64)1) << 32) | 1;
    c[1].UniqueID = (((unsigned __int64)2) << 32) | 2;
    c[2].UniqueID = (((unsigned __int64)3) << 32) | 3;
    CCombatantInfo i0, i1; memset(&i0, 0, sizeof(i0)); memset(&i1, 0, sizeof(i1));
    i0.Attacked = &c[1]; i1.Attacked = 0;
    CCombatManager m;
    m.Combatants.insert(std::pair<const unsigned __int64, CCombatantInfo>(c[0].UniqueID, i0));
    m.Combatants.insert(std::pair<const unsigned __int64, CCombatantInfo>(c[1].UniqueID, i1));
    const CCombatManager& cm = m;
    const std::map<unsigned __int64, CCombatantInfo>& cmap = m.Combatants;   // the const find calls _M_find (emitted here)
    bool ok = m.Combatants.size() == 2 && cmap.find(c[1].UniqueID) != cmap.end() && cmap.find(c[2].UniqueID) == cmap.end() && cm.IsAttacker(c[0]) && !cm.IsAttacker(c[1]) && !cm.IsAttacker(c[2]);
    if (!ok) { puts("006e92c0_TEST FAIL"); return 1; }
    puts("006e92c0_TEST PASS");
    return 0;
}
