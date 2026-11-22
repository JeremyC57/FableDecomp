// Behaviour test for 0x0042B76C (NF228) on the shared headers engine/NUISystem_CManager.h / stlport/fable_map.h: NUISystem::CManager's
// map<EUsedKeys, long> (the key repeat counters). Under test: the tree constructor _Rb_tree(const less&, const allocator&): a new map is empty (size 0, begin == end) and usable.
// One case per condition; the out-of-line STLport rebalance is stubbed (no other model).
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_TREE_INSERT_UNIQUE_HINT_BODY
#define FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY
#include <stdio.h>
#include "engine/NUISystem_CManager.h"
namespace std { template<> void _Rb_global<bool>::_Rebalance(_Rb_tree_node_base*, _Rb_tree_node_base*&) { } }
typedef NUISystem::CManager::EUsedKeys Key;
typedef std::map<Key, long> KeyMap;
static int g_fails = 0;
static void Check(bool ok, const char* what) { if (!ok) { printf("FAIL: %s\n", what); ++g_fails; } }
int main()
{
    {
        KeyMap m;
        Check(m.size() == 0 && m.begin() == m.end(), "new map empty");
        m[NUISystem::CManager::UI_KEY_A] = 3;
        m[NUISystem::CManager::UI_KEY_LEFT] = 1;
        m[NUISystem::CManager::UI_KEY_B] = 2;
        Check(m.size() == 3 && m[NUISystem::CManager::UI_KEY_LEFT] == 1 && m[NUISystem::CManager::UI_KEY_A] == 3, "three keys");
        m.clear();
        Check(m.size() == 0 && m.begin() == m.end(), "clear non-empty");
        m.clear();
        Check(m.size() == 0 && m.begin() == m.end(), "clear empty");
        m[NUISystem::CManager::UI_KEY_UP] = 7;
        Check(m.size() == 1 && m[NUISystem::CManager::UI_KEY_UP] == 7, "reuse after clear");
    }
    printf(g_fails ? "LISTNODE_INIT_FAMILY_FAIL\n" : "LISTNODE_INIT_FAMILY_PASS\n");
    return g_fails ? 1 : 0;
}
