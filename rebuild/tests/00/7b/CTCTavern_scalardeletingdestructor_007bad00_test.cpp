#include <cstdio>

struct CTCTavern {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CTCTavern* g_last_dtor = 0;

void __fastcall CTCTavern_dtor(CTCTavern* tavern)
{
    g_last_dtor = tavern;
    tavern->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CTCTavern_scalar_deleting_destructor(CTCTavern* tavern, int edx_unused, unsigned int flags)
{
    CTCTavern_dtor(tavern);
    if (flags & 1)
        operator delete(tavern);
    return tavern;
}

int main()
{
    CTCTavern a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CTCTavern_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CTCTavern b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CTCTavern_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00442980_TEST PASS\n");
    return 0;
}