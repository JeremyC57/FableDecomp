#include <cstdio>

class CGameScriptThingManager { public:
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CGameScriptThingManager* g_last_dtor = 0;

void __fastcall CGameScriptThingManager_dtor(CGameScriptThingManager* manager)
{
    g_last_dtor = manager;
    manager->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CGameScriptThingManager_scalar_deleting_destructor(CGameScriptThingManager* manager, int edx_unused, unsigned int flags)
{
    CGameScriptThingManager_dtor(manager);
    if (flags & 1)
        operator delete(manager);
    return manager;
}

int main()
{
    CGameScriptThingManager a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CGameScriptThingManager_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CGameScriptThingManager b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CGameScriptThingManager_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_004bb7f0_TEST PASS\n");
    return 0;
}