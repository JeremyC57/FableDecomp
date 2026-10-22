#include <cstdio>

static int g_dtor_called = 0;
static int g_delete_called = 0;
static void* g_vft = (void*)0x1230ba0;

struct CPhysicsDef {
    void* vftable;
};

static void CPhysicsDef_dtor(void* def) { g_dtor_called++; (void)def; }
static void op_delete(void* p) { g_delete_called++; (void)p; }

void* CPhysicsDef_scalar_deleting_destructor(CPhysicsDef* def, int edxpad, unsigned int flags)
{
    (void)edxpad;
    def->vftable = g_vft;
    CPhysicsDef_dtor(def);
    if (flags & 1) {
        op_delete(def);
    }
    return def;
}

int main() {
    CPhysicsDef obj;
    obj.vftable = 0;

    g_dtor_called = 0; g_delete_called = 0;
    void* r = CPhysicsDef_scalar_deleting_destructor(&obj, 0, 0);
    if (r != &obj) { std::printf("FAIL ret\n"); return 1; }
    if (obj.vftable != g_vft) { std::printf("FAIL vft\n"); return 1; }
    if (g_dtor_called != 1) { std::printf("FAIL dtor0\n"); return 1; }
    if (g_delete_called != 0) { std::printf("FAIL del0\n"); return 1; }

    g_dtor_called = 0; g_delete_called = 0;
    r = CPhysicsDef_scalar_deleting_destructor(&obj, 0, 1);
    if (r != &obj) { std::printf("FAIL ret2\n"); return 1; }
    if (g_dtor_called != 1) { std::printf("FAIL dtor1\n"); return 1; }
    if (g_delete_called != 1) { std::printf("FAIL del1\n"); return 1; }

    g_delete_called = 0;
    CPhysicsDef_scalar_deleting_destructor(&obj, 0, 2);
    if (g_delete_called != 0) { std::printf("FAIL del2\n"); return 1; }

    std::printf("CLandscapeBackgroundPatch_0044f6ba_TEST PASS\n");
    return 0;
}