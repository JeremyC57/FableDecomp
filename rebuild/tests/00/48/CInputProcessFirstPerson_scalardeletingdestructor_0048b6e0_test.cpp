#include <cstdio>

class CInputProcessFirstPerson { public:
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CInputProcessFirstPerson* g_last_dtor = 0;

void __fastcall CInputProcessFirstPerson_dtor(CInputProcessFirstPerson* process)
{
    g_last_dtor = process;
    process->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CInputProcessFirstPerson_scalar_deleting_destructor(CInputProcessFirstPerson* process, int edx_unused, unsigned int flags)
{
    CInputProcessFirstPerson_dtor(process);
    if (flags & 1)
        operator delete(process);
    return process;
}

int main()
{
    CInputProcessFirstPerson a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CInputProcessFirstPerson_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CInputProcessFirstPerson b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CInputProcessFirstPerson_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_0048b6e0_TEST PASS\n");
    return 0;
}