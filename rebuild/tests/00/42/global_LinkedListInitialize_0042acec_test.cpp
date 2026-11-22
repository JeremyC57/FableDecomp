// Behaviour test for 0x0042ACEC (NF229) on the shared headers engine/NUISystem_CManager.h / stlport/fable_map.h: NUISystem::CManager's
// map<long, EEngine2DLayer> (0x18-byte nodes). Under test: the _Rb_tree_base(const allocator&) constructor (reached through the map's default constructor): it allocates a
// header node of its own for each tree (two maps get distinct headers: begin == end differ), the new tree is empty (size 0,
// begin == end), and the header works as the tree's end sentinel (one insert, then find hit and miss).
// One case per condition; the out-of-line STLport rebalance is stubbed.
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_TREE_INSERT_UNIQUE_HINT_BODY
#define FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY
#include <stdio.h>
#include <string.h>
#include "engine/NUISystem_CManager.h"
// Stub of the out-of-line STLport rebalance (a landed row of its own): the tree stays a valid, unbalanced search tree.
namespace std { template<> void _Rb_global<bool>::_Rebalance(_Rb_tree_node_base*, _Rb_tree_node_base*&) { } }
static int g_fails = 0;
static void Check(bool ok, const char* what) { if (!ok) { printf("FAIL: %s\n", what); ++g_fails; } }
typedef std::map<long, EEngine2DLayer> Map;
int main()
{
    {
        Map a;
        Map b;
        Check(a.size() == 0 && a.begin() == a.end(), "new tree empty");
        Check(b.size() == 0 && b.begin() == b.end() && !(a.end() == b.end()), "each tree has its own header");
        a[13] = ENGINE_2D_LAYER_13;
        Check(a.size() == 1 && a.find(13) != a.end() && a.find(12) == a.end() && b.size() == 0, "the header is the end sentinel");
    }
    printf(g_fails ? "LINKED_LIST_INITIALIZE_PASS_FAIL\n" : "LINKED_LIST_INITIALIZE_PASS\n");
    return g_fails ? 1 : 0;
}
