#include <cstdio>
struct CMaterialDef { int f0; };
static int g_dtor_called = 0;
static int g_delete_called = 0;
void __fastcall CMaterialDef_dtor(CMaterialDef* def){ g_dtor_called++; (void)def; }
void __cdecl engine_operator_delete(void* p){ g_delete_called++; (void)p; }

void* __fastcall CMaterialDef_scalar_deleting_destructor(CMaterialDef* def, int edx_dummy, unsigned int flags)
{
    (void)edx_dummy;
    CMaterialDef_dtor(def);
    if (flags & 1)
        engine_operator_delete(def);
    return def;
}

int main(){
    CMaterialDef obj; obj.f0 = 7;
    g_dtor_called = 0; g_delete_called = 0;
    void* r = CMaterialDef_scalar_deleting_destructor(&obj, 0, 0);
    if (r != &obj) { std::printf("FAIL ret\n"); return 1; }
    if (g_dtor_called != 1) { std::printf("FAIL dtor\n"); return 1; }
    if (g_delete_called != 0) { std::printf("FAIL nodelete\n"); return 1; }
    g_dtor_called = 0; g_delete_called = 0;
    r = CMaterialDef_scalar_deleting_destructor(&obj, 0, 1);
    if (r != &obj) { std::printf("FAIL ret2\n"); return 1; }
    if (g_dtor_called != 1) { std::printf("FAIL dtor2\n"); return 1; }
    if (g_delete_called != 1) { std::printf("FAIL delete\n"); return 1; }
    std::printf("CLandscapeBackgroundPatch_00455d28_TEST PASS\n");
    return 0;
}