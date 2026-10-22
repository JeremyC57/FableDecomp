#include <cstdio>

struct CLandscapeBackgroundPatch {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CLandscapeBackgroundPatch* g_last_dtor = 0;

void __fastcall CLandscapeBackgroundPatch_dtor(CLandscapeBackgroundPatch* patch)
{
    g_last_dtor = patch;
    patch->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CLandscapeBackgroundPatch_scalar_deleting_destructor(CLandscapeBackgroundPatch* patch, int edx_unused, unsigned int flags)
{
    CLandscapeBackgroundPatch_dtor(patch);
    if (flags & 1)
        operator delete(patch);
    return patch;
}

int main()
{
    CLandscapeBackgroundPatch a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CLandscapeBackgroundPatch_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CLandscapeBackgroundPatch b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CLandscapeBackgroundPatch_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00442980_TEST PASS\n");
    return 0;
}