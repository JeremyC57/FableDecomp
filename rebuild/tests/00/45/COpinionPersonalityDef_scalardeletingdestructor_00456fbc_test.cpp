#include <cstdio>
struct COpinionPersonalityDef { int f0; };
static int g_dtor_called = 0;
static int g_delete_called = 0;
static void dtor(COpinionPersonalityDef* def){ (void)def; g_dtor_called++; }
static void op_delete(void* p){ (void)p; g_delete_called++; }

static void* vdd(COpinionPersonalityDef* def, unsigned int flags){
    dtor(def);
    if (flags & 1) { op_delete((void*)def); }
    return (void*)def;
}

int main(){
    COpinionPersonalityDef obj; obj.f0 = 7;
    g_dtor_called = 0; g_delete_called = 0;
    void* r = vdd(&obj, 0);
    if (r != &obj || g_dtor_called != 1 || g_delete_called != 0) { std::printf("FAIL noflag\n"); return 1; }
    g_dtor_called = 0; g_delete_called = 0;
    r = vdd(&obj, 1);
    if (r != &obj || g_dtor_called != 1 || g_delete_called != 1) { std::printf("FAIL flag\n"); return 1; }
    std::printf("CLandscapeBackgroundPatch_00456fbc_TEST PASS\n");
    return 0;
}