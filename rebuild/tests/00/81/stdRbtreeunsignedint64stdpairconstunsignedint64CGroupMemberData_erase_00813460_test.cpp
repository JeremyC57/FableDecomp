// Behaviour test for 0x00813460 _Rb_tree<u64, pair<const u64, CGroupMemberData>>::erase(first, last) (model copy,
// stubbed rebalance/deallocate).
#include <cstdio>
#include <cstdlib>
struct NodeBase { bool color; NodeBase* parent; NodeBase* left; NodeBase* right; };
struct Data { int vptr_slot; void* PData; bool IsActive; unsigned int JoinedFrame; unsigned int DeactivatedFrame; };
struct Node : NodeBase { unsigned long long key; Data value; };
static void increment(NodeBase*& n)
{
    if (n->right != 0) { n = n->right; while (n->left != 0) n = n->left; }
    else { NodeBase* y = n->parent; while (n == y->right) { n = y; y = y->parent; } if (n->right != y) n = y; }
}
struct Tree { NodeBase* header; unsigned int count; };
// header + 3-node tree: 5 (root) with children 3 and 8
static NodeBase g_header; static Node g_n3, g_n5, g_n8;
static void build(Tree& t)
{
    g_header.parent = &g_n5; g_header.left = &g_n3; g_header.right = &g_n8; g_header.color = false;
    g_n5.parent = &g_header; g_n5.left = &g_n3; g_n5.right = &g_n8; g_n5.key = 5;
    g_n3.parent = &g_n5; g_n3.left = 0; g_n3.right = 0; g_n3.key = 3;
    g_n8.parent = &g_n5; g_n8.left = 0; g_n8.right = 0; g_n8.key = 8;
    g_n3.value.IsActive = false; g_n5.value.IsActive = false; g_n8.value.IsActive = false;
    t.header = &g_header; t.count = 3;
}
static int g_eraseRoot = 0; static NodeBase* g_erased[8]; static int g_nErased = 0;
static void M_erase(NodeBase* root) { if (root == &g_n5) ++g_eraseRoot; }
static NodeBase* rebalance_for_erase(NodeBase* z) { g_erased[g_nErased++] = z; return z; }
static void erase_one(Tree& t, NodeBase* pos) { NodeBase* y = rebalance_for_erase(pos); (void)y; --t.count; }
static void erase_range(Tree& t, NodeBase* first, NodeBase* last)
{
    if (first == t.header->left && last == t.header) {
        if (t.count != 0) { M_erase(t.header->parent); t.header->left = t.header; t.header->parent = 0; t.header->right = t.header; t.count = 0; }
    }
    else
        while (first != last) { NodeBase* old = first; increment(first); erase_one(t, old); }
}
int main()
{
    Tree t; int ok = 1;
    build(t);
    erase_range(t, &g_n3, &g_n8);                         // partial: erases 3 then 5
    if (g_nErased != 2 || g_erased[0] != &g_n3 || g_erased[1] != &g_n5 || t.count != 1 || g_eraseRoot != 0) ok = 0;
    build(t); g_nErased = 0;
    erase_range(t, &g_n5, &g_header);                     // partial to end: erases 5 then 8
    if (g_nErased != 2 || g_erased[0] != &g_n5 || g_erased[1] != &g_n8 || t.count != 1) ok = 0;
    build(t); g_nErased = 0;
    erase_range(t, &g_n3, &g_header);                     // whole range: clear()
    if (g_eraseRoot != 1 || g_nErased != 0 || t.count != 0 || g_header.left != &g_header || g_header.right != &g_header || g_header.parent != 0) ok = 0;
    erase_range(t, g_header.left, &g_header);             // clear of an empty tree does nothing
    if (g_eraseRoot != 1) ok = 0;
    if (!ok) { std::printf("00813460_TEST FAIL\n"); return 1; }
    std::printf("00813460_TEST PASS\n");
    return 0;
}