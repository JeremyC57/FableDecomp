#include <cstdio>

struct CSpecialAbilitiesBulletTimeDef { int f0; };
static int g_dtor_called = 0;
static int g_delete_called = 0;
static void* g_deleted = 0;

void __fastcall CSpecialAbilitiesBulletTimeDef_dtor(CSpecialAbilitiesBulletTimeDef* def) { (void)def; g_dtor_called++; }
void __cdecl eng_operator_delete(void* p) { g_delete_called++; g_deleted = p; }

void* __fastcall CSpecialAbilitiesBulletTimeDef_scalar_deleting_destructor(CSpecialAbilitiesBulletTimeDef* def, int edx, unsigned int flags) {
    (void)edx;
    CSpecialAbilitiesBulletTimeDef_dtor(def);
    if (flags & 1) {
        eng_operator_delete(def);
    }
    return def;
}

int main() {
    CSpecialAbilitiesBulletTimeDef obj; obj.f0 = 7;
    // flags without bit0: dtor only, no delete
    g_dtor_called = g_delete_called = 0; g_deleted = 0;
    void* r1 = CSpecialAbilitiesBulletTimeDef_scalar_deleting_destructor(&obj, 0, 0);
    if (r1 != &obj || g_dtor_called != 1 || g_delete_called != 0) { std::printf("FAIL case0\n"); return 1; }
    // flags with bit0: dtor + delete
    g_dtor_called = g_delete_called = 0; g_deleted = 0;
    void* r2 = CSpecialAbilitiesBulletTimeDef_scalar_deleting_destructor(&obj, 0, 1);
    if (r2 != &obj || g_dtor_called != 1 || g_delete_called != 1 || g_deleted != &obj) { std::printf("FAIL case1\n"); return 1; }
    // even flag bits without bit0 -> no delete
    g_dtor_called = g_delete_called = 0;
    void* r3 = CSpecialAbilitiesBulletTimeDef_scalar_deleting_destructor(&obj, 0, 2);
    if (r3 != &obj || g_delete_called != 0) { std::printf("FAIL case2\n"); return 1; }
    std::printf("CLandscapeBackgroundPatch_004575e0_TEST PASS\n");
    return 0;
}