#include <cstdio>

struct CTCBuyableHouse {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CTCBuyableHouse* g_last_dtor = 0;

void __fastcall CTCBuyableHouse_dtor(CTCBuyableHouse* house)
{
    g_last_dtor = house;
    house->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CTCBuyableHouse_scalar_deleting_destructor(CTCBuyableHouse* house, int edx_unused, unsigned int flags)
{
    CTCBuyableHouse_dtor(house);
    if (flags & 1)
        operator delete(house);
    return house;
}

int main()
{
    CTCBuyableHouse a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CTCBuyableHouse_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CTCBuyableHouse b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CTCBuyableHouse_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00442980_TEST PASS\n");
    return 0;
}