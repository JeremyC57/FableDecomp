#include <cstdio>

class CInputProcessBetting { public:
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CInputProcessBetting* g_last_dtor = 0;

void __fastcall CInputProcessBetting_dtor(CInputProcessBetting* process)
{
    g_last_dtor = process;
    process->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CInputProcessBetting_scalar_deleting_destructor(CInputProcessBetting* process, int edx_unused, unsigned int flags)
{
    CInputProcessBetting_dtor(process);
    if (flags & 1)
        operator delete(process);
    return process;
}

int main()
{
    CInputProcessBetting a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CInputProcessBetting_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CInputProcessBetting b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CInputProcessBetting_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_0048bbc0_TEST PASS\n");
    return 0;
}