// Behaviour test for 0x00990830 _Rb_tree<CPendingObjectSortKey,...>::_M_insert: into an empty tree the node becomes
// root/leftmost/rightmost; a smaller key goes left of y and becomes leftmost; a larger one (or w != 0) goes right and
// becomes rightmost; x != 0 forces the left side; links, value copy, count and the rebalance call are checked.
#include <cstdio>
#include <cstdlib>
#include <new>
struct NodeBase { bool color; NodeBase* parent; NodeBase* left; NodeBase* right; };
struct Key { unsigned long k; };
struct Value { Key first; void* second; };
struct Node : NodeBase { Value v; };
static int g_rebalances = 0; static NodeBase* g_rebalanceNode = 0; static NodeBase** g_rootRef = 0;
static void Rebalance(NodeBase* x, NodeBase*& root) { ++g_rebalances; g_rebalanceNode = x; g_rootRef = &root; }
struct Less { bool operator()(const Key& a, const Key& b) const { return a.k < b.k; } };
struct Tree {
    NodeBase* header; unsigned int count; Less comp;
    Node* create(const Value& v) { Node* t = (Node*)malloc(sizeof(Node)); new (&t->v) Value(v); return t; }
    NodeBase* insert(NodeBase* x, NodeBase* y_, const Value& v, NodeBase* w)
    {
        Node* y = (Node*)y_; Node* z;
        if (y == header || (w == 0 && (x != 0 || comp(v.first, y->v.first)))) {
            z = create(v); y->left = z;
            if (y == header) { header->parent = z; header->right = z; }
            else if (y == header->left) header->left = z;
        } else {
            z = create(v); y->right = z;
            if (y == header->right) header->right = z;
        }
        z->parent = y; z->left = 0; z->right = 0;
        Rebalance(z, header->parent);
        ++count;
        return z;
    }
};
int main()
{
    int fails = 0, o = 0;
    NodeBase h; h.color = false; h.parent = 0; h.left = &h; h.right = &h;
    Tree t; t.header = &h; t.count = 0;
    Value v5 = { { 5 }, &o }, v2 = { { 2 }, 0 }, v9 = { { 9 }, 0 }, v7 = { { 7 }, 0 };
    Node* r = (Node*)t.insert(0, &h, v5, 0);
    if (h.parent != r || h.left != r || h.right != r || r->parent != &h || r->v.first.k != 5 || r->v.second != &o) ++fails;
    if (t.count != 1 || g_rebalances != 1 || g_rebalanceNode != r || g_rootRef != &h.parent) ++fails;
    Node* a = (Node*)t.insert(0, r, v2, 0);
    if (r->left != a || h.left != a || a->parent != r || a->left || a->right || t.count != 2) ++fails;
    Node* b = (Node*)t.insert(0, r, v9, 0);
    if (r->right != b || h.right != b || t.count != 3) ++fails;
    Node* c = (Node*)t.insert(0, b, v7, (NodeBase*)1);
    if (b->right != c || h.right != c || b->left) ++fails;
    Node* d = (Node*)t.insert((NodeBase*)1, c, v9, 0);
    if (c->left != d || h.left != a || t.count != 5 || g_rebalances != 5) ++fails;
    free(r); free(a); free(b); free(c); free(d);
    std::printf(fails ? "00990830_TEST FAIL\n" : "00990830_TEST PASS\n");
    return fails ? 1 : 0;
}