#include <cstdio>

struct CModeJackOfBladesScream {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CModeJackOfBladesScream* g_last_dtor = 0;

void __fastcall CModeJackOfBladesScream_dtor(CModeJackOfBladesScream* mode)
{
    g_last_dtor = mode;
    mode->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CModeJackOfBladesScream_scalar_deleting_destructor(CModeJackOfBladesScream* mode, int edx_unused, unsigned int flags)
{
    CModeJackOfBladesScream_dtor(mode);
    if (flags & 1)
        operator delete(mode);
    return mode;
}

int main()
{
    CModeJackOfBladesScream a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CModeJackOfBladesScream_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CModeJackOfBladesScream b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CModeJackOfBladesScream_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00442980_TEST PASS\n");
    return 0;
}