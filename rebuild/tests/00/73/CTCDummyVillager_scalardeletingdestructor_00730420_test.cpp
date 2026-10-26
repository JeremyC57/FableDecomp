#include <cstdio>

class CTCDummyVillager { public:
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CTCDummyVillager* g_last_dtor = 0;

void __fastcall CTCDummyVillager_dtor(CTCDummyVillager* villager)
{
    g_last_dtor = villager;
    villager->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CTCDummyVillager_scalar_deleting_destructor(CTCDummyVillager* villager, int edx_unused, unsigned int flags)
{
    CTCDummyVillager_dtor(villager);
    if (flags & 1)
        operator delete(villager);
    return villager;
}

int main()
{
    CTCDummyVillager a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CTCDummyVillager_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CTCDummyVillager b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CTCDummyVillager_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00730420_TEST PASS\n");
    return 0;
}