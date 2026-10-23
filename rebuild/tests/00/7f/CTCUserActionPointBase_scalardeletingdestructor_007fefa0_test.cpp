#include <cstdio>

struct CTCUserActionPointBase {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CTCUserActionPointBase* g_last_dtor = 0;

void __fastcall CTCUserActionPointBase_dtor(CTCUserActionPointBase* point)
{
    g_last_dtor = point;
    point->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CTCUserActionPointBase_scalar_deleting_destructor(CTCUserActionPointBase* point, int edx_unused, unsigned int flags)
{
    CTCUserActionPointBase_dtor(point);
    if (flags & 1)
        operator delete(point);
    return point;
}

int main()
{
    CTCUserActionPointBase a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CTCUserActionPointBase_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CTCUserActionPointBase b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CTCUserActionPointBase_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_007fefa0_TEST PASS\n");
    return 0;
}