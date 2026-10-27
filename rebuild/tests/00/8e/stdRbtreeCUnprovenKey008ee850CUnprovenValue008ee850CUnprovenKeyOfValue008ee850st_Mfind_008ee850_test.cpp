// Behaviour test for 0x008EE850 std::_Rb_tree<..long key..>::_M_find(const key&) const: lower-bound walk from the root,
// returning the header unless the found node's key equals the searched key.
#include <cstdio>

struct Node { int color; Node* parent; Node* left; Node* right; long key; };

static Node* Find(Node* header, long k)
{
    Node* y = header;
    Node* x = header->parent;
    while (x != 0)
        if (!(x->key < k)) { y = x; x = x->left; }
        else x = x->right;
    if (y == header || k < y->key)
        y = header;
    return y;
}

int main()
{
    Node header, n10, n20, n30, n5;
    header.parent = &n20;
    n20.key = 20; n20.left = &n10; n20.right = &n30;
    n10.key = 10; n10.left = &n5; n10.right = 0;
    n30.key = 30; n30.left = 0; n30.right = 0;
    n5.key = -5; n5.left = 0; n5.right = 0;
    if (Find(&header, 20) != &n20 || Find(&header, 10) != &n10 || Find(&header, 30) != &n30 || Find(&header, -5) != &n5) { std::printf("008ee850_TEST FAIL\n"); return 1; }
    if (Find(&header, 15) != &header || Find(&header, 31) != &header || Find(&header, -6) != &header) { std::printf("008ee850_TEST FAIL\n"); return 1; }
    Node empty; empty.parent = 0;
    if (Find(&empty, 1) != &empty) { std::printf("008ee850_TEST FAIL\n"); return 1; }
    std::printf("008ee850_TEST PASS\n");
    return 0;
}