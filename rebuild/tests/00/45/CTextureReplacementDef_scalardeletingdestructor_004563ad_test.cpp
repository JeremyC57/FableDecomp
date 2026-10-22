#include <cstdio>

static int g_dtor_called = 0;
static int g_delete_called = 0;
static void* g_last = 0;

void __fastcall CTextureReplacementDef_dtor(void* def) { g_dtor_called++; g_last = def; }
void __cdecl op_delete(void* p) { g_delete_called++; (void)p; }

void* __fastcall CTextureReplacementDef_scalar_deleting_destructor(void* def, int edx, unsigned int flags)
{
    (void)edx;
    CTextureReplacementDef_dtor(def);
    if (flags & 1)
        op_delete(def);
    return def;
}

int main()
{
    int obj = 0;
    void* p = &obj;

    g_dtor_called = g_delete_called = 0;
    void* r = CTextureReplacementDef_scalar_deleting_destructor(p, 0, 0);
    if (r != p || g_dtor_called != 1 || g_delete_called != 0) { std::printf("FAIL noflag\n"); return 1; }

    g_dtor_called = g_delete_called = 0;
    r = CTextureReplacementDef_scalar_deleting_destructor(p, 0, 1);
    if (r != p || g_dtor_called != 1 || g_delete_called != 1) { std::printf("FAIL flag\n"); return 1; }

    g_dtor_called = g_delete_called = 0;
    r = CTextureReplacementDef_scalar_deleting_destructor(p, 0, 2);
    if (r != p || g_dtor_called != 1 || g_delete_called != 0) { std::printf("FAIL flag2\n"); return 1; }

    std::printf("CLandscapeBackgroundPatch_004563ad_TEST PASS\n");
    return 0;
}