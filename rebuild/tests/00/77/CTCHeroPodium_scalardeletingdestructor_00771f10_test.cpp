#include <cstdio>

class CTCHeroPodium { public:
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CTCHeroPodium* g_last_dtor = 0;

void __fastcall CTCHeroPodium_dtor(CTCHeroPodium* podium)
{
    g_last_dtor = podium;
    podium->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CTCHeroPodium_scalar_deleting_destructor(CTCHeroPodium* podium, int edx_unused, unsigned int flags)
{
    CTCHeroPodium_dtor(podium);
    if (flags & 1)
        operator delete(podium);
    return podium;
}

int main()
{
    CTCHeroPodium a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CTCHeroPodium_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CTCHeroPodium b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CTCHeroPodium_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00771f10_TEST PASS\n");
    return 0;
}