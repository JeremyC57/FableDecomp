// Behaviour test for std::_Rb_tree<CCharString, ...anim components...>::clear (0x005E2460): erases from the root and
// resets the header (leftmost/rightmost = header, root = 0, count = 0); an empty tree is left untouched.
#include <stdio.h>
struct Node { bool color; Node* parent; Node* left; Node* right; };
static Node* g_erased = 0;
static int g_calls = 0;
struct Tree {
    Node* header; unsigned int count;
    Node*& root() const { return header->parent; }
    Node*& leftmost() const { return header->left; }
    Node*& rightmost() const { return header->right; }
    void erase_subtree(Node* x) { g_erased = x; ++g_calls; }
    void clear()
    {
        if (count != 0) {
            erase_subtree(root());
            leftmost() = header;
            root() = 0;
            rightmost() = header;
            count = 0;
        }
    }
};
int main()
{
    Node h, r, a, b;
    h.parent = &r; h.left = &a; h.right = &b;
    Tree t; t.header = &h; t.count = 3;
    t.clear();
    int ok = g_erased == &r && g_calls == 1 && h.parent == 0 && h.left == &h && h.right == &h && t.count == 0;
    Tree e; e.header = &h; e.count = 0; h.parent = &r;
    e.clear();
    ok = ok && g_calls == 1 && h.parent == &r;
    printf(ok ? "005e2460_TEST PASS\n" : "005e2460_TEST FAIL\n");
    return ok ? 0 : 1;
}
