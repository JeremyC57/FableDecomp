// Behaviour test for 0x0042AE33 (NF229) on the shared headers engine/NUISystem_CManager.h / stlport/fable_map.h: NUISystem::CManager's
// map<long, EEngine2DLayer>. Under test: the tree _M_lower_bound(const long&) const (the row's FABLE_RETAIL_STLP_TREE_BOUNDS_OUTOFLINE +
// _BOUNDS_BODY_PASS22 body, reached through lower_bound): an empty tree gives the header (end), an exact key gives its node, a key between
// two keys gives the next greater one, a key below all gives the first node, a key above all gives end. One case per condition; the
// out-of-line STLport rebalance is stubbed (no other model).
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_TREE_BOUNDS_OUTOFLINE
#define FABLE_RETAIL_STLP_TREE_BOUNDS_BODY_PASS22
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
        Check(m.lower_bound(5) == m.end(), "empty tree: end");
        m[10] = ENGINE_2D_LAYER_0; m[20] = ENGINE_2D_LAYER_1; m[-5] = ENGINE_2D_LAYER_M3; m[15] = ENGINE_2D_LAYER_13;
        LayerMap::iterator it = m.lower_bound(15);
        Check(it != m.end() && (*it).first == 15 && (*it).second == ENGINE_2D_LAYER_13, "exact key");
        it = m.lower_bound(11);
        Check(it != m.end() && (*it).first == 15, "between keys: the next greater");
        it = m.lower_bound(-100);
        Check(it == m.begin() && (*it).first == -5, "below all: the first node");
        Check(m.lower_bound(21) == m.end(), "above all: end");
    }
    printf(g_fails ? "FAIL_0042ae33\n" : "PASS_0042ae33\n");
    return g_fails ? 1 : 0;
}
