// Behaviour test for 0x00593CE4 _Rb_tree<CDateAndTime, ...>::_M_insert (header link maintenance, model copy)
#include <cstdio>
struct Node { bool color; Node* parent; Node* left; Node* right; int key; };
static int g_rebalanced = 0;
static void rebalance(Node*, Node*&) { ++g_rebalanced; }
static Node g_pool[8];
static int g_used = 0;
struct Tree {
    Node* header;
    unsigned int count;
    Node* create(int v) { Node* n = &g_pool[g_used++]; n->key = v; n->left = n->right = n->parent = (Node*)1; return n; }
    Node* insert(Node* x, Node* y, const int& v, Node* w)
    {
        Node* z;
        if (y == header || (w == 0 && (x != 0 || v < y->key))) {
            z = create(v);
            y->left = z;
            if (y == header) { header->parent = z; header->right = z; }
            else if (y == header->left) header->left = z;
        } else {
            z = create(v);
            y->right = z;
            if (y == header->right) header->right = z;
        }
        z->parent = y; z->left = 0; z->right = 0;
        rebalance(z, header->parent);
        ++count;
        return z;
    }
};
int main()
{
    Node h; h.parent = 0; h.left = &h; h.right = &h;
    Tree t; t.header = &h; t.count = 0;
    Node* a = t.insert(0, &h, 20, 0);
    if (h.parent != a || h.left != a || h.right != a || a->parent != &h || a->left != 0 || t.count != 1) { std::printf("00593ce4_TEST FAIL\n"); return 1; }
    Node* b = t.insert(0, a, 10, 0);          // less: goes left, new leftmost
    if (a->left != b || h.left != b || h.right != a || b->parent != a) { std::printf("00593ce4_TEST FAIL\n"); return 1; }
    Node* c = t.insert(0, a, 30, 0);          // not less: goes right, new rightmost
    if (a->right != c || h.right != c || h.left != b || t.count != 3 || g_rebalanced != 3) { std::printf("00593ce4_TEST FAIL\n"); return 1; }
    Node* d = t.insert(0, c, 5, c);           // w != 0 forces the right side
    if (c->right != d || h.right != d) { std::printf("00593ce4_TEST FAIL\n"); return 1; }
    std::printf("00593ce4_TEST PASS\n");
    return 0;
}