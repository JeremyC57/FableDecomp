#include <cstdio>

struct CEngineResourceManager {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CEngineResourceManager* g_last_dtor = 0;

void __fastcall CEngineResourceManager_dtor(CEngineResourceManager* manager)
{
    g_last_dtor = manager;
    manager->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CEngineResourceManager_scalar_deleting_destructor(CEngineResourceManager* manager, int edx_unused, unsigned int flags)
{
    CEngineResourceManager_dtor(manager);
    if (flags & 1)
        operator delete(manager);
    return manager;
}

int main()
{
    CEngineResourceManager a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CEngineResourceManager_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CEngineResourceManager b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CEngineResourceManager_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00442980_TEST PASS\n");
    return 0;
}