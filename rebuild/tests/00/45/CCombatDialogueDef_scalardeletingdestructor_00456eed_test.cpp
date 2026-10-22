#include <cstdio>

struct CCombatDialogueDef { int dummy; };

static int g_dtor_calls = 0;
static int g_delete_calls = 0;

void __fastcall CCombatDialogueDef_dtor(CCombatDialogueDef* def) { (void)def; g_dtor_calls++; }
void __cdecl op_delete(void* p) { (void)p; g_delete_calls++; }

void* __fastcall CCombatDialogueDef_scalar_deleting_destructor(CCombatDialogueDef* def, int /*edx*/, unsigned int flags)
{
    CCombatDialogueDef_dtor(def);
    if (flags & 1)
        op_delete((void*)def);
    return def;
}

int main()
{
    CCombatDialogueDef obj; obj.dummy = 42;

    // flags without bit 0: destruct only, no delete
    g_dtor_calls = 0; g_delete_calls = 0;
    void* r0 = CCombatDialogueDef_scalar_deleting_destructor(&obj, 0, 0);
    if (r0 != &obj || g_dtor_calls != 1 || g_delete_calls != 0) { std::printf("FAIL branch0\n"); return 1; }

    // flags with bit 0: destruct + delete
    g_dtor_calls = 0; g_delete_calls = 0;
    void* r1 = CCombatDialogueDef_scalar_deleting_destructor(&obj, 0, 1);
    if (r1 != &obj || g_dtor_calls != 1 || g_delete_calls != 1) { std::printf("FAIL branch1\n"); return 1; }

    // even flag value (bit0 clear) => no delete
    g_dtor_calls = 0; g_delete_calls = 0;
    void* r2 = CCombatDialogueDef_scalar_deleting_destructor(&obj, 0, 2);
    if (r2 != &obj || g_delete_calls != 0) { std::printf("FAIL branch2\n"); return 1; }

    std::printf("CLandscapeBackgroundPatch_00456eed_TEST PASS\n");
    return 0;
}