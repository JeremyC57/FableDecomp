#include <cstdio>

struct CSoundManagerLH {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CSoundManagerLH* g_last_dtor = 0;

void __fastcall CSoundManagerLH_dtor(CSoundManagerLH* managerLH)
{
    g_last_dtor = managerLH;
    managerLH->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CSoundManagerLH_scalar_deleting_destructor(CSoundManagerLH* managerLH, int edx_unused, unsigned int flags)
{
    CSoundManagerLH_dtor(managerLH);
    if (flags & 1)
        operator delete(managerLH);
    return managerLH;
}

int main()
{
    CSoundManagerLH a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CSoundManagerLH_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CSoundManagerLH b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CSoundManagerLH_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00442980_TEST PASS\n");
    return 0;
}