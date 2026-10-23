#include <cstdio>

struct CInputProcessCutScene {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CInputProcessCutScene* g_last_dtor = 0;

void __fastcall CInputProcessCutScene_dtor(CInputProcessCutScene* process)
{
    g_last_dtor = process;
    process->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CInputProcessCutScene_scalar_deleting_destructor(CInputProcessCutScene* process, int edx_unused, unsigned int flags)
{
    CInputProcessCutScene_dtor(process);
    if (flags & 1)
        operator delete(process);
    return process;
}

int main()
{
    CInputProcessCutScene a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CInputProcessCutScene_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CInputProcessCutScene b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CInputProcessCutScene_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_0048bac0_TEST PASS\n");
    return 0;
}