// Behaviour test for 0x0042A879 (NF228) on the shared headers engine/NUISystem_CManager.h / stlport/fable_map.h: NUISystem::CManager's
// map<long, EEngine2DLayer> (Layers). Under test: the tree's _M_find<long> (map::find): hit, misses below / between / above the keys, and on an empty tree.
// One case per condition; the out-of-line STLport rebalance is stubbed (no other model).
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_TREE_INSERT_UNIQUE_HINT_BODY
#define FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY
#include <stdio.h>
#include "engine/NUISystem_CManager.h"
// Stub of the out-of-line STLport rebalance (a landed row of its own): the tree stays a valid, unbalanced search tree.
namespace std { template<> void _Rb_global<bool>::_Rebalance(_Rb_tree_node_base*, _Rb_tree_node_base*&) { } }
typedef std::map<long, EEngine2DLayer> LayerMap;
static int g_fails = 0;
static void Check(bool ok, const char* what) { if (!ok) { printf("FAIL: %s\n", what); ++g_fails; } }
int main()
{
    {
        LayerMap m;
        Check(m.size() == 0 && m.begin() == m.end(), "new map empty");
        Check(m.find(5) == m.end(), "find on empty");
        m[-3] = ENGINE_2D_LAYER_M3;
        m[13] = ENGINE_2D_LAYER_13;
        m[0] = ENGINE_2D_LAYER_0;
        Check(m.size() == 3, "three inserts");
        Check(m[0] == ENGINE_2D_LAYER_0 && m.size() == 3, "operator[] on a present key");
        LayerMap::iterator it = m.find(13);
        Check(it != m.end() && (*it).second == ENGINE_2D_LAYER_13, "find hit");
        Check(m.find(7) == m.end() && m.find(-4) == m.end() && m.find(14) == m.end(), "find misses");
        Check(m[7] == (EEngine2DLayer)0 && m.size() == 4, "operator[] inserts the default value");
        m.clear();
        Check(m.size() == 0 && m.begin() == m.end(), "clear non-empty");
        m.clear();
        Check(m.size() == 0 && m.begin() == m.end(), "clear empty");
        m[1] = ENGINE_2D_LAYER_1;
        Check(m.size() == 1 && m[1] == ENGINE_2D_LAYER_1, "reuse after clear");
    }
    printf(g_fails ? "0042a879_TEST FAIL\n" : "0042a879_TEST PASS\n");
    return g_fails ? 1 : 0;
}
