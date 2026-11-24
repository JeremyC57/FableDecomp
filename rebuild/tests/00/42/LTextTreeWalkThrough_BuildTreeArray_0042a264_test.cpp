// Behaviour test for 0x0042A264 (NF229) on the shared headers engine/NUISystem_CManager.h / stlport/fable_map.h: NUISystem::CManager's
// map<long, EEngine2DLayer>. Under test: the tree _M_erase (reached through clear()): a one-node tree, and a tree whose root has a right
// subtree and a left chain (insert order 5, 9, 2, 1, 7 on the unbalanced tree: the right recursion and the left continuation both run),
// end empty and reusable. One case per condition; the out-of-line STLport rebalance is stubbed (no other model).
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_TREE_INSERT_UNIQUE_HINT_BODY
#define FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY
#include <stdio.h>
#include "engine/NUISystem_CManager.h"
// Stub of the out-of-line STLport rebalance (a landed row of its own): the tree stays a valid, unbalanced search tree.
namespace std { template<> void _Rb_global<bool>::_Rebalance(_Rb_tree_node_base*, _Rb_tree_node_base*&) { } }
static int g_fails = 0;
static void Check(bool ok, const char* what) { if (!ok) { printf("FAIL: %s\n", what); ++g_fails; } }
typedef std::map<long, EEngine2DLayer> LayerMap;
int main()
{
    {
        LayerMap m;
        m[4] = ENGINE_2D_LAYER_1;
        Check(m.size() == 1, "one node");
        m.clear();
        Check(m.size() == 0 && m.begin() == m.end(), "erase one node");
        m[5] = ENGINE_2D_LAYER_0; m[9] = ENGINE_2D_LAYER_1; m[2] = ENGINE_2D_LAYER_13; m[1] = ENGINE_2D_LAYER_M3; m[7] = ENGINE_2D_LAYER_0;
        Check(m.size() == 5, "five nodes");
        m.clear();
        Check(m.size() == 0 && m.begin() == m.end() && m.find(9) == m.end() && m.find(1) == m.end(), "erase right subtree and left chain");
        m[3] = ENGINE_2D_LAYER_13;
        Check(m.size() == 1 && m[3] == ENGINE_2D_LAYER_13, "reuse after erase");
    }
    printf(g_fails ? "FAIL_0042a264\n" : "PASS_0042a264\n");
    return g_fails ? 1 : 0;
}
