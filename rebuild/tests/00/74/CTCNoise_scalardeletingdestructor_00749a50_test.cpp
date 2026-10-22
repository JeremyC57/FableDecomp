#include <cstdio>

struct CTCNoise {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CTCNoise* g_last_dtor = 0;

void __fastcall CTCNoise_dtor(CTCNoise* noise)
{
    g_last_dtor = noise;
    noise->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CTCNoise_scalar_deleting_destructor(CTCNoise* noise, int edx_unused, unsigned int flags)
{
    CTCNoise_dtor(noise);
    if (flags & 1)
        operator delete(noise);
    return noise;
}

int main()
{
    CTCNoise a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CTCNoise_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CTCNoise b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CTCNoise_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00442980_TEST PASS\n");
    return 0;
}