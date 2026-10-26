#include <cstdio>

class CSpecialAbilitiesBerserkDef { public: int vt; };

static int g_dtor_called = 0;
static int g_delete_called = 0;

void __fastcall CSpecialAbilitiesBerserkDef_dtor(CSpecialAbilitiesBerserkDef* def)
{
    (void)def;
    g_dtor_called++;
}
void __cdecl op_delete(void* p)
{
    (void)p;
    g_delete_called++;
}

void* __fastcall CSpecialAbilitiesBerserkDef_scalar_deleting_destructor(CSpecialAbilitiesBerserkDef* def, int edx, unsigned int flags)
{
    (void)edx;
    CSpecialAbilitiesBerserkDef_dtor(def);
    if (flags & 1)
        op_delete(def);
    return def;
}

int main()
{
    CSpecialAbilitiesBerserkDef obj; obj.vt = 0;

    g_dtor_called = 0; g_delete_called = 0;
    void* r = CSpecialAbilitiesBerserkDef_scalar_deleting_destructor(&obj, 0, 0);
    if (r != &obj || g_dtor_called != 1 || g_delete_called != 0) {
        std::printf("FAIL no-delete path\n"); return 1;
    }

    g_dtor_called = 0; g_delete_called = 0;
    r = CSpecialAbilitiesBerserkDef_scalar_deleting_destructor(&obj, 0, 1);
    if (r != &obj || g_dtor_called != 1 || g_delete_called != 1) {
        std::printf("FAIL delete path\n"); return 1;
    }

    g_dtor_called = 0; g_delete_called = 0;
    r = CSpecialAbilitiesBerserkDef_scalar_deleting_destructor(&obj, 0, 2);
    if (r != &obj || g_delete_called != 0) {
        std::printf("FAIL flag2 path\n"); return 1;
    }

    std::printf("CLandscapeBackgroundPatch_004573e1_TEST PASS\n");
    return 0;
}