#include <cstdio>
#include <cstdlib>
struct CHeroStatsDef { int f0; };
static int g_dtor_called = 0;
static int g_delete_called = 0;
static void* g_last_deleted = 0;
static void __fastcall CHeroStatsDef_dtor(CHeroStatsDef* def) { (void)def; g_dtor_called++; }

static void* __fastcall CHeroStatsDef_scalar_deleting_destructor(CHeroStatsDef* def, int edx_ignored, unsigned int flags)
{
    (void)edx_ignored;
    CHeroStatsDef_dtor(def);
    if (flags & 1) { g_delete_called++; g_last_deleted = (void*)def; }
    return def;
}

int main()
{
    CHeroStatsDef obj; obj.f0 = 42;
    g_dtor_called = 0; g_delete_called = 0;
    void* r = CHeroStatsDef_scalar_deleting_destructor(&obj, 0, 0);
    if (r != &obj || g_dtor_called != 1 || g_delete_called != 0) { std::printf("FAIL noflag\n"); return 1; }
    g_dtor_called = 0; g_delete_called = 0;
    r = CHeroStatsDef_scalar_deleting_destructor(&obj, 0, 1);
    if (r != &obj || g_dtor_called != 1 || g_delete_called != 1 || g_last_deleted != &obj) { std::printf("FAIL flag\n"); return 1; }
    std::printf("CLandscapeBackgroundPatch_00456182_TEST PASS\n");
    return 0;
}