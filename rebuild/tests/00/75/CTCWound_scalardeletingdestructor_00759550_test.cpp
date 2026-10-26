#include <cstdio>

class CTCWound { public:
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CTCWound* g_last_dtor = 0;

void __fastcall CTCWound_dtor(CTCWound* wound)
{
    g_last_dtor = wound;
    wound->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CTCWound_scalar_deleting_destructor(CTCWound* wound, int edx_unused, unsigned int flags)
{
    CTCWound_dtor(wound);
    if (flags & 1)
        operator delete(wound);
    return wound;
}

int main()
{
    CTCWound a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CTCWound_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CTCWound b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CTCWound_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00759550_TEST PASS\n");
    return 0;
}