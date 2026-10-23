#include <cstdio>

struct CShaderStats {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CShaderStats* g_last_dtor = 0;

void __fastcall CShaderStats_dtor(CShaderStats* stats)
{
    g_last_dtor = stats;
    stats->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CShaderStats_scalar_deleting_destructor(CShaderStats* stats, int edx_unused, unsigned int flags)
{
    CShaderStats_dtor(stats);
    if (flags & 1)
        operator delete(stats);
    return stats;
}

int main()
{
    CShaderStats a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CShaderStats_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CShaderStats b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CShaderStats_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_0098de70_TEST PASS\n");
    return 0;
}