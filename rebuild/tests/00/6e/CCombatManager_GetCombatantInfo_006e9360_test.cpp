#include <cstdio>
// Behaviour test for 0x006e9360 CCombatManager::GetCombatantInfo const: links against the source object; the tree find
// stand-in is defined here over a small node array.
int main();
struct CCombatantInfo {                           // the mapped value (Ego_r.pdb struct CCombatantInfo; opaque here)
    long FirstWord;                                // +0x00
};

struct SCombatantNode {                            // STLport _Rb_tree_node<pair<const unsigned __int64, CCombatantInfo> >
    long             _M_color;                     // +0x00
    SCombatantNode*  _M_parent;                    // +0x04
    SCombatantNode*  _M_left;                      // +0x08
    SCombatantNode*  _M_right;                     // +0x0c
    unsigned __int64 first;                        // +0x10 the creature's UniqueID
    CCombatantInfo   second;                       // +0x18
};

struct SCombatantMap {                             // map<unsigned __int64, CCombatantInfo> (STLport _Rb_tree)
    SCombatantNode* _M_header;                     // +0x00
    SCombatantNode* FindNode(const unsigned __int64& key) const;   // 0x006F0250 (the tree's node find; stand-in name)
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
    SCombatantMap  Combatants;                     // +0x10 map<unsigned __int64, CCombatantInfo>
    const CCombatantInfo& GetCombatantInfo(const CThingCreatureBase& creature) const;   // 0x006E9360
private:
    CCombatantInfo& GetCombatantInfo(const CThingCreatureBase& creature);               // 0x006E9390
};

static unsigned __int64 g_lastKey = 0;
static SCombatantNode* g_nodes = 0; static int g_count = 0;
SCombatantNode* SCombatantMap::FindNode(const unsigned __int64& key) const
{
    g_lastKey = key;
    for (int i = 0; i < g_count; ++i)
        if (g_nodes[i].first == key) return &g_nodes[i];
    return _M_header;
}
int main()
{
    SCombatantNode header; header.second.FirstWord = -1;
    SCombatantNode nodes[3];
    for (int i = 0; i < 3; ++i) { nodes[i].first = (7ULL << 32) | (10 + i); nodes[i].second.FirstWord = 100 + i; }
    g_nodes = nodes; g_count = 3;
    static CCombatManager m; m.Combatants._M_header = &header;
    CThingCreatureBase c; c.UniqueID = (7ULL << 32) | 11;
    const CCombatantInfo& r = static_cast<const CCombatManager&>(m).GetCombatantInfo(c);
    if (&r != &nodes[1].second || r.FirstWord != 101 || g_lastKey != c.UniqueID) { std::printf("006e9360_TEST FAIL found\n"); return 1; }
    c.UniqueID = (8ULL << 32) | 11;
    const CCombatantInfo& r2 = static_cast<const CCombatManager&>(m).GetCombatantInfo(c);
    if (&r2 != &header.second) { std::printf("006e9360_TEST FAIL end\n"); return 1; }
    std::printf("006e9360_TEST PASS\n");
    return 0;
}
