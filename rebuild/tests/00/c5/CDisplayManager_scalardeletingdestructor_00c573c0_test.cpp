#include <cstdio>

struct CDisplayManager {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CDisplayManager* g_last_dtor = 0;

void __fastcall CDisplayManager_dtor(CDisplayManager* manager)
{
    g_last_dtor = manager;
    manager->destroyed = 1;
}

void free_engine(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CDisplayManager_scalar_deleting_destructor(CDisplayManager* manager, int edx_unused, unsigned int flags)
{
    CDisplayManager_dtor(manager);
    if (flags & 1)
        free_engine(manager);
    return manager;
}

int main()
{
    CDisplayManager a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CDisplayManager_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CDisplayManager b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CDisplayManager_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00c573c0_TEST PASS\n");
    return 0;
}