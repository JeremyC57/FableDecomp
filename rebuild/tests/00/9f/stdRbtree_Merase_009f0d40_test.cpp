// behaviour test for 0x009F0D40 _Rb_tree::_M_erase: every node of a hand-built tree is visited once (right subtree first, then
// the node, then down the left spine), its value destroyed last member first, and the node freed
#include <cstdio>
#include <cstdlib>
static int g_order[16]; static int g_n = 0; static int g_freed = 0;
struct Member { int id; ~Member() { g_order[g_n++] = id; } };
struct Value { int key; Member a; Member b; };              // b is destroyed before a
struct Node { char color; Node* parent; Node* left; Node* right; Value v; };
static Node* mk(int key)
{
    Node* n = (Node*)std::malloc(sizeof(Node));
    n->left = n->right = n->parent = 0; n->v.key = key; n->v.a.id = 10 * key + 1; n->v.b.id = 10 * key + 2;
    return n;
}
static void erase(Node* x)
{
    while (x != 0) {
        erase(x->right);
        Node* y = x->left;
        x->v.~Value();
        std::free(x); ++g_freed;
        x = y;
    }
}
int main()
{
    int fail = 0;
    Node* r = mk(2); Node* l = mk(1); Node* rr = mk(3);
    r->left = l; r->right = rr; l->parent = rr->parent = r;
    erase(r);
    if (g_freed != 3 || g_n != 6) fail |= 1;
    if (g_order[0] != 32 || g_order[1] != 31) fail |= 2;   // right child first, b before a
    if (g_order[2] != 22 || g_order[3] != 21) fail |= 4;   // then the node itself
    if (g_order[4] != 12 || g_order[5] != 11) fail |= 8;   // then the left child
    erase(0);
    if (g_freed != 3) fail |= 16;
    if (fail) { std::printf("009f0d40_TEST FAIL %d\n", fail); return 1; }
    std::printf("009f0d40_TEST PASS\n");
    return 0;
}