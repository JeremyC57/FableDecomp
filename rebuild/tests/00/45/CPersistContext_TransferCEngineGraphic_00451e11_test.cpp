// Behaviour test for 0x00451E11 CPersistContext::Transfer<CEngineGraphic>(CEngineGraphic&) (NF415: linked): links the row object (the
// specialisation is declared here, not defined) and defines its callees under the row's spellings. Checks the CheckCRC key is
// retail's pooled "" (0x0122D70E), then the mode switch (2: load through +0x24, 3: save through +0x28, else nothing).
#include <cstdio>
#include <cstring>

struct LocalObj {
    char pad[16];
    LocalObj();
    void apply24(void* fld, int arg);
    void apply28(void* fld, int arg);
};
extern void __stdcall ProfileEnter(int tag);
class CEngineGraphic;
class CPersistContext { public:
    char pad[0x18];
    int mode;      // +0x18
    char pad2[0x24 - 0x1c];
    void* p24;     // +0x24
    void* p28;     // +0x28
    template <class T> void Transfer(T& value);
};
// The row's specialisation Transfer<CEngineGraphic> is reached through the declared (undefined) member template: the call below names
//   ??$Transfer@VCEngineGraphic@@@CPersistContext@@QAEXAAVCEngineGraphic@@@Z, which the linked row object defines.

static int g_log = 0, g_bad = 0; static const char* g_key = 0; static void* g_fld = 0; static int g_arg = 0;
LocalObj::LocalObj() { g_log += 1000; }
void LocalObj::apply24(void* fld, int arg) { g_log += 24; g_fld = fld; g_arg = arg; }
void LocalObj::apply28(void* fld, int arg) { g_log += 28; g_fld = fld; g_arg = arg; }
void __stdcall ProfileEnter(int tag) { g_log += 1; g_key = (const char*)tag; if (g_key == 0 || std::strcmp(g_key, "") != 0) ++g_bad; }

int main() {
    CPersistContext ctx;
    for (int i = 0; i < (int)sizeof(ctx); i++) ((char*)&ctx)[i] = 0;
    ctx.p24 = (void*)0x24; ctx.p28 = (void*)0x28;
    static int graphic[4];
    CEngineGraphic* value = (CEngineGraphic*)graphic;
    const int want = (int)graphic;
    ctx.mode = 2; g_log = 0;
    ctx.Transfer<CEngineGraphic>(*value);
    if (g_log != 1000 + 1 + 24 || g_fld != (void*)0x24 || g_arg != want) { std::printf("FAIL mode2 %d\n", g_log); return 1; }
    ctx.mode = 3; g_log = 0;
    ctx.Transfer<CEngineGraphic>(*value);
    if (g_log != 1000 + 1 + 28 || g_fld != (void*)0x28 || g_arg != want) { std::printf("FAIL mode3 %d\n", g_log); return 1; }
    ctx.mode = 5; g_log = 0;
    ctx.Transfer<CEngineGraphic>(*value);
    if (g_log != 1000 + 1) { std::printf("FAIL modeDefault %d\n", g_log); return 1; }
    if (g_bad) { std::printf("FAIL key is not the empty string\n"); return 1; }
    std::printf("CEngineLightingManager_00451e11_TEST PASS\n");
    return 0;
}
