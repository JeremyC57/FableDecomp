#include <cstdio>

class CSpecialAbilitiesUnholyPowerDef { public: int x; };
static int g_dtor_calls = 0;
static int g_delete_calls = 0;

extern "C" void __fastcall CSpecialAbilitiesUnholyPowerDef_dtor(CSpecialAbilitiesUnholyPowerDef* def) { (void)def; g_dtor_calls++; }
extern "C" void __cdecl op_delete(void* p) { (void)p; g_delete_calls++; }

void* __fastcall CSpecialAbilitiesUnholyPowerDef_scalar_deleting_destructor(CSpecialAbilitiesUnholyPowerDef* def, int edx_unused, unsigned int flags)
{
    (void)edx_unused;
    CSpecialAbilitiesUnholyPowerDef_dtor(def);
    if (flags & 1)
        op_delete((void*)def);
    return (void*)def;
}

int main()
{
    CSpecialAbilitiesUnholyPowerDef obj;
    obj.x = 42;
    g_dtor_calls = 0; g_delete_calls = 0;
    void* r = CSpecialAbilitiesUnholyPowerDef_scalar_deleting_destructor(&obj, 0, 0);
    if (r != &obj) { std::printf("FAIL: return mismatch\n"); return 1; }
    if (g_dtor_calls != 1) { std::printf("FAIL: dtor not called\n"); return 1; }
    if (g_delete_calls != 0) { std::printf("FAIL: delete called when flag=0\n"); return 1; }

    g_dtor_calls = 0; g_delete_calls = 0;
    r = CSpecialAbilitiesUnholyPowerDef_scalar_deleting_destructor(&obj, 0, 1);
    if (r != &obj) { std::printf("FAIL: return mismatch2\n"); return 1; }
    if (g_dtor_calls != 1) { std::printf("FAIL: dtor not called2\n"); return 1; }
    if (g_delete_calls != 1) { std::printf("FAIL: delete not called when flag=1\n"); return 1; }

    std::printf("CLandscapeBackgroundPatch_004582ba_TEST PASS\n");
    return 0;
}