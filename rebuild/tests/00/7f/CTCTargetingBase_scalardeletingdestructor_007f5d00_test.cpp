#include <cstdio>

class CTCTargetingBase { public:
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CTCTargetingBase* g_last_dtor = 0;

void __fastcall CTCTargetingBase_dtor(CTCTargetingBase* targeting)
{
    g_last_dtor = targeting;
    targeting->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CTCTargetingBase_scalar_deleting_destructor(CTCTargetingBase* targeting, int edx_unused, unsigned int flags)
{
    CTCTargetingBase_dtor(targeting);
    if (flags & 1)
        operator delete(targeting);
    return targeting;
}

int main()
{
    CTCTargetingBase a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CTCTargetingBase_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CTCTargetingBase b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CTCTargetingBase_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_007f5d00_TEST PASS\n");
    return 0;
}