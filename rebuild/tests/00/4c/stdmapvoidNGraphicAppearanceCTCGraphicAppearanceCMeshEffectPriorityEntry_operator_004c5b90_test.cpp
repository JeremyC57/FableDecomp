// Behaviour test for 0x004C5B90 std::map<void*, CTCGraphicAppearance::CMeshEffectPriorityEntry>::operator[]: the row verbatim, driven
// through a real STLport map. Missing owners are inserted with an entry whose effect pointer is null; existing owners return the same
// entry (unsigned pointer order).
#include <stdio.h>
#include "fable_compiler.h"
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_MOVEMENT
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_EFFECT_ENTRY_PASS23
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_TREE_INSERT_UNIQUE_HINT_BODY
#define FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"

NGraphicAppearance::CTCGraphicAppearance::CMeshEffectPriorityEntry::CMeshEffectPriorityEntry()
{
}
struct CTCGraphicAppearanceMeshEffects : NGraphicAppearance::CTCGraphicAppearance {
    typedef CMeshEffectPriorityEntry TEntry;
};
namespace std {
template CTCGraphicAppearanceMeshEffects::TEntry& map<void*, CTCGraphicAppearanceMeshEffects::TEntry>::operator[](void* const&);
}

// Test stand-in for STLport's out-of-line _Rb_global<bool>::_Rebalance (retail library code, 0x0042971D): no rotations, the tree stays a
// valid (unbalanced) binary search tree, which is all operator[]'s lower_bound walk needs.
namespace std {
template<> void __cdecl _Rb_global<bool>::_Rebalance(_Rb_tree_node_base*, _Rb_tree_node_base*& root) { root->_M_color = true; }
}
static int g_fail = 0;
static void check(int ok, const char* what) { if (!ok) { printf("FAIL %s\n", what); g_fail = 1; } }

int main()
{
    typedef CTCGraphicAppearanceMeshEffects::TEntry TEntry;
    std::map<void*, TEntry> effects;
    static int owners[3];
    TEntry& a = effects[&owners[1]];
    check(effects.size() == 1 && *(void**)&a == 0, "inserted with a null effect");
    *(long*)&a.Priority = 7;
    TEntry& b = effects[&owners[0]];
    TEntry& c = effects[(void*)0xF0000000];
    check(effects.size() == 3 && &a != &b && &b != &c, "three owners");
    check(&effects[&owners[1]] == &a && *(long*)&a.Priority == 7 && effects.size() == 3, "existing owner found");
    check(effects.begin()->first == &owners[0] && (--effects.end())->first == (void*)0xF0000000, "unsigned order");
    if (!g_fail) printf("004c5b90_TEST PASS\n");
    return g_fail;
}