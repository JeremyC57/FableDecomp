#include <cstdio>

class CInputProcessZTarget { public:
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CInputProcessZTarget* g_last_dtor = 0;

void __fastcall CInputProcessZTarget_dtor(CInputProcessZTarget* process)
{
    g_last_dtor = process;
    process->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CInputProcessZTarget_scalar_deleting_destructor(CInputProcessZTarget* process, int edx_unused, unsigned int flags)
{
    CInputProcessZTarget_dtor(process);
    if (flags & 1)
        operator delete(process);
    return process;
}

int main()
{
    CInputProcessZTarget a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CInputProcessZTarget_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CInputProcessZTarget b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CInputProcessZTarget_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_0048cea0_TEST PASS\n");
    return 0;
}