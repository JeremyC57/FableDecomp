#include <cstdio>

static int g_dtor_called = 0;
static int g_free_called = 0;
static void* g_last_self = 0;

 void operator delete(void* p) { g_free_called++; g_last_self = p; }
void __fastcall CSpecialAbilitiesBattleChargeDef_dtor(void* def) { g_dtor_called++; g_last_self = def; }

void* __fastcall CSpecialAbilitiesBattleChargeDef_scalar_deleting_destructor(void* def, int edx, unsigned int flags)
{
    CSpecialAbilitiesBattleChargeDef_dtor(def);
    if (flags & 1)
        operator delete(def);
    return def;
}

int main()
{
    int dummy = 0;
    void* def = &dummy;

    g_dtor_called = 0; g_free_called = 0;
    void* r1 = CSpecialAbilitiesBattleChargeDef_scalar_deleting_destructor(def, 0, 0);
    if (r1 != def || g_dtor_called != 1 || g_free_called != 0) {
        std::printf("FAIL flags0 r=%p dtor=%d free=%d\n", r1, g_dtor_called, g_free_called);
        return 1;
    }

    g_dtor_called = 0; g_free_called = 0;
    void* r2 = CSpecialAbilitiesBattleChargeDef_scalar_deleting_destructor(def, 0, 1);
    if (r2 != def || g_dtor_called != 1 || g_free_called != 1) {
        std::printf("FAIL flags1 r=%p dtor=%d free=%d\n", r2, g_dtor_called, g_free_called);
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00457116_TEST PASS\n");
    return 0;
}