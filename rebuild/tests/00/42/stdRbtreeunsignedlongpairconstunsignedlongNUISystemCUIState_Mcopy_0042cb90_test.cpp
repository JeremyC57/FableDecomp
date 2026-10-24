#include <cstdio>
#include <cstdlib>
// Behaviour test for _Rb_tree<unsigned long, pair<const unsigned long, CUIState>>::_M_copy (retail 0x0042CB90): a
// structural copy of a subtree -- same shape, same colours and values, parent links pointing into the copy, the top
// node's parent set to the given parent.
struct Node { bool color; Node* parent; Node* left; Node* right; unsigned long key; };
static int g_clones = 0;
static Node* CloneNode(Node* x)
{
    Node* n = (Node*)std::malloc(sizeof(Node));
    n->key = x->key; n->color = x->color; n->left = 0; n->right = 0; n->parent = 0;
    ++g_clones;
    return n;
}
static Node* Copy(Node* x, Node* p)
{
    Node* top = CloneNode(x);
    top->parent = p;
    if (x->right)
        top->right = Copy(x->right, top);
    p = top;
    x = x->left;
    while (x != 0) {
        Node* y = CloneNode(x);
        p->left = y;
        y->parent = p;
        if (x->right)
            y->right = Copy(x->right, y);
        p = y;
        x = x->left;
    }
    return top;
}
static Node g_n[7];
static int Same(const Node* a, const Node* b, const Node* parentOfB)
{
    if (!a || !b) return a == b;
    if (a->key != b->key || a->color != b->color || b->parent != parentOfB || a == b) return 0;
    return Same(a->left, b->left, b) && Same(a->right, b->right, b);
}
static void Link(int p, int l, int r)
{
    g_n[p].left = l >= 0 ? &g_n[l] : 0; g_n[p].right = r >= 0 ? &g_n[r] : 0;
    if (l >= 0) g_n[l].parent = &g_n[p];
    if (r >= 0) g_n[r].parent = &g_n[p];
}
int main()
{
    int i;
    for (i = 0; i < 7; ++i) { g_n[i].key = 10 * (i + 1); g_n[i].color = (i & 1) != 0; g_n[i].left = g_n[i].right = g_n[i].parent = 0; }
    Link(3, 1, 5); Link(1, 0, 2); Link(5, 4, 6);         // a full 7-node tree rooted at g_n[3]
    Node header;
    Node* copy = Copy(&g_n[3], &header);
    int ok = g_clones == 7 && copy->parent == &header && Same(&g_n[3], copy, &header);
    g_n[0].right = 0; g_n[0].left = 0; Link(2, -1, -1);
    Node* leaf = Copy(&g_n[2], 0);
    ok = ok && g_clones == 8 && leaf->left == 0 && leaf->right == 0 && leaf->key == 30 && leaf->parent == 0;
    if (!ok) { std::printf("0042cb90_TEST FAIL\n"); return 1; }
    std::printf("0042cb90_TEST PASS\n");
    return 0;
}