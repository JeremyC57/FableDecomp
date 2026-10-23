#include <cstdio>

struct CEngineLightingManager {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CEngineLightingManager* g_last_dtor = 0;

void __fastcall CEngineLightingManager_dtor(CEngineLightingManager* manager)
{
    g_last_dtor = manager;
    manager->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CEngineLightingManager_scalar_deleting_destructor(CEngineLightingManager* manager, int edx_unused, unsigned int flags)
{
    CEngineLightingManager_dtor(manager);
    if (flags & 1)
        operator delete(manager);
    return manager;
}

int main()
{
    CEngineLightingManager a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CEngineLightingManager_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CEngineLightingManager b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CEngineLightingManager_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00b4a4a0_TEST PASS\n");
    return 0;
}