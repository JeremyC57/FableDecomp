#include <cstdio>

struct CEngineInternalPrimitiveWeaponTrail {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CEngineInternalPrimitiveWeaponTrail* g_last_dtor = 0;

void __fastcall CEngineInternalPrimitiveWeaponTrail_dtor(CEngineInternalPrimitiveWeaponTrail* trail)
{
    g_last_dtor = trail;
    trail->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CEngineInternalPrimitiveWeaponTrail_scalar_deleting_destructor(CEngineInternalPrimitiveWeaponTrail* trail, int edx_unused, unsigned int flags)
{
    CEngineInternalPrimitiveWeaponTrail_dtor(trail);
    if (flags & 1)
        operator delete(trail);
    return trail;
}

int main()
{
    CEngineInternalPrimitiveWeaponTrail a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CEngineInternalPrimitiveWeaponTrail_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CEngineInternalPrimitiveWeaponTrail b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CEngineInternalPrimitiveWeaponTrail_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00bcde20_TEST PASS\n");
    return 0;
}