#include <cstdio>

struct CTCHeroReceiveItems {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CTCHeroReceiveItems* g_last_dtor = 0;

void __fastcall CTCHeroReceiveItems_dtor(CTCHeroReceiveItems* items)
{
    g_last_dtor = items;
    items->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CTCHeroReceiveItems_scalar_deleting_destructor(CTCHeroReceiveItems* items, int edx_unused, unsigned int flags)
{
    CTCHeroReceiveItems_dtor(items);
    if (flags & 1)
        operator delete(items);
    return items;
}

int main()
{
    CTCHeroReceiveItems a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CTCHeroReceiveItems_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CTCHeroReceiveItems b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CTCHeroReceiveItems_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_007cbd80_TEST PASS\n");
    return 0;
}