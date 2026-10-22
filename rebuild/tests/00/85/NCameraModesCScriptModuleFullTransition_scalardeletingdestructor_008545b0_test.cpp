#include <cstdio>

struct CScriptModuleFullTransition {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CScriptModuleFullTransition* g_last_dtor = 0;

void __fastcall CScriptModuleFullTransition_dtor(CScriptModuleFullTransition* transition)
{
    g_last_dtor = transition;
    transition->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CScriptModuleFullTransition_scalar_deleting_destructor(CScriptModuleFullTransition* transition, int edx_unused, unsigned int flags)
{
    CScriptModuleFullTransition_dtor(transition);
    if (flags & 1)
        operator delete(transition);
    return transition;
}

int main()
{
    CScriptModuleFullTransition a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CScriptModuleFullTransition_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CScriptModuleFullTransition b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CScriptModuleFullTransition_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00442980_TEST PASS\n");
    return 0;
}