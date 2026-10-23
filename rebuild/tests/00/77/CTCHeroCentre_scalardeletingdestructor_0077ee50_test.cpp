#include <cstdio>

struct CTCHeroCentre {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CTCHeroCentre* g_last_dtor = 0;

void __fastcall CTCHeroCentre_dtor(CTCHeroCentre* centre)
{
    g_last_dtor = centre;
    centre->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CTCHeroCentre_scalar_deleting_destructor(CTCHeroCentre* centre, int edx_unused, unsigned int flags)
{
    CTCHeroCentre_dtor(centre);
    if (flags & 1)
        operator delete(centre);
    return centre;
}

int main()
{
    CTCHeroCentre a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CTCHeroCentre_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CTCHeroCentre b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CTCHeroCentre_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_0077ee50_TEST PASS\n");
    return 0;
}