#include <cstdio>
struct CInventoryDef { int x; };
static int g_dtor_called = 0;
static int g_delete_called = 0;
static void __fastcall CInventoryDef_dtor(CInventoryDef* def) { (void)def; g_dtor_called++; }
static void __cdecl op_delete(void* p) { (void)p; g_delete_called++; }

void* __fastcall CInventoryDef_scalar_deleting_destructor(CInventoryDef* def, int, unsigned int flags)
{
    CInventoryDef_dtor(def);
    if (flags & 1)
        op_delete((void*)def);
    return def;
}

int main()
{
    CInventoryDef obj;
    obj.x = 7;
    void* r1 = CInventoryDef_scalar_deleting_destructor(&obj, 0, 0);
    if (r1 != &obj) { std::printf("FAIL ret\n"); return 1; }
    if (g_dtor_called != 1) { std::printf("FAIL dtor0\n"); return 1; }
    if (g_delete_called != 0) { std::printf("FAIL del0\n"); return 1; }
    void* r2 = CInventoryDef_scalar_deleting_destructor(&obj, 0, 1);
    if (r2 != &obj) { std::printf("FAIL ret2\n"); return 1; }
    if (g_dtor_called != 2) { std::printf("FAIL dtor1\n"); return 1; }
    if (g_delete_called != 1) { std::printf("FAIL del1\n"); return 1; }
    std::printf("CLandscapeBackgroundPatch_004554aa_TEST PASS\n");
    return 0;
}