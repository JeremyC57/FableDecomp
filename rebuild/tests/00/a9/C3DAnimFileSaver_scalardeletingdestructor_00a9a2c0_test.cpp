#include <cstdio>

struct C3DAnimFileSaver {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static C3DAnimFileSaver* g_last_dtor = 0;

void __fastcall C3DAnimFileSaver_dtor(C3DAnimFileSaver* saver)
{
    g_last_dtor = saver;
    saver->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall C3DAnimFileSaver_scalar_deleting_destructor(C3DAnimFileSaver* saver, int edx_unused, unsigned int flags)
{
    C3DAnimFileSaver_dtor(saver);
    if (flags & 1)
        operator delete(saver);
    return saver;
}

int main()
{
    C3DAnimFileSaver a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = C3DAnimFileSaver_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    C3DAnimFileSaver b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = C3DAnimFileSaver_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00a9a2c0_TEST PASS\n");
    return 0;
}