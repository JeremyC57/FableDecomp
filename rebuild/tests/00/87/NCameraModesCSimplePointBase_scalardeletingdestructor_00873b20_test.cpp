#include <cstdio>

struct CSimplePointBase {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CSimplePointBase* g_last_dtor = 0;

void __fastcall CSimplePointBase_dtor(CSimplePointBase* point)
{
    g_last_dtor = point;
    point->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CSimplePointBase_scalar_deleting_destructor(CSimplePointBase* point, int edx_unused, unsigned int flags)
{
    CSimplePointBase_dtor(point);
    if (flags & 1)
        operator delete(point);
    return point;
}

int main()
{
    CSimplePointBase a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CSimplePointBase_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CSimplePointBase b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CSimplePointBase_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00873b20_TEST PASS\n");
    return 0;
}