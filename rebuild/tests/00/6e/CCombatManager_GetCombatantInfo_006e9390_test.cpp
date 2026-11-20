#include <stdio.h>
// Behaviour test for 0x006e9390 CCombatManager::GetCombatantInfo (private): links against the source object; the combatant map is the real
// STLport map<unsigned __int64, CCombatantInfo> (stlport/fable_map.h, NF199 2026-11-20 q4389: the SCombatantMap::FindNode stub is gone;
// this TU emits the _M_find<unsigned __int64> body the row calls). A known creature gets its own info, an unknown one the end node's.
#define FABLE_RETAIL_STLP_TREE_MFIND_NOINLINE
#define FABLE_RETAIL_STLP_TREE_CONSTFIND
#define FABLE_RETAIL_STLP_MAP                         // the map insert / node bodies the test needs
#define FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY
#include "stlport/fable_map.h"
namespace std {
// Test-only body of the out-of-line STLport _Rb_global<bool>::_Rebalance (retail 0x0042971D, not under test): _M_insert has
// already linked the node, so the small tree stays a valid (unbalanced) search tree for find.
template<> void __cdecl _Rb_global<bool>::_Rebalance(_Rb_tree_node_base*, _Rb_tree_node_base*&) { }
}
int main();
struct CCombatantInfo {                           // the mapped value (Ego_r.pdb struct CCombatantInfo; opaque here)
    long FirstWord;                                // +0x00
};
class CThing {
public:
    unsigned char    _pad00[0x18];
    unsigned __int64 UniqueID;                     // +0x18 (Ego_r.pdb CThing::UniqueID)
};
class CThingCreatureBase : public CThing { };

class CCombatManager {
public:
    unsigned char  _pad00[0x10];                   // +0x00 vptr, +0x04 RegisteredCombatSequences (STLport map, 12 bytes)
    std::map<unsigned __int64, CCombatantInfo> Combatants;   // +0x10
    const CCombatantInfo& GetCombatantInfo(const CThingCreatureBase& creature) const;   // 0x006E9360
private:
    friend int ::main();
    CCombatantInfo& GetCombatantInfo(const CThingCreatureBase& creature);               // 0x006E9390
};

int main()
{
    static CCombatManager m;
    for (int i = 0; i < 3; ++i) {
        CCombatantInfo info; info.FirstWord = 100 + i;
        m.Combatants.insert(std::pair<const unsigned __int64, CCombatantInfo>((7ULL << 32) | (10 + i), info));
    }
    const std::map<unsigned __int64, CCombatantInfo>& cmap = m.Combatants;   // the const find calls _M_find (emitted here)
    const CCombatantInfo* found = &cmap.find((7ULL << 32) | 11)->second;
    const CCombatantInfo* endInfo = &cmap.find(5ULL)->second;
    CThingCreatureBase c; c.UniqueID = (7ULL << 32) | 11;
    CCombatantInfo& r = m.GetCombatantInfo(c);
    if (&r != found || r.FirstWord != 101 || m.Combatants.size() != 3) { printf("006e9390_TEST FAIL found\n"); return 1; }
    c.UniqueID = (8ULL << 32) | 11;
    CCombatantInfo& r2 = m.GetCombatantInfo(c);
    if (&r2 != endInfo) { printf("006e9390_TEST FAIL end\n"); return 1; }
    printf("006e9390_TEST PASS\n");
    return 0;
}
