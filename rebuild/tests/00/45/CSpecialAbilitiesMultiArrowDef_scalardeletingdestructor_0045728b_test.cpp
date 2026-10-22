#include <cstdio>

struct CSpecialAbilitiesMultiArrowDef { int f0; };

static int g_dtor_called = 0;
static int g_delete_called = 0;

void __fastcall CSpecialAbilitiesMultiArrowDef_dtor(CSpecialAbilitiesMultiArrowDef* def) { (void)def; g_dtor_called++; }
void __cdecl eng_operator_delete(void* p) { (void)p; g_delete_called++; }

void* __fastcall CSpecialAbilitiesMultiArrowDef_scalar_deleting_destructor(CSpecialAbilitiesMultiArrowDef* def, void* edx, unsigned int flags)
{
    (void)edx;
    CSpecialAbilitiesMultiArrowDef_dtor(def);
    if (flags & 1)
        eng_operator_delete(def);
    return def;
}

int main() {
    CSpecialAbilitiesMultiArrowDef obj; obj.f0 = 42;
    g_dtor_called = 0; g_delete_called = 0;
    void* r1 = CSpecialAbilitiesMultiArrowDef_scalar_deleting_destructor(&obj, 0, 0);
    if (r1 != &obj || g_dtor_called != 1 || g_delete_called != 0) { std::printf("FAIL no-delete path\n"); return 1; }
    g_dtor_called = 0; g_delete_called = 0;
    void* r2 = CSpecialAbilitiesMultiArrowDef_scalar_deleting_destructor(&obj, 0, 1);
    if (r2 != &obj || g_dtor_called != 1 || g_delete_called != 1) { std::printf("FAIL delete path\n"); return 1; }
    std::printf("CLandscapeBackgroundPatch_0045728b_TEST PASS\n");
    return 0;
}