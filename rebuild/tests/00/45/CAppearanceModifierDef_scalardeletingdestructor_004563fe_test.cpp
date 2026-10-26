#include <cstdio>

static int g_dtor_calls = 0;
static int g_delete_calls = 0;
static void* g_deleted = 0;

class CAppearanceModifierDef { public:
    int f0;
    int Parent;
    void* f8;
    void CAppearanceModifierDef_dtor();
    void* CAppearanceModifierDef_scalar_deleting_destructor(unsigned int flags);
};

void __cdecl eng_operator_delete(void* p) {
    ++g_delete_calls;
    g_deleted = p;
}

void CAppearanceModifierDef::CAppearanceModifierDef_dtor() {
    ++g_dtor_calls;
}

void* CAppearanceModifierDef::CAppearanceModifierDef_scalar_deleting_destructor(unsigned int flags)
{
    this->CAppearanceModifierDef_dtor();
    if (flags & 1) {
        eng_operator_delete(this);
    }
    return this;
}

int main() {
    CAppearanceModifierDef obj;
    g_dtor_calls = g_delete_calls = 0; g_deleted = 0;
    void* r1 = obj.CAppearanceModifierDef_scalar_deleting_destructor(0);
    if (r1 != &obj || g_dtor_calls != 1 || g_delete_calls != 0) {
        std::printf("FAIL case0 r=%p dtor=%d del=%d\n", r1, g_dtor_calls, g_delete_calls);
        return 1;
    }
    g_dtor_calls = g_delete_calls = 0; g_deleted = 0;
    void* r2 = obj.CAppearanceModifierDef_scalar_deleting_destructor(1);
    if (r2 != &obj || g_dtor_calls != 1 || g_delete_calls != 1 || g_deleted != &obj) {
        std::printf("FAIL case1 r=%p dtor=%d del=%d\n", r2, g_dtor_calls, g_delete_calls);
        return 1;
    }
    g_dtor_calls = g_delete_calls = 0; g_deleted = 0;
    void* r3 = obj.CAppearanceModifierDef_scalar_deleting_destructor(2);
    if (r3 != &obj || g_delete_calls != 0) {
        std::printf("FAIL case2 del=%d\n", g_delete_calls);
        return 1;
    }
    std::printf("CLandscapeBackgroundPatch_004563fe_TEST PASS\n");
    return 0;
}