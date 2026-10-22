#include <cstdio>
struct CHairCardDef {
    void* vptr;
};
static int g_dtor_called = 0;
static int g_delete_called = 0;
static void* g_vft = (void*)0x1230ba0;
void CHairCardDef_dtor(CHairCardDef* def) { (void)def; g_dtor_called++; }
void engine_delete(void* p) { (void)p; g_delete_called++; }

void* CHairCardDef_scalar_deleting_destructor(CHairCardDef* def, unsigned int flags)
{
    def->vptr = g_vft;
    CHairCardDef_dtor(def);
    if (flags & 1) {
        engine_delete(def);
    }
    return def;
}

int main() {
    CHairCardDef a; a.vptr = 0;
    void* r = CHairCardDef_scalar_deleting_destructor(&a, 0u);
    if (r != &a) { std::printf("FAIL return\n"); return 1; }
    if (g_dtor_called != 1) { std::printf("FAIL dtor\n"); return 1; }
    if (g_delete_called != 0) { std::printf("FAIL delete0\n"); return 1; }
    if (a.vptr != g_vft) { std::printf("FAIL vptr\n"); return 1; }
    CHairCardDef b; b.vptr = 0;
    r = CHairCardDef_scalar_deleting_destructor(&b, 1u);
    if (r != &b) { std::printf("FAIL return2\n"); return 1; }
    if (g_dtor_called != 2) { std::printf("FAIL dtor2\n"); return 1; }
    if (g_delete_called != 1) { std::printf("FAIL delete1\n"); return 1; }
    std::printf("CLandscapeBackgroundPatch_0044faba_TEST PASS\n");
    return 0;
}