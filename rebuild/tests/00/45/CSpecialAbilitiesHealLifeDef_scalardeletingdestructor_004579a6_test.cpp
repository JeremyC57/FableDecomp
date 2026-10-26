#include <cstdio>
class CSpecialAbilitiesHealLifeDef { public: void* vftable; };
static int g_dtor = 0;
static int g_del = 0;
static void* g_deleted = 0;
extern "C" void __fastcall CSpecialAbilitiesHealLifeDef_dtor(CSpecialAbilitiesHealLifeDef* def){ g_dtor++; }
extern "C" void __cdecl fable_operator_delete(void* p){ g_del++; g_deleted = p; }

void* __fastcall CSpecialAbilitiesHealLifeDef_scalar_deleting_destructor(CSpecialAbilitiesHealLifeDef* def, int edx_unused, unsigned int flags)
{
    CSpecialAbilitiesHealLifeDef_dtor(def);
    if (flags & 1)
        fable_operator_delete(def);
    return def;
}

int main(){
    CSpecialAbilitiesHealLifeDef obj; obj.vftable = 0;
    g_dtor = g_del = 0; g_deleted = 0;
    void* r = CSpecialAbilitiesHealLifeDef_scalar_deleting_destructor(&obj, 0, 0);
    if (r != &obj || g_dtor != 1 || g_del != 0){ std::printf("FAIL noflag\n"); return 1; }
    g_dtor = g_del = 0; g_deleted = 0;
    r = CSpecialAbilitiesHealLifeDef_scalar_deleting_destructor(&obj, 0, 1);
    if (r != &obj || g_dtor != 1 || g_del != 1 || g_deleted != &obj){ std::printf("FAIL flag\n"); return 1; }
    g_dtor = g_del = 0;
    r = CSpecialAbilitiesHealLifeDef_scalar_deleting_destructor(&obj, 0, 2);
    if (r != &obj || g_dtor != 1 || g_del != 0){ std::printf("FAIL flag2\n"); return 1; }
    std::printf("CLandscapeBackgroundPatch_004579a6_TEST PASS\n");
    return 0;
}