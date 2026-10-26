#include <cstdio>

class CSpecialAbilitiesDrainLifeDef { public: int marker; };

static int g_dtor_calls = 0;
static int g_delete_calls = 0;

void __fastcall CSpecialAbilitiesDrainLifeDef_dtor(CSpecialAbilitiesDrainLifeDef* def)
{
    (void)def;
    ++g_dtor_calls;
}
void __cdecl fable_operator_delete(void* p)
{
    (void)p;
    ++g_delete_calls;
}

void* __fastcall CSpecialAbilitiesDrainLifeDef_scalar_deleting_destructor(CSpecialAbilitiesDrainLifeDef* def, int edx, unsigned int flags)
{
    (void)edx;
    CSpecialAbilitiesDrainLifeDef_dtor(def);
    if (flags & 1)
        fable_operator_delete(def);
    return def;
}

int main()
{
    CSpecialAbilitiesDrainLifeDef obj; obj.marker = 42;

    g_dtor_calls = 0; g_delete_calls = 0;
    void* r1 = CSpecialAbilitiesDrainLifeDef_scalar_deleting_destructor(&obj, 0, 0);
    if (r1 != &obj) { std::printf("FAIL: return ptr mismatch flags=0\n"); return 1; }
    if (g_dtor_calls != 1) { std::printf("FAIL: dtor not called flags=0\n"); return 1; }
    if (g_delete_calls != 0) { std::printf("FAIL: delete called with flags=0\n"); return 1; }

    g_dtor_calls = 0; g_delete_calls = 0;
    void* r2 = CSpecialAbilitiesDrainLifeDef_scalar_deleting_destructor(&obj, 0, 1);
    if (r2 != &obj) { std::printf("FAIL: return ptr mismatch flags=1\n"); return 1; }
    if (g_dtor_calls != 1) { std::printf("FAIL: dtor not called flags=1\n"); return 1; }
    if (g_delete_calls != 1) { std::printf("FAIL: delete not called flags=1\n"); return 1; }

    g_dtor_calls = 0; g_delete_calls = 0;
    CSpecialAbilitiesDrainLifeDef_scalar_deleting_destructor(&obj, 0, 2);
    if (g_delete_calls != 0) { std::printf("FAIL: delete called when bit0 clear (flags=2)\n"); return 1; }

    std::printf("CLandscapeBackgroundPatch_0045791a_TEST PASS\n");
    return 0;
}