#include <cstdio>

class CSpecialAbilitiesElectrocutedEffectDef { public: void* vftable; };

static int g_dtor_called = 0;
static int g_delete_called = 0;
static void* g_deleted = 0;

extern "C" void __fastcall CSpecialAbilitiesElectrocutedEffectDef_dtor(CSpecialAbilitiesElectrocutedEffectDef* def) { (void)def; g_dtor_called++; }
extern "C" void __cdecl eng_operator_delete(void* p) { g_delete_called++; g_deleted = p; }

void* __fastcall CSpecialAbilitiesElectrocutedEffectDef_scalar_deleting_destructor(CSpecialAbilitiesElectrocutedEffectDef* def, int edx, unsigned int flags)
{
    (void)edx;
    CSpecialAbilitiesElectrocutedEffectDef_dtor(def);
    if (flags & 1)
        eng_operator_delete(def);
    return def;
}

int main()
{
    CSpecialAbilitiesElectrocutedEffectDef obj;
    obj.vftable = 0;

    g_dtor_called = 0; g_delete_called = 0; g_deleted = 0;
    void* r0 = CSpecialAbilitiesElectrocutedEffectDef_scalar_deleting_destructor(&obj, 0, 0);
    if (r0 != &obj || g_dtor_called != 1 || g_delete_called != 0) {
        std::printf("FAIL flags=0 r=%p dtor=%d del=%d\n", r0, g_dtor_called, g_delete_called);
        return 1;
    }

    g_dtor_called = 0; g_delete_called = 0; g_deleted = 0;
    void* r1 = CSpecialAbilitiesElectrocutedEffectDef_scalar_deleting_destructor(&obj, 0, 1);
    if (r1 != &obj || g_dtor_called != 1 || g_delete_called != 1 || g_deleted != &obj) {
        std::printf("FAIL flags=1 r=%p dtor=%d del=%d\n", r1, g_dtor_called, g_delete_called);
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00457eba_TEST PASS\n");
    return 0;
}