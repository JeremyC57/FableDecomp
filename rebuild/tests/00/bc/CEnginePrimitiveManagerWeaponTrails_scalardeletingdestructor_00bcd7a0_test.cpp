#include <cstdio>

struct CEnginePrimitiveManagerWeaponTrails {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CEnginePrimitiveManagerWeaponTrails* g_last_dtor = 0;

void __fastcall CEnginePrimitiveManagerWeaponTrails_dtor(CEnginePrimitiveManagerWeaponTrails* trails)
{
    g_last_dtor = trails;
    trails->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CEnginePrimitiveManagerWeaponTrails_scalar_deleting_destructor(CEnginePrimitiveManagerWeaponTrails* trails, int edx_unused, unsigned int flags)
{
    CEnginePrimitiveManagerWeaponTrails_dtor(trails);
    if (flags & 1)
        operator delete(trails);
    return trails;
}

int main()
{
    CEnginePrimitiveManagerWeaponTrails a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CEnginePrimitiveManagerWeaponTrails_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CEnginePrimitiveManagerWeaponTrails b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CEnginePrimitiveManagerWeaponTrails_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00bcd7a0_TEST PASS\n");
    return 0;
}