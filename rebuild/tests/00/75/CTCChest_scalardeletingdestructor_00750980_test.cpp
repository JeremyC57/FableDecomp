#include <cstdio>

struct CTCChest {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CTCChest* g_last_dtor = 0;

void __fastcall CTCChest_dtor(CTCChest* chest)
{
    g_last_dtor = chest;
    chest->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CTCChest_scalar_deleting_destructor(CTCChest* chest, int edx_unused, unsigned int flags)
{
    CTCChest_dtor(chest);
    if (flags & 1)
        operator delete(chest);
    return chest;
}

int main()
{
    CTCChest a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CTCChest_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CTCChest b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CTCChest_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00442980_TEST PASS\n");
    return 0;
}