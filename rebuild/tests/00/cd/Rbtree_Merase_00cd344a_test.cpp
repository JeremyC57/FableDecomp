// Behaviour test for _Rb_tree::_M_erase (0x00CD344A, the mapActor instantiation): erase without rebalancing frees every
// node of the subtree (right subtrees recursively, left children iteratively) and destroys each value once.
#include <cstdio>
#include <cstdlib>
#include <new>

static int g_destroyed = 0;
static int g_freed = 0;
struct CValue {
    int key;
    ~CValue() { ++g_destroyed; }
};
struct CNodeBase {
    bool       color;
    CNodeBase* parent;
    CNodeBase* left;
    CNodeBase* right;
};
struct CNode : CNodeBase {
    CValue value;
};
struct CTree {
    static CNode*& Left(CNode* x) { return (CNode*&)(x->left); }
    static CNode*& Right(CNode* x) { return (CNode*&)(x->right); }
    void Deallocate(CNode* p) { if (p != 0) { ++g_freed; std::free(p); } }
    void Erase(CNode* x)
    {
        while (x != 0) {
            Erase(Right(x));
            CNode* y = Left(x);
            (&x->value)->~CValue();
            Deallocate(x);
            x = y;
        }
    }
};
static CNode* Make(int key, CNode* l, CNode* r)
{
    CNode* n = (CNode*)std::malloc(sizeof(CNode));
    n->color = false; n->parent = 0; n->left = l; n->right = r;
    new (&n->value) CValue();
    n->value.key = key;
    return n;
}

int main()
{
    // an 8-node tree with a deep right spine and left chains
    CNode* tree = Make(4, Make(2, Make(1, 0, 0), Make(3, 0, 0)), Make(6, Make(5, 0, 0), Make(7, 0, Make(8, 0, 0))));
    CTree t;
    t.Erase(tree);
    t.Erase(0);
    if (g_destroyed != 8 || g_freed != 8) {
        std::printf("00cd344a_TEST FAIL\n");
        return 1;
    }
    std::printf("00cd344a_TEST PASS\n");
    return 0;
}