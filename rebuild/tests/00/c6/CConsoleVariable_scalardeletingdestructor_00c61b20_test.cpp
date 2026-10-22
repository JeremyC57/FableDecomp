#include <cstdio>

struct CConsoleVariable {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CConsoleVariable* g_last_dtor = 0;

void __fastcall CConsoleVariable_dtor(CConsoleVariable* variable)
{
    g_last_dtor = variable;
    variable->destroyed = 1;
}

void free_engine(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CConsoleVariable_scalar_deleting_destructor(CConsoleVariable* variable, int edx_unused, unsigned int flags)
{
    CConsoleVariable_dtor(variable);
    if (flags & 1)
        free_engine(variable);
    return variable;
}

int main()
{
    CConsoleVariable a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CConsoleVariable_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CConsoleVariable b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CConsoleVariable_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00442980_TEST PASS\n");
    return 0;
}