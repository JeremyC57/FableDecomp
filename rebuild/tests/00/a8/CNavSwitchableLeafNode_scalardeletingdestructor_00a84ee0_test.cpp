#include <cstdio>

struct CNavSwitchableLeafNode {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CNavSwitchableLeafNode* g_last_dtor = 0;

void __fastcall CNavSwitchableLeafNode_dtor(CNavSwitchableLeafNode* node)
{
    g_last_dtor = node;
    node->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CNavSwitchableLeafNode_scalar_deleting_destructor(CNavSwitchableLeafNode* node, int edx_unused, unsigned int flags)
{
    CNavSwitchableLeafNode_dtor(node);
    if (flags & 1)
        operator delete(node);
    return node;
}

int main()
{
    CNavSwitchableLeafNode a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CNavSwitchableLeafNode_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CNavSwitchableLeafNode b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CNavSwitchableLeafNode_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00a84ee0_TEST PASS\n");
    return 0;
}