#include <cstdio>

struct CNavQuadTree {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CNavQuadTree* g_last_dtor = 0;

void __fastcall CNavQuadTree_dtor(CNavQuadTree* tree)
{
    g_last_dtor = tree;
    tree->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CNavQuadTree_scalar_deleting_destructor(CNavQuadTree* tree, int edx_unused, unsigned int flags)
{
    CNavQuadTree_dtor(tree);
    if (flags & 1)
        operator delete(tree);
    return tree;
}

int main()
{
    CNavQuadTree a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CNavQuadTree_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CNavQuadTree b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CNavQuadTree_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00442980_TEST PASS\n");
    return 0;
}