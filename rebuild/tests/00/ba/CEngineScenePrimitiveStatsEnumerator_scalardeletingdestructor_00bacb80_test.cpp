#include <cstdio>

struct CEngineScenePrimitiveStatsEnumerator {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CEngineScenePrimitiveStatsEnumerator* g_last_dtor = 0;

void __fastcall CEngineScenePrimitiveStatsEnumerator_dtor(CEngineScenePrimitiveStatsEnumerator* enumerator)
{
    g_last_dtor = enumerator;
    enumerator->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CEngineScenePrimitiveStatsEnumerator_scalar_deleting_destructor(CEngineScenePrimitiveStatsEnumerator* enumerator, int edx_unused, unsigned int flags)
{
    CEngineScenePrimitiveStatsEnumerator_dtor(enumerator);
    if (flags & 1)
        operator delete(enumerator);
    return enumerator;
}

int main()
{
    CEngineScenePrimitiveStatsEnumerator a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CEngineScenePrimitiveStatsEnumerator_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CEngineScenePrimitiveStatsEnumerator b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CEngineScenePrimitiveStatsEnumerator_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00442980_TEST PASS\n");
    return 0;
}