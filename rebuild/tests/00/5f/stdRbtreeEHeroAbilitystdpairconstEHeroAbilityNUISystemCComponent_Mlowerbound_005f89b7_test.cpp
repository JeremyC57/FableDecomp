#include <cstdio>
// Behaviour model of STLport _Rb_tree::_M_lower_bound: the first node whose key is not less than k, else the header.
struct Node { Node* left; Node* right; long key; };
static Node* lower_bound(Node* header, Node* root, const long& k)
{
    Node* y = header; Node* x = root;
    while (x != 0) if (!(x->key < k)) { y = x; x = x->left; } else x = x->right;
    return y;
}
int main()
{
    Node header; Node n[5];
    long keys[5] = { 50, 20, 80, -5, 30 };
    for (int i = 0; i < 5; ++i) { n[i].key = keys[i]; n[i].left = n[i].right = 0; }
    n[0].left = &n[1]; n[0].right = &n[2]; n[1].left = &n[3]; n[1].right = &n[4];
    if (lower_bound(&header, &n[0], 20) != &n[1] || lower_bound(&header, &n[0], 21) != &n[4] || lower_bound(&header, &n[0], -100) != &n[3]
        || lower_bound(&header, &n[0], 81) != &header || lower_bound(&header, &n[0], 31) != &n[0] || lower_bound(&header, 0, 1) != &header)
        { std::printf("005f89b7_TEST FAIL\n"); return 1; }
    std::printf("005f89b7_TEST PASS\n");
    return 0;
}