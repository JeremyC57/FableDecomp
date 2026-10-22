#include <cstdio>

static int g_dtor_called = 0;
static int g_delete_called = 0;

extern "C" void __fastcall CTattooDef_dtor(void* def)
{
    (void)def;
    g_dtor_called++;
}
extern "C" void __cdecl fable_operator_delete(void* p)
{
    (void)p;
    g_delete_called++;
}

void* __fastcall CTattooDef_scalar_deleting_destructor(void* def, int edx, unsigned int flags)
{
    (void)edx;
    CTattooDef_dtor(def);
    if (flags & 1)
        fable_operator_delete(def);
    return def;
}

int main()
{
    int obj = 0;
    void* p = &obj;

    g_dtor_called = 0; g_delete_called = 0;
    void* r = CTattooDef_scalar_deleting_destructor(p, 0, 0);
    if (r != p || g_dtor_called != 1 || g_delete_called != 0) {
        std::printf("FAIL: flags=0 path dtor=%d del=%d r=%p\n", g_dtor_called, g_delete_called, r);
        return 1;
    }

    g_dtor_called = 0; g_delete_called = 0;
    r = CTattooDef_scalar_deleting_destructor(p, 0, 1);
    if (r != p || g_dtor_called != 1 || g_delete_called != 1) {
        std::printf("FAIL: flags=1 path dtor=%d del=%d r=%p\n", g_dtor_called, g_delete_called, r);
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_0044fa51_TEST PASS\n");
    return 0;
}