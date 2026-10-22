#include <cstdio>

struct CTCWillLightning {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CTCWillLightning* g_last_dtor = 0;

void __fastcall CTCWillLightning_dtor(CTCWillLightning* lightning)
{
    g_last_dtor = lightning;
    lightning->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CTCWillLightning_scalar_deleting_destructor(CTCWillLightning* lightning, int edx_unused, unsigned int flags)
{
    CTCWillLightning_dtor(lightning);
    if (flags & 1)
        operator delete(lightning);
    return lightning;
}

int main()
{
    CTCWillLightning a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CTCWillLightning_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CTCWillLightning b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CTCWillLightning_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00442980_TEST PASS\n");
    return 0;
}