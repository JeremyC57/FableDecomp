#include <cstdio>
// Behaviour model of STLport _Rb_tree::_M_find over unsigned __int64 keys: the lower-bound walk, then the not-less check.
struct Node { Node* left; Node* right; unsigned long long key; };
static Node* find(Node* header, Node* root, const unsigned long long& k)
{
    Node* y = header; Node* x = root;
    while (x != 0) if (!(x->key < k)) { y = x; x = x->left; } else x = x->right;
    if (y == header || k < y->key) y = header;
    return y;
}
int main()
{
    Node header; Node n[5];
    unsigned long long keys[5] = { 0x100000005ULL, 0x100000002ULL, 0x200000000ULL, 0x100000001ULL, 0x100000003ULL };
    for (int i = 0; i < 5; ++i) { n[i].key = keys[i]; n[i].left = n[i].right = 0; }
    n[0].left = &n[1]; n[0].right = &n[2]; n[1].left = &n[3]; n[1].right = &n[4];   // BST rooted at n[0]
    for (int i = 0; i < 5; ++i) if (find(&header, &n[0], keys[i]) != &n[i]) { std::printf("00527d20_TEST FAIL\n"); return 1; }
    unsigned long long miss[4] = { 0ULL, 0x100000004ULL, 0x1FFFFFFFFULL, 0x300000000ULL };
    for (int i = 0; i < 4; ++i) if (find(&header, &n[0], miss[i]) != &header) { std::printf("00527d20_TEST FAIL\n"); return 1; }
    if (find(&header, 0, keys[0]) != &header) { std::printf("00527d20_TEST FAIL\n"); return 1; }
    std::printf("00527d20_TEST PASS\n");
    return 0;
}