// Behaviour test for 0x007AE0E0 _Rb_tree<CCharString, pair<const CCharString, vector<CCharString> > >::_M_erase.
#include <cstdio>
#include <cstdlib>

static int g_strDtors = 0, g_frees = 0;
static void Free(void* p) { ++g_frees; std::free(p); }
class CCharString { public: int id; void Destroy() { ++g_strDtors; } };
struct StrVec { CCharString* start; CCharString* finish; CCharString* eos; };
struct Node { Node* parent; Node* left; Node* right; CCharString key; StrVec vec; };

static void Erase(Node* x)
{
    while (x != 0) {
        Erase(x->right);
        Node* y = x->left;
        for (CCharString* p = x->vec.start; p != x->vec.finish; ++p) p->Destroy();
        if (x->vec.start != 0) Free(x->vec.start);
        x->key.Destroy();
        Free(x);
        x = y;
    }
}

static Node* Make(int strings)
{
    Node* n = (Node*)std::malloc(sizeof(Node));
    n->left = n->right = n->parent = 0;
    n->vec.start = strings ? (CCharString*)std::malloc(strings * sizeof(CCharString)) : 0;
    n->vec.finish = n->vec.eos = n->vec.start + strings;
    return n;
}

int main()
{
    // root with two strings, left child with none, right child with three, right-left grandchild with one
    Node* root = Make(2); Node* l = Make(0); Node* r = Make(3); Node* rl = Make(1);
    root->left = l; root->right = r; r->left = rl;
    Erase(root);
    // strings: 4 keys + 6 vector elements; frees: 4 nodes + 3 non-empty vectors
    if (g_strDtors != 10 || g_frees != 7) { std::printf("007ae0e0_TEST FAIL\n"); return 1; }
    Erase(0);
    if (g_frees != 7) { std::printf("007ae0e0_TEST FAIL\n"); return 1; }
    std::printf("007ae0e0_TEST PASS\n");
    return 0;
}