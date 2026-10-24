#include <cstdio>
#include <cstdlib>
// Behaviour test for _Rb_tree<unsigned long, pair<const unsigned long, CUIState>>::operator= (retail 0x0042CE7A):
// self-assignment is a no-op; assigning an empty tree clears and resets the header (root 0, leftmost = rightmost =
// header, count 0); assigning a non-empty tree clones it and sets leftmost/rightmost to its minimum/maximum and the count.
struct Node { bool color; Node* parent; Node* left; Node* right; unsigned long key; };
static Node* MinNode(Node* x) { while (x->left) x = x->left; return x; }
static Node* MaxNode(Node* x) { while (x->right) x = x->right; return x; }
static int g_erased = 0;
static void Erase(Node* x) { while (x) { Erase(x->right); Node* y = x->left; std::free(x); ++g_erased; x = y; } }
static Node* Copy(Node* x, Node* p)
{
    Node* top = (Node*)std::malloc(sizeof(Node)); *top = *x; top->left = top->right = 0; top->parent = p;
    if (x->right) top->right = Copy(x->right, top);
    p = top; x = x->left;
    while (x) {
        Node* y = (Node*)std::malloc(sizeof(Node)); *y = *x; y->left = y->right = 0;
        p->left = y; y->parent = p;
        if (x->right) y->right = Copy(x->right, y);
        p = y; x = x->left;
    }
    return top;
}
struct Tree {
    Node* header; unsigned int count;
    void clear() { if (count) { Erase(header->parent); header->left = header; header->parent = 0; header->right = header; count = 0; } }
    Tree& operator=(const Tree& x)
    {
        if (this != &x) {
            clear();
            count = 0;
            if (x.header->parent == 0) { header->parent = 0; header->left = header; header->right = header; }
            else {
                header->parent = Copy(x.header->parent, header);
                header->left = MinNode(header->parent);
                header->right = MaxNode(header->parent);
                count = x.count;
            }
        }
        return *this;
    }
};
int main()
{
    Node h1, h2, a, b, c;
    a.key = 1; b.key = 2; c.key = 3; a.color = b.color = c.color = false;
    b.left = &a; b.right = &c; a.left = a.right = c.left = c.right = 0; a.parent = c.parent = &b; b.parent = &h1;
    h1.parent = &b; h1.left = &a; h1.right = &c;
    Tree src; src.header = &h1; src.count = 3;
    h2.parent = 0; h2.left = h2.right = &h2;
    Tree dst; dst.header = &h2; dst.count = 0;
    dst = src;
    int ok = dst.count == 3 && h2.parent && h2.parent->key == 2 && h2.parent->parent == &h2 && h2.left->key == 1 && h2.right->key == 3
          && h2.parent != &b;
    Tree& self = (dst = dst);
    ok = ok && &self == &dst && dst.count == 3 && g_erased == 0;
    Node h3; h3.parent = 0; h3.left = h3.right = &h3;
    Tree empty; empty.header = &h3; empty.count = 0;
    dst = empty;
    ok = ok && g_erased == 3 && dst.count == 0 && h2.parent == 0 && h2.left == &h2 && h2.right == &h2;
    if (!ok) { std::printf("0042ce7a_TEST FAIL\n"); return 1; }
    std::printf("0042ce7a_TEST PASS\n");
    return 0;
}