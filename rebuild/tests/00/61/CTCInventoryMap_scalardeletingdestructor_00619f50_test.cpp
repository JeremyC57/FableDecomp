#include <cstdio>

class CTCInventoryMap { public:
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CTCInventoryMap* g_last_dtor = 0;

void __fastcall CTCInventoryMap_dtor(CTCInventoryMap* map)
{
    g_last_dtor = map;
    map->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CTCInventoryMap_scalar_deleting_destructor(CTCInventoryMap* map, int edx_unused, unsigned int flags)
{
    CTCInventoryMap_dtor(map);
    if (flags & 1)
        operator delete(map);
    return map;
}

int main()
{
    CTCInventoryMap a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CTCInventoryMap_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CTCInventoryMap b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CTCInventoryMap_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00619f50_TEST PASS\n");
    return 0;
}