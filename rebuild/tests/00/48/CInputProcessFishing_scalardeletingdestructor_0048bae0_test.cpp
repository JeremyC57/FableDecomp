#include <cstdio>

class CInputProcessFishing { public:
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CInputProcessFishing* g_last_dtor = 0;

void __fastcall CInputProcessFishing_dtor(CInputProcessFishing* process)
{
    g_last_dtor = process;
    process->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CInputProcessFishing_scalar_deleting_destructor(CInputProcessFishing* process, int edx_unused, unsigned int flags)
{
    CInputProcessFishing_dtor(process);
    if (flags & 1)
        operator delete(process);
    return process;
}

int main()
{
    CInputProcessFishing a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CInputProcessFishing_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CInputProcessFishing b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CInputProcessFishing_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_0048bae0_TEST PASS\n");
    return 0;
}