// behaviour test for 0x00A80C20 _Rb_tree::_M_insert (model copy with unsigned keys; _Rebalance is a counting stub)
#include <cstdio>
#include <cstdlib>
#include <new>
struct NodeBase { bool color; NodeBase* parent; NodeBase* left; NodeBase* right; };
struct Node : NodeBase { unsigned long key; unsigned long val; };
static int g_rebal = 0;
static void Rebalance(NodeBase*, NodeBase*&) { ++g_rebal; }
struct Tree {
    Node* header; unsigned count;
    Node* insert(NodeBase* x_, NodeBase* y_, const unsigned long* v, NodeBase* w_)
    {
        Node* w = (Node*)w_; Node* x = (Node*)x_; Node* y = (Node*)y_; Node* z;
        if (y == header || (w == 0 && (x != 0 || v[0] < y->key))) {
            z = (Node*)std::malloc(sizeof(Node)); z->key = v[0]; z->val = v[1];
            y->left = z;
            if (y == header) { header->parent = z; header->right = z; }
            else if (y == header->left) header->left = z;
        } else {
            z = (Node*)std::malloc(sizeof(Node)); z->key = v[0]; z->val = v[1];
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
    Node hdr; hdr.color = false; hdr.parent = 0; hdr.left = &hdr; hdr.right = &hdr;
    Tree t; t.header = &hdr; t.count = 0;
    unsigned long v1[2] = { 50, 1 }, v2[2] = { 70, 2 }, v3[2] = { 20, 3 };
    Node* a = t.insert(0, &hdr, v1, 0);                  // empty tree: root, leftmost, rightmost
    int ok = hdr.parent == a && hdr.left == a && hdr.right == a && a->parent == &hdr;
    Node* b = t.insert(0, a, v2, 0);                     // larger key: right of a, new rightmost
    ok = ok && a->right == b && hdr.right == b && hdr.left == a;
    Node* c = t.insert(0, a, v3, 0);                     // smaller key: left of a, new leftmost
    ok = ok && a->left == c && hdr.left == c && t.count == 3 && g_rebal == 3 && c->val == 3;
    if (!ok) { std::printf("00a80c20_TEST FAIL\n"); return 1; }
    std::printf("00a80c20_TEST PASS\n");
    return 0;
}