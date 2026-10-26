#include <cstdio>

class CTCBattleCharge { public:
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CTCBattleCharge* g_last_dtor = 0;

void __fastcall CTCBattleCharge_dtor(CTCBattleCharge* charge)
{
    g_last_dtor = charge;
    charge->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CTCBattleCharge_scalar_deleting_destructor(CTCBattleCharge* charge, int edx_unused, unsigned int flags)
{
    CTCBattleCharge_dtor(charge);
    if (flags & 1)
        operator delete(charge);
    return charge;
}

int main()
{
    CTCBattleCharge a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CTCBattleCharge_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CTCBattleCharge b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CTCBattleCharge_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_007a9800_TEST PASS\n");
    return 0;
}