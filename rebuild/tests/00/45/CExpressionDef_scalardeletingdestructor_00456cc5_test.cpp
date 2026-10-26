#include <cstdio>

class CExpressionDef { public: int vt; };

static int g_dtor_calls = 0;
static int g_delete_calls = 0;

void __fastcall CExpressionDef_dtor(CExpressionDef* def) { g_dtor_calls++; }
void __cdecl fable_operator_delete(void* p) { g_delete_calls++; }

void* __fastcall CExpressionDef_scalar_deleting_destructor(CExpressionDef* def, int edx_unused, unsigned int flags) {
    CExpressionDef_dtor(def);
    if (flags & 1) {
        fable_operator_delete(def);
    }
    return def;
}

int main() {
    CExpressionDef obj; obj.vt = 7;
    g_dtor_calls = 0; g_delete_calls = 0;
    void* r = CExpressionDef_scalar_deleting_destructor(&obj, 0, 0);
    if (r != &obj || g_dtor_calls != 1 || g_delete_calls != 0) { std::printf("FAIL case0\n"); return 1; }
    g_dtor_calls = 0; g_delete_calls = 0;
    r = CExpressionDef_scalar_deleting_destructor(&obj, 0, 1);
    if (r != &obj || g_dtor_calls != 1 || g_delete_calls != 1) { std::printf("FAIL case1\n"); return 1; }
    g_dtor_calls = 0; g_delete_calls = 0;
    r = CExpressionDef_scalar_deleting_destructor(&obj, 0, 2);
    if (r != &obj || g_dtor_calls != 1 || g_delete_calls != 0) { std::printf("FAIL case2\n"); return 1; }
    std::printf("CLandscapeBackgroundPatch_00456cc5_TEST PASS\n");
    return 0;
}