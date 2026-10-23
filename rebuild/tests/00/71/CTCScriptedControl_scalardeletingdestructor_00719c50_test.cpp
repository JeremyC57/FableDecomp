#include <cstdio>

struct CTCScriptedControl {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CTCScriptedControl* g_last_dtor = 0;

void __fastcall CTCScriptedControl_dtor(CTCScriptedControl* control)
{
    g_last_dtor = control;
    control->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CTCScriptedControl_scalar_deleting_destructor(CTCScriptedControl* control, int edx_unused, unsigned int flags)
{
    CTCScriptedControl_dtor(control);
    if (flags & 1)
        operator delete(control);
    return control;
}

int main()
{
    CTCScriptedControl a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CTCScriptedControl_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CTCScriptedControl b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CTCScriptedControl_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00719c50_TEST PASS\n");
    return 0;
}