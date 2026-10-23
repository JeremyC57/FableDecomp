#include <cstdio>

struct CTCWeapon {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CTCWeapon* g_last_dtor = 0;

void __fastcall CTCWeapon_dtor(CTCWeapon* weapon)
{
    g_last_dtor = weapon;
    weapon->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CTCWeapon_scalar_deleting_destructor(CTCWeapon* weapon, int edx_unused, unsigned int flags)
{
    CTCWeapon_dtor(weapon);
    if (flags & 1)
        operator delete(weapon);
    return weapon;
}

int main()
{
    CTCWeapon a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CTCWeapon_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CTCWeapon b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CTCWeapon_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_005dae20_TEST PASS\n");
    return 0;
}