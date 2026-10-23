#include <cstdio>

struct CProjectileWeapon {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CProjectileWeapon* g_last_dtor = 0;

void __fastcall CProjectileWeapon_dtor(CProjectileWeapon* weapon)
{
    g_last_dtor = weapon;
    weapon->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CProjectileWeapon_scalar_deleting_destructor(CProjectileWeapon* weapon, int edx_unused, unsigned int flags)
{
    CProjectileWeapon_dtor(weapon);
    if (flags & 1)
        operator delete(weapon);
    return weapon;
}

int main()
{
    CProjectileWeapon a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CProjectileWeapon_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CProjectileWeapon b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CProjectileWeapon_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_0087b640_TEST PASS\n");
    return 0;
}