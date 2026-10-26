#include <cstdio>

class CSpecialAbilitiesPhysicalShieldDef { public:
    void* vftable;
};

static int g_dtor_calls = 0;
static int g_delete_calls = 0;

extern "C" void __fastcall CSpecialAbilitiesPhysicalShieldDef_scalar_dtor(CSpecialAbilitiesPhysicalShieldDef* def) { (void)def; ++g_dtor_calls; }
extern "C" void __cdecl op_delete(void* p) { (void)p; ++g_delete_calls; }

void* __fastcall CSpecialAbilitiesPhysicalShieldDef_scalar_deleting_destructor(CSpecialAbilitiesPhysicalShieldDef* def, int edx_dummy, unsigned int flags)
{
    (void)edx_dummy;
    CSpecialAbilitiesPhysicalShieldDef_scalar_dtor(def);
    if (flags & 1)
        op_delete(def);
    return def;
}

int main()
{
    CSpecialAbilitiesPhysicalShieldDef obj; obj.vftable = 0;
    g_dtor_calls = 0; g_delete_calls = 0;
    void* r = CSpecialAbilitiesPhysicalShieldDef_scalar_deleting_destructor(&obj, 0, 0);
    if (r != &obj || g_dtor_calls != 1 || g_delete_calls != 0) { std::printf("FAIL no-delete path\n"); return 1; }
    g_dtor_calls = 0; g_delete_calls = 0;
    r = CSpecialAbilitiesPhysicalShieldDef_scalar_deleting_destructor(&obj, 0, 1);
    if (r != &obj || g_dtor_calls != 1 || g_delete_calls != 1) { std::printf("FAIL delete path\n"); return 1; }
    g_dtor_calls = 0; g_delete_calls = 0;
    r = CSpecialAbilitiesPhysicalShieldDef_scalar_deleting_destructor(&obj, 0, 2);
    if (r != &obj || g_dtor_calls != 1 || g_delete_calls != 0) { std::printf("FAIL flag-bit path\n"); return 1; }
    std::printf("CLandscapeBackgroundPatch_0045782f_TEST PASS\n");
    return 0;
}