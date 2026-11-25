// Behaviour test for 0x0041735A CMainGameComponent::InitWorld() (NF415: linked): links the row object (InitWorld is declared here, not
// defined) and defines its callees under the row's spellings. Checks the world construction (context, 0x198-byte allocation, counted
// pointer reset), the two event-package flags, the progress text "Init World Init" (retail 0x0122EEA4) with length -1 and progress
// -1.0f, the text destroyed after the call, and the final virtual slot 9 on the new world.
#include <stdio.h>
#include <string.h>

class CWideString { public:                             // fable_string.h's out-of-line conversion, the spelling the row calls
    void* PStringData;
    operator const unsigned short*() const;             // ??BCWideString@@QBEPBGXZ
};
class CGameDefinitionManager { public: static CGameDefinitionManager& __fastcall Get(); };
struct WorldCtx;
struct WorldObj {
    virtual void v0();virtual void v1();virtual void v2();virtual void v3();virtual void v4();
    virtual void v5();virtual void v6();virtual void v7();virtual void v8();virtual void vtbl9();
    WorldObj* Construct(WorldCtx* ctx);
};
struct WorldCtx { void* comp; int z4; void* f1c; CGameDefinitionManager* defmgr; int z10; };
struct CCharString2 { char buf[4]; CCharString2(int lit, int n); ~CCharString2(); };
struct CountedPtr { WorldObj* ptr; void DeleteData(WorldObj* w); };
extern "C" void __fastcall DisplayProgress(CCharString2* text, bool edxBool, float progress, int flags);
extern CWideString GEventPackageFileName;
extern unsigned char g_flag1, g_flag2;
class CMainGameComponent { public:
    char pad0[0x1c]; void* f1c; char pad20[0x24-0x20]; CountedPtr f24;
protected:
    bool InitEventPackageLoading(const unsigned short* fileName);
    bool InitEventPackageSaving(const unsigned short* fileName);
    void InitWorld();                                   // the row (linked)
};
class CTestComponent : public CMainGameComponent { public: void Run() { InitWorld(); } };

static int g_get, g_construct, g_del, g_load, g_save, g_ctor, g_dtor, g_prog, g_vtbl, g_bad, g_order;
static const unsigned short g_wadName[] = { 'e', 'v', 0 };
CWideString GEventPackageFileName;
unsigned char g_flag1, g_flag2;
static CGameDefinitionManager g_defs;
static WorldObj g_world;
static WorldCtx g_seenCtx;
static CTestComponent* g_comp;
CWideString::operator const unsigned short*() const { return this == &GEventPackageFileName ? g_wadName : 0; }
CGameDefinitionManager& __fastcall CGameDefinitionManager::Get() { ++g_get; return g_defs; }
void WorldObj::v0() {} void WorldObj::v1() {} void WorldObj::v2() {} void WorldObj::v3() {} void WorldObj::v4() {}
void WorldObj::v5() {} void WorldObj::v6() {} void WorldObj::v7() {} void WorldObj::v8() {}
void WorldObj::vtbl9() { ++g_vtbl; if (this != &g_world || g_dtor != 1) ++g_bad; }
WorldObj* WorldObj::Construct(WorldCtx* ctx) { ++g_construct; g_seenCtx = *ctx; return &g_world; }
void CountedPtr::DeleteData(WorldObj* w) { ++g_del; ptr = w; }
bool CMainGameComponent::InitEventPackageLoading(const unsigned short* f) { ++g_load; if (f != g_wadName) ++g_bad; return true; }
bool CMainGameComponent::InitEventPackageSaving(const unsigned short* f) { ++g_save; if (f != g_wadName) ++g_bad; return true; }
static const char* g_text; static int g_len;
CCharString2::CCharString2(int lit, int n) { ++g_ctor; g_text = (const char*)lit; g_len = n; }
CCharString2::~CCharString2() { ++g_dtor; if (g_prog != 1) ++g_bad; }
extern "C" void __fastcall DisplayProgress(CCharString2* text, bool edxBool, float progress, int flags)
{
    ++g_prog;
    if (text == 0 || g_ctor != 1 || g_dtor != 0 || edxBool || progress != -1.0f || flags != 0) ++g_bad;
}

int main()
{
    static CTestComponent comp; comp.f1c = (void*)0x11; comp.f24.ptr = 0; g_comp = &comp;
    g_flag1 = 1; g_flag2 = 1;
    comp.Run();
    int ok = g_get == 1 && g_construct == 1 && g_del == 1 && g_load == 1 && g_save == 1 && g_ctor == 1 && g_dtor == 1 && g_prog == 1
          && g_vtbl == 1 && comp.f24.ptr == &g_world && g_bad == 0;
    ok = ok && g_seenCtx.comp == (void*)&comp && g_seenCtx.f1c == (void*)0x11 && g_seenCtx.defmgr == &g_defs && g_seenCtx.z4 == 0 && g_seenCtx.z10 == 0;
    ok = ok && g_text != 0 && strcmp(g_text, "Init World Init") == 0 && g_len == -1;   // retail 0x0122EEA4
    g_flag1 = 0; g_flag2 = 0; g_ctor = g_dtor = g_prog = g_vtbl = 0; g_text = 0;
    comp.Run();                                         /* flags clear: no event-package calls */
    ok = ok && g_load == 1 && g_save == 1 && g_prog == 1 && g_vtbl == 1 && g_dtor == 1 && g_bad == 0 && g_text && strcmp(g_text, "Init World Init") == 0;
    printf(ok ? "INITWORLD_OK\n" : "FAIL\n");
    return ok ? 0 : 1;
}
