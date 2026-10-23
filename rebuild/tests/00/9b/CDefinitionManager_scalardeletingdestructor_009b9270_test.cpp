#include <cstdio>

struct CDefinitionManager {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CDefinitionManager* g_last_dtor = 0;

void __fastcall CDefinitionManager_dtor(CDefinitionManager* manager)
{
    g_last_dtor = manager;
    manager->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CDefinitionManager_scalar_deleting_destructor(CDefinitionManager* manager, int edx_unused, unsigned int flags)
{
    CDefinitionManager_dtor(manager);
    if (flags & 1)
        operator delete(manager);
    return manager;
}

int main()
{
    CDefinitionManager a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CDefinitionManager_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CDefinitionManager b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CDefinitionManager_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_009b9270_TEST PASS\n");
    return 0;
}