// Behaviour test for 0x00813580 _Rb_tree<u64, ...>::_M_insert(x, y, v, w) (model copy; rebalance stubbed).
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
static int g_rebalanced = 0;
static NodeBase* insert_model(Tree& t, NodeBase* x, NodeBase* y, const Node& v, NodeBase* w)
{
    Node* z;
    if (y == t.header || (w == 0 && (x != 0 || v.key < ((Node*)y)->key))) {
        z = (Node*)std::malloc(sizeof(Node)); z->key = v.key; z->value = v.value;
        y->left = z;
        if (y == t.header) { t.header->parent = z; t.header->right = z; }
        else if (y == t.header->left) t.header->left = z;
    }
    else {
        z = (Node*)std::malloc(sizeof(Node)); z->key = v.key; z->value = v.value;
        y->right = z;
        if (y == t.header->right) t.header->right = z;
    }
    z->parent = y; z->left = 0; z->right = 0;
    ++g_rebalanced;
    ++t.count;
    return z;
}
int main()
{
    Tree t; int ok = 1;
    build(t);
    Node v; v.key = 1; v.value.IsActive = true;
    NodeBase* a = insert_model(t, 0, &g_n3, v, 0);       // 1 < 3: left of 3, new leftmost
    if (g_n3.left != a || t.header->left != a || a->parent != &g_n3 || t.count != 4 || ((Node*)a)->key != 1) ok = 0;
    v.key = 9;
    NodeBase* b = insert_model(t, 0, &g_n8, v, 0);       // 9 > 8: right of 8, new rightmost
    if (g_n8.right != b || t.header->right != b || t.count != 5) ok = 0;
    v.key = 4;
    NodeBase* c = insert_model(t, 0, &g_n3, v, &g_n3);   // hint given: right insert
    if (g_n3.right != c || t.header->right != b) ok = 0;
    Tree e; NodeBase h; h.parent = 0; h.left = &h; h.right = &h; e.header = &h; e.count = 0;
    v.key = 7;
    NodeBase* d = insert_model(e, 0, &h, v, 0);          // empty tree: root, leftmost, rightmost
    if (h.parent != d || h.left != d || h.right != d || e.count != 1 || g_rebalanced != 4) ok = 0;
    std::free(a); std::free(b); std::free(c); std::free(d);
    if (!ok) { std::printf("00813580_TEST FAIL\n"); return 1; }
    std::printf("00813580_TEST PASS\n");
    return 0;
}