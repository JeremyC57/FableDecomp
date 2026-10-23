#include <cstdio>

struct CTCRockTrollShield {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CTCRockTrollShield* g_last_dtor = 0;

void __fastcall CTCRockTrollShield_dtor(CTCRockTrollShield* shield)
{
    g_last_dtor = shield;
    shield->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CTCRockTrollShield_scalar_deleting_destructor(CTCRockTrollShield* shield, int edx_unused, unsigned int flags)
{
    CTCRockTrollShield_dtor(shield);
    if (flags & 1)
        operator delete(shield);
    return shield;
}

int main()
{
    CTCRockTrollShield a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CTCRockTrollShield_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CTCRockTrollShield b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CTCRockTrollShield_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_007af870_TEST PASS\n");
    return 0;
}