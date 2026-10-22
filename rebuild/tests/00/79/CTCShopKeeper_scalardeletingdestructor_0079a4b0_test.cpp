#include <cstdio>

struct CTCShopKeeper {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CTCShopKeeper* g_last_dtor = 0;

void __fastcall CTCShopKeeper_dtor(CTCShopKeeper* keeper)
{
    g_last_dtor = keeper;
    keeper->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CTCShopKeeper_scalar_deleting_destructor(CTCShopKeeper* keeper, int edx_unused, unsigned int flags)
{
    CTCShopKeeper_dtor(keeper);
    if (flags & 1)
        operator delete(keeper);
    return keeper;
}

int main()
{
    CTCShopKeeper a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CTCShopKeeper_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CTCShopKeeper b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CTCShopKeeper_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00442980_TEST PASS\n");
    return 0;
}