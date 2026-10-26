#include <cstdio>

class CInputProcessTavernGame { public:
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CInputProcessTavernGame* g_last_dtor = 0;

void __fastcall CInputProcessTavernGame_dtor(CInputProcessTavernGame* process)
{
    g_last_dtor = process;
    process->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CInputProcessTavernGame_scalar_deleting_destructor(CInputProcessTavernGame* process, int edx_unused, unsigned int flags)
{
    CInputProcessTavernGame_dtor(process);
    if (flags & 1)
        operator delete(process);
    return process;
}

int main()
{
    CInputProcessTavernGame a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CInputProcessTavernGame_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CInputProcessTavernGame b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CInputProcessTavernGame_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_0048baa0_TEST PASS\n");
    return 0;
}