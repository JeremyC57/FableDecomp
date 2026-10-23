#include <cstdio>

struct CTCShop {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CTCShop* g_last_dtor = 0;

void __fastcall CTCShop_dtor(CTCShop* shop)
{
    g_last_dtor = shop;
    shop->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CTCShop_scalar_deleting_destructor(CTCShop* shop, int edx_unused, unsigned int flags)
{
    CTCShop_dtor(shop);
    if (flags & 1)
        operator delete(shop);
    return shop;
}

int main()
{
    CTCShop a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CTCShop_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CTCShop b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CTCShop_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_007783d0_TEST PASS\n");
    return 0;
}