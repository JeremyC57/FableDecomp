#include <cstdio>

class CSpecialAbilitiesAssassinRushDef { public: int x; };

static int g_dtor_called = 0;
static int g_delete_called = 0;

void __fastcall CLBP_dtor(CSpecialAbilitiesAssassinRushDef* def) { (void)def; g_dtor_called++; }
void __cdecl CLBP_operator_delete(void* def) { (void)def; g_delete_called++; }

void* __fastcall CSpecialAbilitiesAssassinRushDef_scalar_deleting_destructor(CSpecialAbilitiesAssassinRushDef* def, int edx_dummy, unsigned int flags)
{
    (void)edx_dummy;
    CLBP_dtor(def);
    if (flags & 1) {
        CLBP_operator_delete(def);
    }
    return def;
}

int main()
{
    CSpecialAbilitiesAssassinRushDef obj;
    g_dtor_called = 0; g_delete_called = 0;
    void* r0 = CSpecialAbilitiesAssassinRushDef_scalar_deleting_destructor(&obj, 0, 0);
    if (r0 != &obj || g_dtor_called != 1 || g_delete_called != 0) { std::printf("FAIL flags=0\n"); return 1; }

    g_dtor_called = 0; g_delete_called = 0;
    void* r1 = CSpecialAbilitiesAssassinRushDef_scalar_deleting_destructor(&obj, 0, 1);
    if (r1 != &obj || g_dtor_called != 1 || g_delete_called != 1) { std::printf("FAIL flags=1\n"); return 1; }

    std::printf("CLandscapeBackgroundPatch_0045709b_TEST PASS\n");
    return 0;
}