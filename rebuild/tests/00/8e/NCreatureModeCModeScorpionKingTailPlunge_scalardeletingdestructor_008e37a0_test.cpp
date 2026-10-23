#include <cstdio>

struct CModeScorpionKingTailPlunge {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CModeScorpionKingTailPlunge* g_last_dtor = 0;

void __fastcall CModeScorpionKingTailPlunge_dtor(CModeScorpionKingTailPlunge* mode)
{
    g_last_dtor = mode;
    mode->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CModeScorpionKingTailPlunge_scalar_deleting_destructor(CModeScorpionKingTailPlunge* mode, int edx_unused, unsigned int flags)
{
    CModeScorpionKingTailPlunge_dtor(mode);
    if (flags & 1)
        operator delete(mode);
    return mode;
}

int main()
{
    CModeScorpionKingTailPlunge a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CModeScorpionKingTailPlunge_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CModeScorpionKingTailPlunge b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CModeScorpionKingTailPlunge_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_008e37a0_TEST PASS\n");
    return 0;
}