// Behaviour test for 0x0058148D CTCHaste::UnsetActionSpeedMultiplier (NF64 / NF70 round 7), on the row's own shared headers
// (engine/CTCHaste.h: Multipliers is the STLport map<void*,float> at +0x18). The row object carries the visible STLport erase(const key&) /
// erase(first, last) / clear COMDATs, so the test links them as they are and only supplies what that object imports: the private
// UpdateActionSpeedMultiplier (0x00800020; counted here) and the single-position erase(iterator) (never reached by these cases: each
// erase either clears the whole tree or erases an empty range). The tree is laid out by hand in STLport 4.6 node form (color, parent,
// left, right, then the value) with malloc'd nodes, as the row's clear frees them through deallocate.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCHASTE
#define FABLE_RETAIL_CTCHASTE_UNSET
#define FABLE_RETAIL_STLP_TREE_ERASE
#define FABLE_RETAIL_STLP_TREE_ERASE_KEY
#include "fable_compiler.h"
#include "fable_string.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CTCHaste.h"

static int g_updates = 0;
static int g_positionErases = 0;
void CTCHaste::UpdateActionSpeedMultiplier() { ++g_updates; }
typedef std::_Rb_tree<void*, std::pair<void* const, float>, std::_Select1st<std::pair<void* const, float> >, std::less<void*>,
                      std::allocator<std::pair<void* const, float> > > TMultiplierTree;
template <> void TMultiplierTree::erase(TMultiplierTree::iterator) { ++g_positionErases; }

struct SNode { long color; SNode* parent; SNode* left; SNode* right; void* key; float value; };   // STLport _Rb_tree_node<pair<void* const,float> >

int main()
{
    static unsigned long storage[0x40];
    CTCHaste* haste = reinterpret_cast<CTCHaste*>(storage);
    unsigned long* tree = storage + 0x18 / 4;   // the private Multipliers (+0x18): header node pointer, node count, comparator
    static SNode header;
    int a, b;
    bool ok = true;
    // one node {&a, 2.0f}
    SNode* node = static_cast<SNode*>(std::malloc(sizeof(SNode)));
    node->color = 1; node->parent = &header; node->left = 0; node->right = 0; node->key = &a; node->value = 2.0f;
    header.color = 0; header.parent = node; header.left = node; header.right = node;
    tree[0] = reinterpret_cast<unsigned long>(&header); tree[1] = 1;
    // key absent (an empty equal_range): the tree is unchanged, the speed recomputed anyway
    haste->UnsetActionSpeedMultiplier(&b);
    if (tree[1] != 1 || header.parent != node || g_updates != 1 || g_positionErases != 0) ok = false;
    // key present and the only one: erase(first, last) over the whole tree clears it (frees the node), then recomputes
    haste->UnsetActionSpeedMultiplier(&a);
    if (tree[1] != 0 || header.parent != 0 || header.left != &header || header.right != &header || g_updates != 2 || g_positionErases != 0) ok = false;
    // empty tree: still recomputes
    haste->UnsetActionSpeedMultiplier(&a);
    if (tree[1] != 0 || g_updates != 3) ok = false;
    if (!ok) { std::printf("0058148d_TEST FAIL\n"); return 1; }
    std::printf("0058148d_TEST PASS\n");
    return 0;
}
