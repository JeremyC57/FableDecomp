// Behaviour test for 0x00A2DC20 _Rb_tree<CCharString, pair<const CCharString, CArray<CLipSyncEntry> > >::_M_erase:
// every node of the subtree is visited once: its entries' strings, its array storage, its key and the node are released.
#include <cstdio>
#include <cstdlib>

static int g_strings = 0;      // live strings
static int g_frees = 0;
static void Free(void* p) { ++g_frees; std::free(p); }

class CCharString { public:
    int id;
    void Init(int i) { id = i; ++g_strings; }
    void Destroy() { --g_strings; }
};
struct CLipSyncEntry { CCharString PhonemeName; unsigned int AnimationIndex; };
struct CArray {
    CLipSyncEntry* start; CLipSyncEntry* finish; CLipSyncEntry* eos;
    void Destroy()
    {
        for (CLipSyncEntry* p = start; p != finish; ++p) p->PhonemeName.Destroy();
        if (start != 0) Free(start);
    }
};
struct Node {
    unsigned char color; Node* parent; Node* left; Node* right;
    CCharString key; CArray value;
};
static Node* MakeNode(int key, int entries)
{
    Node* n = (Node*)std::malloc(sizeof(Node));
    n->color = 0; n->parent = 0; n->left = 0; n->right = 0;
    n->key.Init(key);
    n->value.start = entries ? (CLipSyncEntry*)std::malloc(entries * sizeof(CLipSyncEntry)) : 0;
    for (int i = 0; i < entries; ++i) { n->value.start[i].PhonemeName.Init(key * 10 + i); n->value.start[i].AnimationIndex = i; }
    n->value.finish = n->value.start + entries;
    n->value.eos = n->value.finish;
    return n;
}
struct Tree {
    void Erase(Node* x)
    {
        while (x != 0) {
            Erase(x->right);
            Node* y = x->left;
            x->value.Destroy();
            x->key.Destroy();
            Free(x);
            x = y;
        }
    }
};

int main()
{
    Node* root = MakeNode(4, 2);
    root->left = MakeNode(2, 0);
    root->right = MakeNode(6, 3);
    root->left->left = MakeNode(1, 1);
    root->left->right = MakeNode(3, 0);
    root->right->left = MakeNode(5, 2);
    if (g_strings != 6 + 8) { std::printf("00a2dc20_TEST FAIL\n"); return 1; }
    Tree t;
    t.Erase(root->right);
    root->right = 0;
    if (g_strings != 4 + 3 || g_frees != 2 + 2) { std::printf("00a2dc20_TEST FAIL\n"); return 1; }
    t.Erase(root);
    if (g_strings != 0 || g_frees != 6 + 4) { std::printf("00a2dc20_TEST FAIL\n"); return 1; }
    t.Erase(0);
    std::printf("00a2dc20_TEST PASS\n");
    return 0;
}