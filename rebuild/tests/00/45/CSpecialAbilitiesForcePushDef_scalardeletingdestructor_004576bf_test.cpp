#include <cstdio>

class CSpecialAbilitiesForcePushDef { public:
    int f0;
};

static int g_dtor_calls = 0;
static int g_delete_calls = 0;

void __fastcall CSpecialAbilitiesForcePushDef_dtor(CSpecialAbilitiesForcePushDef* def) { (void)def; ++g_dtor_calls; }
void __cdecl op_delete(void* p) { (void)p; ++g_delete_calls; }

void* __fastcall CSpecialAbilitiesForcePushDef_scalar_deleting_destructor(CSpecialAbilitiesForcePushDef* def, int edx, unsigned int flags)
{
    (void)edx;
    CSpecialAbilitiesForcePushDef_dtor(def);
    if (flags & 1)
        op_delete(def);
    return def;
}

int main()
{
    CSpecialAbilitiesForcePushDef obj;
    obj.f0 = 42;

    g_dtor_calls = 0; g_delete_calls = 0;
    void* r1 = CSpecialAbilitiesForcePushDef_scalar_deleting_destructor(&obj, 0, 0);
    if (r1 != &obj || g_dtor_calls != 1 || g_delete_calls != 0) {
        std::printf("FAIL flags=0 dtor=%d del=%d ret=%p\n", g_dtor_calls, g_delete_calls, r1);
        return 1;
    }

    g_dtor_calls = 0; g_delete_calls = 0;
    void* r2 = CSpecialAbilitiesForcePushDef_scalar_deleting_destructor(&obj, 0, 1);
    if (r2 != &obj || g_dtor_calls != 1 || g_delete_calls != 1) {
        std::printf("FAIL flags=1 dtor=%d del=%d ret=%p\n", g_dtor_calls, g_delete_calls, r2);
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_004576bf_TEST PASS\n");
    return 0;
}