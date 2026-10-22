#include <cstdio>

struct CMovie {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CMovie* g_last_dtor = 0;

void __fastcall CMovie_dtor(CMovie* movie)
{
    g_last_dtor = movie;
    movie->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CMovie_scalar_deleting_destructor(CMovie* movie, int edx_unused, unsigned int flags)
{
    CMovie_dtor(movie);
    if (flags & 1)
        operator delete(movie);
    return movie;
}

int main()
{
    CMovie a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CMovie_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CMovie b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CMovie_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00442980_TEST PASS\n");
    return 0;
}