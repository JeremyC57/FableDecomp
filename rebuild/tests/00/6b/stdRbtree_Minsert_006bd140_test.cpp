// Behaviour test for 0x006BD140 _Rb_tree<CCharString, pair<const CCharString, C2DVector> >::_M_insert (model copy).
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>

struct Node { bool color; Node* parent; Node* left; Node* right; const char* key; float x, y; };
struct Tree {
    Node* header; unsigned int count; int rebalances;
    static bool less(const char* a, const char* b)                 // CCharString::operator< on the shared blocks
    {
        if (a == b) return false;
        if (a == 0) return true;
        if (b == 0) return false;
        return std::strcmp(a, b) < 0;
    }
    Node* create(const char* k, float x, float y)
    {
        Node* n = (Node*)std::malloc(sizeof(Node)); n->key = k; n->x = x; n->y = y; return n;
    }
    Node* insert(Node* x, Node* y, const char* k, float vx, float vy, Node* w)
    {
        Node* z;
        if (y == header || (w == 0 && (x != 0 || less(k, y->key)))) {
            z = create(k, vx, vy);
            y->left = z;
            if (y == header) { header->parent = z; header->right = z; }
            else if (y == header->left) header->left = z;
        } else {
            z = create(k, vx, vy);
            y->right = z;
            if (y == header->right) header->right = z;
        }
        z->parent = y; z->left = 0; z->right = 0;
        ++rebalances;                                              // _Rb_global<bool>::_Rebalance(z, root)
        ++count;
        return z;
    }
};

int main()
{
    Node h; h.color = false; h.parent = 0; h.left = &h; h.right = &h;
    Tree t; t.header = &h; t.count = 0; t.rebalances = 0;
    Node* r = t.insert(0, &h, "m", 1.0f, 2.0f, 0);                 // empty tree: becomes root, leftmost, rightmost
    int ok = h.parent == r && h.left == r && h.right == r && r->parent == &h && t.count == 1;
    Node* a = t.insert(0, r, "c", 3.0f, 4.0f, 0);                  // "c" < "m": left child, new leftmost
    ok = ok && r->left == a && h.left == a && h.right == r && a->x == 3.0f;
    Node* z = t.insert(0, r, "z", 5.0f, 6.0f, 0);                  // "z" > "m": right child, new rightmost
    ok = ok && r->right == z && h.right == z && h.left == a;
    Node* d = t.insert(0, a, "d", 7.0f, 8.0f, &h);                 // w != 0 forces the right link
    ok = ok && a->right == d && h.right == z && d->left == 0 && d->right == 0 && d->parent == a;
    ok = ok && t.count == 4 && t.rebalances == 4 && d->y == 8.0f;
    std::free(r); std::free(a); std::free(z); std::free(d);
    if (!ok) { std::printf("006bd140_TEST FAIL\n"); return 1; }
    std::printf("006bd140_TEST PASS\n");
    return 0;
}