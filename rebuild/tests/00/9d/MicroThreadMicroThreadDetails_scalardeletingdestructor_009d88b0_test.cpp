#include <cstdio>

struct MicroThreadDetails {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static MicroThreadDetails* g_last_dtor = 0;

void __fastcall MicroThreadDetails_dtor(MicroThreadDetails* details)
{
    g_last_dtor = details;
    details->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall MicroThreadDetails_scalar_deleting_destructor(MicroThreadDetails* details, int edx_unused, unsigned int flags)
{
    MicroThreadDetails_dtor(details);
    if (flags & 1)
        operator delete(details);
    return details;
}

int main()
{
    MicroThreadDetails a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = MicroThreadDetails_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    MicroThreadDetails b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = MicroThreadDetails_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_009d88b0_TEST PASS\n");
    return 0;
}