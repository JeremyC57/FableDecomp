// behaviour test for 0x00AD5240 _Rb_tree::_M_erase (std::map<unsigned long, QuadricOptimiserInternals::CContractionReference >): every node of the subtree is visited once (right subtree first, then the node, then down the left spine), each node's counted pointer drops one reference (the deleter runs and the count block is freed when it reaches zero), the pointer is cleared and the node freed
#include <cstdio>
#include <cstdlib>
struct Info { long RefCount; int deleted; };
struct Node { Node* left; Node* right; Info* info; void* data; int id; };
static int g_freed = 0;
static int g_order[16]; static int g_n = 0;
static void Release(Node* x)                                         // the inline ~CCountedPointer
{
    if (x->info) {
        --x->info->RefCount;
        if (x->info->RefCount == 0) { x->info->deleted = 1; std::free(x->info); }
    }
    x->data = 0; x->info = 0;
}
static void Erase(Node* x)                                           // the function under test
{
    while (x != 0) {
        Erase(x->right);
        Node* y = x->left;
        g_order[g_n++] = x->id;
        Release(x);
        std::free(x); ++g_freed;
        x = y;
    }
}
static Node* Mk(int id, Info* info, Node* l, Node* r)
{
    Node* n = (Node*)std::malloc(sizeof(Node)); n->left = l; n->right = r; n->info = info; n->data = info; n->id = id;
    if (info) ++info->RefCount;
    return n;
}
int main()
{
    int fail = 0;
    Info* shared = (Info*)std::malloc(sizeof(Info)); shared->RefCount = 1; shared->deleted = 0;   // one outside holder
    Info* own = (Info*)std::malloc(sizeof(Info)); own->RefCount = 0; own->deleted = 0;
    //        4
    //      2   6
    //     1 3 5 (7: empty pointer)
    Node* root = Mk(4, shared, Mk(2, shared, Mk(1, own, 0, 0), Mk(3, shared, 0, 0)), Mk(6, shared, Mk(5, shared, 0, 0), Mk(7, 0, 0, 0)));
    if (shared->RefCount != 6) fail |= 1;
    Erase(root);
    int want[7] = { 7, 6, 5, 4, 3, 2, 1 };
    if (g_n != 7 || g_freed != 7) fail |= 2;
    for (int i = 0; i < 7 && i < g_n; ++i) if (g_order[i] != want[i]) fail |= 4;
    if (shared->RefCount != 1 || shared->deleted) fail |= 8;          // the outside holder keeps it alive
    Erase(0);
    if (g_n != 7) fail |= 16;
    std::free(shared);
    if (fail) { std::printf("00ad5240_TEST FAIL %d\n", fail); return 1; }
    std::printf("00ad5240_TEST PASS\n");
    return 0;
}