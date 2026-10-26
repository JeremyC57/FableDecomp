#include <cstdio>

class CNavNavigableLeafNode { public:
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CNavNavigableLeafNode* g_last_dtor = 0;

void __fastcall CNavNavigableLeafNode_dtor(CNavNavigableLeafNode* node)
{
    g_last_dtor = node;
    node->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CNavNavigableLeafNode_scalar_deleting_destructor(CNavNavigableLeafNode* node, int edx_unused, unsigned int flags)
{
    CNavNavigableLeafNode_dtor(node);
    if (flags & 1)
        operator delete(node);
    return node;
}

int main()
{
    CNavNavigableLeafNode a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CNavNavigableLeafNode_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CNavNavigableLeafNode b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CNavNavigableLeafNode_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00a80340_TEST PASS\n");
    return 0;
}