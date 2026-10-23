#include <cstdio>

struct CSpellContainer {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CSpellContainer* g_last_dtor = 0;

void __fastcall CSpellContainer_dtor(CSpellContainer* container)
{
    g_last_dtor = container;
    container->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CSpellContainer_scalar_deleting_destructor(CSpellContainer* container, int edx_unused, unsigned int flags)
{
    CSpellContainer_dtor(container);
    if (flags & 1)
        operator delete(container);
    return container;
}

int main()
{
    CSpellContainer a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CSpellContainer_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CSpellContainer b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CSpellContainer_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_005460a0_TEST PASS\n");
    return 0;
}