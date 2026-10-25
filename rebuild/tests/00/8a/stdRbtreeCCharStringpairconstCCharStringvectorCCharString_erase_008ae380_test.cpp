// Behaviour test for 0x008AE380 _Rb_tree::erase(first, last): whole range -> clear(), else node-by-node in order.
#include <cstdio>
struct N { bool color; N* parent; N* left; N* right; int key; };
static N* Increment(N* x)
{
    if (x->right != 0) { x = x->right; while (x->left != 0) x = x->left; }
    else { N* y = x->parent; while (x == y->right) { x = y; y = y->parent; } if (x->right != y) x = y; }
    return x;
}
struct Tree {
    N* header; unsigned int count; int erased[8]; int nErased; int clears;
    void erase_one(N* p) { erased[nErased++] = p->key; --count; }
    void clear() { if (count != 0) { ++clears; header->left = header; header->parent = 0; header->right = header; count = 0; } }
    void erase(N* first, N* last)
    {
        if (first == header->left && last == header) clear();
        else while (first != last) { N* old = first; first = Increment(first); erase_one(old); }
    }
};
int main()
{
    // header + three nodes: 2 (root), 1 (left), 3 (right)
    N h, a, b, c;
    h.color = false; a.key = 2; b.key = 1; c.key = 3;
    h.parent = &a; h.left = &b; h.right = &c;
    a.parent = &h; a.left = &b; a.right = &c;
    b.parent = &a; b.left = 0; b.right = 0;
    c.parent = &a; c.left = 0; c.right = 0;
    Tree t; t.header = &h; t.count = 3; t.nErased = 0; t.clears = 0;
    t.erase(&b, &c);   // erases 1 and 2
    if (t.nErased != 2 || t.erased[0] != 1 || t.erased[1] != 2 || t.count != 1 || t.clears != 0) { std::printf("008ae380_TEST FAIL\n"); return 1; }
    t.count = 3; t.nErased = 0;
    t.erase(&b, &h);   // whole tree
    if (t.clears != 1 || t.count != 0 || h.left != &h || h.parent != 0 || t.nErased != 0) { std::printf("008ae380_TEST FAIL\n"); return 1; }
    std::printf("008ae380_TEST PASS\n");
    return 0;
}