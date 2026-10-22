#include <cstdio>

struct CTriangleList {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CTriangleList* g_last_dtor = 0;

void __fastcall CTriangleList_dtor(CTriangleList* list)
{
    g_last_dtor = list;
    list->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CTriangleList_scalar_deleting_destructor(CTriangleList* list, int edx_unused, unsigned int flags)
{
    CTriangleList_dtor(list);
    if (flags & 1)
        operator delete(list);
    return list;
}

int main()
{
    CTriangleList a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CTriangleList_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CTriangleList b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CTriangleList_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00442980_TEST PASS\n");
    return 0;
}