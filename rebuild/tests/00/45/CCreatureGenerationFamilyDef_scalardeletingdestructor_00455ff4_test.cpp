#include <cstdio>
struct CCreatureGenerationFamilyDef { int f0; };
static int g_dtor_called = 0;
static int g_delete_called = 0;
static void __fastcall CCreatureGenerationFamilyDef_dtor(CCreatureGenerationFamilyDef* def) { (void)def; g_dtor_called++; }
static void __cdecl eng_operator_delete(void* p) { (void)p; g_delete_called++; }

static void* __fastcall CCreatureGenerationFamilyDef_scalar_deleting_destructor(CCreatureGenerationFamilyDef* def, int edx_unused, unsigned int flags)
{
    (void)edx_unused;
    CCreatureGenerationFamilyDef_dtor(def);
    if (flags & 1)
        eng_operator_delete(def);
    return def;
}

int main() {
    CCreatureGenerationFamilyDef obj; obj.f0 = 5;
    void* r = CCreatureGenerationFamilyDef_scalar_deleting_destructor(&obj, 0, 0);
    if (r != &obj || g_dtor_called != 1 || g_delete_called != 0) { std::printf("FAIL flags0\n"); return 1; }
    r = CCreatureGenerationFamilyDef_scalar_deleting_destructor(&obj, 0, 1);
    if (r != &obj || g_dtor_called != 2 || g_delete_called != 1) { std::printf("FAIL flags1\n"); return 1; }
    r = CCreatureGenerationFamilyDef_scalar_deleting_destructor(&obj, 0, 2);
    if (r != &obj || g_delete_called != 1) { std::printf("FAIL flags2\n"); return 1; }
    std::printf("CLandscapeBackgroundPatch_00455ff4_TEST PASS\n");
    return 0;
}