// behaviour test for 0x00835270 std::_Rb_tree::_M_insert [set<void*>](x, y, v, w): the first node becomes root, leftmost and rightmost; a key below y (unsigned) or a non-null x links left of y and updates leftmost; w != 0 or a key not below y links right and updates rightmost; the count grows
#include <cstdio>
#include <cstdlib>
typedef unsigned int Key;
struct NodeBase { bool color; NodeBase* parent; NodeBase* left; NodeBase* right; };
struct Node : NodeBase { Key value; };
static int gRebalanced = 0;
static void Rebalance(NodeBase* x, NodeBase*& root) { if (x != 0 && root != 0) ++gRebalanced; }
struct Tree {
    NodeBase* header; unsigned int count;
    NodeBase* Insert(NodeBase* x, NodeBase* y, const Key& v, NodeBase* w)   // the function under test (STLport 4.6)
    {
        Node* z;
        if (y == header || (w == 0 && (x != 0 || v < ((Node*)y)->value))) {
            z = (Node*)std::malloc(sizeof(Node)); z->value = v;
            y->left = z;
            if (y == header) { header->parent = z; header->right = z; }
            else if (y == header->left) header->left = z;
        } else {
            z = (Node*)std::malloc(sizeof(Node)); z->value = v;
            y->right = z;
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
    int fail = 0;
    Tree t;
    NodeBase* h = (NodeBase*)std::malloc(sizeof(Node));
    h->color = false; h->parent = 0; h->left = h; h->right = h;
    t.header = h; t.count = 0;
    NodeBase* a = t.Insert(0, h, 20u, 0);                         // empty tree: y == header
    if (h->parent != a || h->left != a || h->right != a || a->parent != h || t.count != 1) fail |= 1;
    NodeBase* b = t.Insert(0, a, 10u, 0);                         // below y: left child, new leftmost
    if (a->left != b || h->left != b || h->right != a || t.count != 2) fail |= 2;
    NodeBase* c = t.Insert(0, a, 0x90000000u, 0);                 // above (unsigned): right child, new rightmost
    if (a->right != c || h->right != c || h->left != b || t.count != 3) fail |= 4;
    NodeBase* d = t.Insert(c, c, 25u, 0);                         // x hint: left of y regardless of the compare
    if (c->left != d || h->left != b || t.count != 4) fail |= 8;
    NodeBase* e = t.Insert(0, b, 5u, b);                          // w != 0: right of y even though 5 < 10
    if (b->right != e || h->right != c || t.count != 5 || gRebalanced != 5) fail |= 16;
    if (fail) { std::printf("00835270_TEST FAIL %d\n", fail); return 1; }
    std::printf("00835270_TEST PASS\n");
    return 0;
}