// Behaviour test for 00bd7e30: std::_Rb_tree_iterator<map<unsigned long, CArray<C3DVector> >,..>::operator-- -- a reverse in-order walk from the header visits every node once, ends at the leftmost; the header steps to the rightmost
#include <cstdio>
struct Node { bool color; Node* parent; Node* left; Node* right; long key; };
struct Iter {
    Node* node;
    Iter& operator--()
    {
        Node* n = node;
        if (n->color == false && n->parent->parent == n) n = n->right;
        else if (n->left) { Node* y = n->left; while (y->right) y = y->right; n = y; }
        else { Node* y = n->parent; while (n == y->left) { n = y; y = y->parent; } n = y; }
        node = n;
        return *this;
    }
};
int main()
{
    //        4
    //      2   6
    //     1 3 5 7
    Node h, n[8];
    for (int i = 1; i <= 7; ++i) { n[i].key = i; n[i].left = n[i].right = 0; n[i].color = true; }
    n[4].left = &n[2]; n[4].right = &n[6]; n[2].parent = &n[4]; n[6].parent = &n[4];
    n[2].left = &n[1]; n[2].right = &n[3]; n[1].parent = n[3].parent = &n[2];
    n[6].left = &n[5]; n[6].right = &n[7]; n[5].parent = n[7].parent = &n[6];
    // every node is black (true); only the red (false) header satisfies color==red && parent->parent==self
    h.color = false; h.parent = &n[4]; n[4].parent = &h; h.left = &n[1]; h.right = &n[7]; h.key = -1;
    Iter it; it.node = &h;
    int ok = 1;
    for (int k = 7; k >= 1; --k) { Iter& r = --it; ok &= &r == &it; ok &= it.node == &n[k]; }
    std::printf(ok ? "00bd7e30_TEST PASS\n" : "00bd7e30_TEST FAIL\n");
    return ok ? 0 : 1;
}
