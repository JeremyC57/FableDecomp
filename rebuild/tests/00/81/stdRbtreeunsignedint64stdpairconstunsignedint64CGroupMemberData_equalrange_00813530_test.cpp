// Behaviour test for 0x00813530 _Rb_tree<u64, ...>::equal_range (model copy: (lower_bound(k), upper_bound(k))).
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
static NodeBase* lower_bound(Tree& t, unsigned long long k)
{
    NodeBase* y = t.header; NodeBase* x = t.header->parent;
    while (x != 0) if (!(((Node*)x)->key < k)) y = x, x = x->left; else x = x->right;
    return y;
}
static NodeBase* upper_bound(Tree& t, unsigned long long k)
{
    NodeBase* y = t.header; NodeBase* x = t.header->parent;
    while (x != 0) if (k < ((Node*)x)->key) y = x, x = x->left; else x = x->right;
    return y;
}
struct Range { NodeBase* first; NodeBase* second; Range(NodeBase* a, NodeBase* b) : first(a), second(b) { } };
static Range equal_range(Tree& t, const unsigned long long& k) { return Range(lower_bound(t, k), upper_bound(t, k)); }
int main()
{
    Tree t; int ok = 1;
    build(t);
    g_n8.key = 0x100000000ULL + 8;                        // exercise the high dword
    Range r = equal_range(t, 5);
    if (r.first != &g_n5 || r.second != &g_n8) ok = 0;
    r = equal_range(t, 4);
    if (r.first != &g_n5 || r.second != &g_n5) ok = 0;
    r = equal_range(t, 0x100000000ULL + 8);
    if (r.first != &g_n8 || r.second != &g_header) ok = 0;
    r = equal_range(t, 1);
    if (r.first != &g_n3 || r.second != &g_n3) ok = 0;
    r = equal_range(t, 9);                                // 9 < 0x100000008
    if (r.first != &g_n8 || r.second != &g_n8) ok = 0;
    if (!ok) { std::printf("00813530_TEST FAIL\n"); return 1; }
    std::printf("00813530_TEST PASS\n");
    return 0;
}