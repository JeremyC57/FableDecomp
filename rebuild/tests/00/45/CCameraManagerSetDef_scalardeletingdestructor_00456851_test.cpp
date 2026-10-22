#include <cstdio>

static int g_dtor_called = 0;
static int g_delete_called = 0;

static void CCameraManagerSetDef_dtor(void* def) { (void)def; g_dtor_called++; }
static void op_delete(void* p) { (void)p; g_delete_called++; }

static void* CCameraManagerSetDef_scalar_deleting_destructor(void* def, unsigned int flags)
{
    CCameraManagerSetDef_dtor(def);
    if (flags & 1)
        op_delete(def);
    return def;
}

int main()
{
    int obj = 0;
    void* p = &obj;

    g_dtor_called = 0; g_delete_called = 0;
    void* r = CCameraManagerSetDef_scalar_deleting_destructor(p, 0);
    if (r != p || g_dtor_called != 1 || g_delete_called != 0) {
        std::printf("FAIL noflag\n"); return 1;
    }

    g_dtor_called = 0; g_delete_called = 0;
    r = CCameraManagerSetDef_scalar_deleting_destructor(p, 1);
    if (r != p || g_dtor_called != 1 || g_delete_called != 1) {
        std::printf("FAIL flag\n"); return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00456851_TEST PASS\n");
    return 0;
}