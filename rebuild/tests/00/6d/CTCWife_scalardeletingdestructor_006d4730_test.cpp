#include <cstdio>

struct CTCWife {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CTCWife* g_last_dtor = 0;

void __fastcall CTCWife_dtor(CTCWife* wife)
{
    g_last_dtor = wife;
    wife->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CTCWife_scalar_deleting_destructor(CTCWife* wife, int edx_unused, unsigned int flags)
{
    CTCWife_dtor(wife);
    if (flags & 1)
        operator delete(wife);
    return wife;
}

int main()
{
    CTCWife a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CTCWife_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CTCWife b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CTCWife_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00442980_TEST PASS\n");
    return 0;
}