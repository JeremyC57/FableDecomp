// behaviour test for 0x0079F330 _Rb_tree::insert_unique (set<CThing*>): new keys are inserted (true), duplicates are rejected (false) and return the existing node; the predecessor step and both _M_insert paths are exercised
#include <cstdio>
#include <cstdlib>
#include <stddef.h>
class CThing;
typedef CThing* K;
static K mk(unsigned int v) { return (K)(size_t)(v * 16 + 16); }
struct Node { bool color; Node* parent; Node* left; Node* right; K key; };
struct Tree {
    Node* header; unsigned int count;
    Node* root() const { return header->parent; }
    Node* Insert(Node* x, Node* y, const K& v)        // STLport _M_insert without the rebalance (shape only)
    {
        Node* z = (Node*)std::malloc(sizeof(Node));
        z->key = v; z->left = z->right = 0; z->parent = y; z->color = true;
        if (y == header || x != 0 || v < y->key) {
            y->left = z;
            if (y == header) { header->parent = z; header->right = z; }
            else if (y == header->left) header->left = z;
        } else {
            y->right = z;
            if (y == header->right) header->right = z;
        }
        ++count;
        return z;
    }
    static Node* Dec(Node* n)
    {
        if (n->color == false && n->parent->parent == n) return n->right;
        if (n->left != 0) { Node* y = n->left; while (y->right != 0) y = y->right; return y; }
        Node* y = n->parent;
        while (n == y->left) { n = y; y = y->parent; }
        return y;
    }
    bool InsertUnique(const K& v, Node** out)
    {
        Node* y = header; Node* x = root(); bool comp = true;
        while (x != 0) { y = x; comp = v < x->key; x = comp ? x->left : x->right; }
        Node* j = y;
        if (comp) {
            if (j == header->left) { *out = Insert(y, y, v); return true; }
            j = Dec(j);
        }
        if (j->key < v) { *out = Insert(x, y, v); return true; }
        *out = j; return false;
    }
};
int main()
{
    Node h; h.color = false; h.parent = 0; h.left = &h; h.right = &h;
    Tree t; t.header = &h; t.count = 0;
    unsigned int keys[7] = { 5, 2, 8, 2, 7, 5, 9 };
    bool want[7] = { true, true, true, false, true, false, true };
    Node* got[7];
    int ok = 1;
    for (int i = 0; i < 7; ++i) {
        bool r = t.InsertUnique(mk(keys[i]), &got[i]);
        if (r != want[i] || got[i]->key != mk(keys[i])) ok = 0;
    }
    ok = ok && t.count == 5 && got[3] == got[1] && got[5] == got[0] && h.left->key == mk(2) && h.right->key == mk(9);
    if (!ok) { std::printf("0079f330_TEST FAIL\n"); return 1; }
    std::printf("0079f330_TEST PASS\n");
    return 0;
}