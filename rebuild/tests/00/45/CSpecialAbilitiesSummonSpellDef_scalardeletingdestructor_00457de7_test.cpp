#include <cstdio>

struct CSpecialAbilitiesSummonSpellDef { void* vftable; };

static int g_dtor_calls = 0;
static int g_delete_calls = 0;

void __fastcall CSpecialAbilitiesSummonSpellDef_dtor(CSpecialAbilitiesSummonSpellDef* def) { (void)def; ++g_dtor_calls; }
void __cdecl op_delete(void* p) { (void)p; ++g_delete_calls; }

void* __fastcall CSpecialAbilitiesSummonSpellDef_scalar_deleting_destructor(CSpecialAbilitiesSummonSpellDef* def, int, unsigned int flags)
{
    CSpecialAbilitiesSummonSpellDef_dtor(def);
    if (flags & 1)
        op_delete(def);
    return def;
}

int main() {
    CSpecialAbilitiesSummonSpellDef obj;
    obj.vftable = 0;
    void* r = CSpecialAbilitiesSummonSpellDef_scalar_deleting_destructor(&obj, 0, 0);
    if (r != &obj || g_dtor_calls != 1 || g_delete_calls != 0) { std::printf("FAIL no-delete\n"); return 1; }
    r = CSpecialAbilitiesSummonSpellDef_scalar_deleting_destructor(&obj, 0, 1);
    if (r != &obj || g_dtor_calls != 2 || g_delete_calls != 1) { std::printf("FAIL delete\n"); return 1; }
    r = CSpecialAbilitiesSummonSpellDef_scalar_deleting_destructor(&obj, 0, 2);
    if (g_delete_calls != 1) { std::printf("FAIL bit-mask\n"); return 1; }
    std::printf("CLandscapeBackgroundPatch_00457de7_TEST PASS\n");
    return 0;
}