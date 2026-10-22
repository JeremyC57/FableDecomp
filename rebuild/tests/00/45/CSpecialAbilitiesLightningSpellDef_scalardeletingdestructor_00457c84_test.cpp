#include <cstdio>

struct CSpecialAbilitiesLightningSpellDef { void* vftable; };

static int g_dtor = 0;
static int g_delete = 0;
static void* g_last = 0;

void __fastcall CSpecialAbilitiesLightningSpellDef_dtor(CSpecialAbilitiesLightningSpellDef* def) { g_dtor++; }
void __cdecl operator delete(void* p) { g_delete++; g_last = p; }

void* __fastcall CSpecialAbilitiesLightningSpellDef_scalar_deleting_destructor(CSpecialAbilitiesLightningSpellDef* def, int edx_pad, unsigned int flags)
{
    CSpecialAbilitiesLightningSpellDef_dtor(def);
    if (flags & 1)
        operator delete((void*)def);
    return def;
}

int main()
{
    CSpecialAbilitiesLightningSpellDef obj;
    obj.vftable = 0;
    void* r = CSpecialAbilitiesLightningSpellDef_scalar_deleting_destructor(&obj, 0, 0);
    if (r != &obj || g_dtor != 1 || g_delete != 0) { std::printf("FAIL a\n"); return 1; }
    r = CSpecialAbilitiesLightningSpellDef_scalar_deleting_destructor(&obj, 0, 1);
    if (r != &obj || g_dtor != 2 || g_delete != 1 || g_last != &obj) { std::printf("FAIL b\n"); return 1; }
    r = CSpecialAbilitiesLightningSpellDef_scalar_deleting_destructor(&obj, 0, 2);
    if (r != &obj || g_dtor != 3 || g_delete != 1) { std::printf("FAIL c\n"); return 1; }
    std::printf("CLandscapeBackgroundPatch_00457c84_TEST PASS\n");
    return 0;
}