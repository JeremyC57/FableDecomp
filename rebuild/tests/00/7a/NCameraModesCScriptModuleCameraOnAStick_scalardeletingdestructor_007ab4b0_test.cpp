#include <cstdio>

struct CScriptModuleCameraOnAStick {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CScriptModuleCameraOnAStick* g_last_dtor = 0;

void __fastcall CScriptModuleCameraOnAStick_dtor(CScriptModuleCameraOnAStick* stick)
{
    g_last_dtor = stick;
    stick->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CScriptModuleCameraOnAStick_scalar_deleting_destructor(CScriptModuleCameraOnAStick* stick, int edx_unused, unsigned int flags)
{
    CScriptModuleCameraOnAStick_dtor(stick);
    if (flags & 1)
        operator delete(stick);
    return stick;
}

int main()
{
    CScriptModuleCameraOnAStick a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CScriptModuleCameraOnAStick_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CScriptModuleCameraOnAStick b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CScriptModuleCameraOnAStick_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_007ab4b0_TEST PASS\n");
    return 0;
}