// Behaviour test for 0x0042A237 (NF229) on the shared headers engine/NUISystem_CManager.h / stlport/fable_map.h: NUISystem::CManager's
// map<EUsedKeys, long> (Keys). Under test: the tree _M_erase (reached through clear()): a one-node tree, and a tree whose root has a right
// subtree and a left chain (insert order A, ZOOM_OUT, DOWN, LEFT, B on the unbalanced tree: the right recursion and the left continuation
// both run), end empty and reusable. One case per condition; the out-of-line STLport rebalance is stubbed (no other model).
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_TREE_INSERT_UNIQUE_HINT_BODY
#define FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY
#include <stdio.h>
#include "engine/NUISystem_CManager.h"
// Stub of the out-of-line STLport rebalance (a landed row of its own): the tree stays a valid, unbalanced search tree.
namespace std { template<> void _Rb_global<bool>::_Rebalance(_Rb_tree_node_base*, _Rb_tree_node_base*&) { } }
static int g_fails = 0;
static void Check(bool ok, const char* what) { if (!ok) { printf("FAIL: %s\n", what); ++g_fails; } }
typedef NUISystem::CManager::EUsedKeys Key;
typedef std::map<Key, long> KeyMap;
int main()
{
    {
        KeyMap m;
        m[NUISystem::CManager::UI_KEY_UP] = 1;
        Check(m.size() == 1, "one node");
        m.clear();
        Check(m.size() == 0 && m.begin() == m.end(), "erase one node");
        m[NUISystem::CManager::UI_KEY_A] = 6; m[NUISystem::CManager::UI_KEY_ZOOM_OUT] = 9; m[NUISystem::CManager::UI_KEY_DOWN] = 2;
        m[NUISystem::CManager::UI_KEY_LEFT] = 0; m[NUISystem::CManager::UI_KEY_B] = 7;
        Check(m.size() == 5, "five nodes");
        m.clear();
        Check(m.size() == 0 && m.begin() == m.end() && m.find(NUISystem::CManager::UI_KEY_ZOOM_OUT) == m.end(), "erase right subtree and left chain");
        m[NUISystem::CManager::UI_KEY_RIGHT] = 3;
        Check(m.size() == 1 && m[NUISystem::CManager::UI_KEY_RIGHT] == 3, "reuse after erase");
    }
    printf(g_fails ? "FAIL_0042a237\n" : "PASS_0042a237\n");
    return g_fails ? 1 : 0;
}
