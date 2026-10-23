#include <cstdio>

struct CEngineInternalPrimitiveChanger {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CEngineInternalPrimitiveChanger* g_last_dtor = 0;

void __fastcall CEngineInternalPrimitiveChanger_dtor(CEngineInternalPrimitiveChanger* changer)
{
    g_last_dtor = changer;
    changer->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CEngineInternalPrimitiveChanger_scalar_deleting_destructor(CEngineInternalPrimitiveChanger* changer, int edx_unused, unsigned int flags)
{
    CEngineInternalPrimitiveChanger_dtor(changer);
    if (flags & 1)
        operator delete(changer);
    return changer;
}

int main()
{
    CEngineInternalPrimitiveChanger a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CEngineInternalPrimitiveChanger_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CEngineInternalPrimitiveChanger b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CEngineInternalPrimitiveChanger_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00bc5f30_TEST PASS\n");
    return 0;
}