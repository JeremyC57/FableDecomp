// behaviour test for 0x0042A879 _Rb_tree<long, ...>::_M_find: exact-key lookup over a signed key (hits, misses between keys, below the minimum, above the maximum, negative keys), empty tree returns the header
#include <cstdio>
enum EEngine2DLayer { ENGINE_2D_LAYER_0 };
typedef long K;
struct Node { bool color; Node* parent; Node* left; Node* right; K key; long mapped; };
struct Tree { Node* header; unsigned int count; };
static Node* Find(const Tree& t, const K& k)
{
    Node* y = t.header;
    Node* x = t.header->parent;
    while (x != 0)
        if (!(x->key < k)) { y = x; x = x->left; }
        else x = x->right;
    if (y == t.header || k < y->key) y = t.header;
    return y;
}
static Node gNodes[16];
static int gUsed = 0;
static Node* Insert(Node* root, Node* parent, long v)
{
    if (root == 0) {
        Node* n = &gNodes[gUsed++];
        n->color = false; n->parent = parent; n->left = 0; n->right = 0; n->key = (K)v; n->mapped = v * 10;
        return n;
    }
    if ((K)v < root->key) root->left = Insert(root->left, root, v);
    else root->right = Insert(root->right, root, v);
    return root;
}
int main()
{
    Node header = { false, 0, 0, 0, (K)0, 0 };
    Tree t = { &header, 0 };
    int fails = 0;
    long kq = 4;
    if (Find(t, (K)kq) != &header) fails |= 1;                    // empty tree
    const long keys[] = { 5, -3, 12, 0, 7, -20, 9 };
    for (int i = 0; i < 7; ++i) { header.parent = Insert(header.parent, &header, keys[i]); ++t.count; }
    for (int i = 0; i < 7; ++i) {
        Node* n = Find(t, (K)keys[i]);
        if (n == &header || n->mapped != keys[i] * 10) fails |= 2;
    }
    const long misses[] = { 1, 6, 8, -21, 13, -4, 100 };
    for (int i = 0; i < 7; ++i)
        if (Find(t, (K)misses[i]) != &header) fails |= 4;
    if (fails) { std::printf("0042a879_TEST FAIL %d\n", fails); return 1; }
    std::printf("0042a879_TEST PASS\n");
    return 0;
}