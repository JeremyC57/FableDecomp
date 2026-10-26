#include <cstdio>

class CSkyDef { public: int x; };

static int g_subDtor = 0;
static int g_dtor = 0;
static int g_deleted = 0;

void __fastcall Sub_Dtor_00453327(void* p) { (void)p; g_subDtor++; }
void __fastcall CSkyDef_Dtor(CSkyDef* def) { (void)def; g_dtor++; }
void __cdecl operator_delete(void* p) { (void)p; g_deleted++; }

void* __fastcall CSkyDef_scalar_deleting_destructor(CSkyDef* def, int edx, unsigned int flags)
{
    (void)edx;
    Sub_Dtor_00453327((char*)def + 0x74);
    CSkyDef_Dtor(def);
    if (flags & 1)
        operator_delete(def);
    return def;
}

int main()
{
    CSkyDef obj;
    void* r = CSkyDef_scalar_deleting_destructor(&obj, 0, 0);
    if (r != &obj) { std::printf("FAIL: return\n"); return 1; }
    if (g_subDtor != 1 || g_dtor != 1 || g_deleted != 0) { std::printf("FAIL: no-delete path\n"); return 1; }
    r = CSkyDef_scalar_deleting_destructor(&obj, 0, 1);
    if (g_deleted != 1) { std::printf("FAIL: delete path\n"); return 1; }
    std::printf("CEngineSubPrimitiveText_00455899_TEST PASS\n");
    return 0;
}