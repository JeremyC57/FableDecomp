// Behaviour test for 0x00B2E900 std::_Rb_tree<long key ...>::_M_find(const key&) const -- the node with the key, or the header
#include <cstdio>
struct Node { bool color; Node* parent; Node* left; Node* right; long key; };
struct Tree {
    Node* header;
    Node* M_find(const long& k) const
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
};
int main()
{
    Node h, n[5];
    long keys[5] = { 10, 20, 30, 40, 50 };
    for (int i = 0; i < 5; ++i) { n[i].key = keys[i]; n[i].left = n[i].right = n[i].parent = 0; }
    n[2].left = &n[1]; n[1].left = &n[0]; n[2].right = &n[3]; n[3].right = &n[4];
    h.parent = 0; Tree t; t.header = &h;
    long k = 20;
    if (t.M_find(k) != &h) { std::printf("00b2e900_TEST FAIL\n"); return 1; }  // empty tree
    h.parent = &n[2];
    for (int i = 0; i < 5; ++i) if (t.M_find(keys[i]) != &n[i]) { std::printf("00b2e900_TEST FAIL\n"); return 1; }
    k = 25; if (t.M_find(k) != &h) { std::printf("00b2e900_TEST FAIL\n"); return 1; }
    k = -5; if (t.M_find(k) != &h) { std::printf("00b2e900_TEST FAIL\n"); return 1; }
    k = 99; if (t.M_find(k) != &h) { std::printf("00b2e900_TEST FAIL\n"); return 1; }
    std::printf("00b2e900_TEST PASS\n");
    return 0;
}