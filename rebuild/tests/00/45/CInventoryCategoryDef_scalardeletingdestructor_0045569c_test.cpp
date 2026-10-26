#include <cstdio>
class CInventoryCategoryDef { public: int f0; };
static int g_dtor_called = 0;
static int g_delete_called = 0;
void __fastcall CInventoryCategoryDef_dtor(CInventoryCategoryDef* def) { g_dtor_called++; }
void __cdecl operator_delete_impl(void* p) { g_delete_called++; }

void* __fastcall CInventoryCategoryDef_scalar_deleting_destructor(CInventoryCategoryDef* def, int, unsigned int flags)
{
    CInventoryCategoryDef_dtor(def);
    if (flags & 1)
        operator_delete_impl(def);
    return def;
}

int main() {
    CInventoryCategoryDef obj; obj.f0 = 7;
    g_dtor_called = 0; g_delete_called = 0;
    void* r1 = CInventoryCategoryDef_scalar_deleting_destructor(&obj, 0, 0);
    if (r1 != &obj || g_dtor_called != 1 || g_delete_called != 0) { std::printf("FAIL no-delete path\n"); return 1; }
    g_dtor_called = 0; g_delete_called = 0;
    void* r2 = CInventoryCategoryDef_scalar_deleting_destructor(&obj, 0, 1);
    if (r2 != &obj || g_dtor_called != 1 || g_delete_called != 1) { std::printf("FAIL delete path\n"); return 1; }
    std::printf("CLandscapeBackgroundPatch_0045569c_TEST PASS\n");
    return 0;
}