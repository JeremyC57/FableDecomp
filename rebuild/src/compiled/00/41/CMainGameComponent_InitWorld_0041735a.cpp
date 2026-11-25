// Leaf: ?InitWorld@CMainGameComponent@@IAEXXZ
// Literal (NF415, 2026-11-25): was the bare integer 0x122eea4 (the retail VA of "Init World Init"); now the literal is spelled, cast to the
//   stand-in's existing int parameter so no callee or table line changes (the callee port is T3). Retail 0x0122EEA4: 49 6E 69 74 20 57 6F 72 6C 64 20 49 6E 69 74 00.
// Name fix (agentNF26, 2026-11-12, FAM09 follow-up): 0x013B8660 is GEventPackageFileName (CWideString; Ego_r.pdb + FableWin.pdb
//   `dynamic initializer for 'GEventPackageFileName'', initializer 0x01219F80 landed by agentFAM09; Anniv360 ?GEventPackageFileName@@3VCWideString@@A),
//   read through CWideString::operator const wchar_t*() const 0x0099AF00 (fable_string.h; the ??BCWideString@@QBEPBGXZ spelling the landed
//   0x0099CF10 binds) into the protected InitEventPackageLoading / InitEventPackageSaving(const wchar_t*) (Ego_r.pdb
//   ?InitEventPackageLoading@CMainGameComponent@@IAE_NPB_W@Z; retail builds without a native wchar_t). Was g_obj (a GObj with GetPtr).
#include "fable_string.h"
class CGameDefinitionManager;
struct WorldCtx;
class CGameDefinitionManager { public: static CGameDefinitionManager& __fastcall Get(); };   // 0x0044C6B0 ?Get@CGameDefinitionManager@@SIAAV1@XZ
struct WorldObj {
    virtual void v0();virtual void v1();virtual void v2();virtual void v3();virtual void v4();
    virtual void v5();virtual void v6();virtual void v7();virtual void v8();virtual void vtbl9();
    WorldObj* Construct(WorldCtx* ctx);
};
struct WorldCtx { void* comp; int z4; void* f1c; CGameDefinitionManager* defmgr; int z10; };
struct CCharString2 { char buf[4]; CCharString2(int lit, int n); ~CCharString2(); };
struct CountedPtr { WorldObj* ptr; void DeleteData(WorldObj* w); };
extern "C" void __fastcall DisplayProgress(CCharString2* text, bool edxBool, float progress, int flags);
extern CWideString GEventPackageFileName;   // 0x013B8660
extern unsigned char g_flag1, g_flag2;
class CMainGameComponent { public:
    char pad0[0x1c]; void* f1c; char pad20[0x24-0x20]; CountedPtr f24;
protected:
    bool InitEventPackageLoading(const wchar_t* fileName);   // 0x00416056
    bool InitEventPackageSaving(const wchar_t* fileName);    // 0x004160CC
public:
    protected:
    void InitWorld();
    public:
};
void CMainGameComponent::InitWorld()
{
    WorldCtx ctx;
    ctx.z4 = 0; ctx.defmgr = 0; ctx.z10 = 0;
    ctx.comp = this;
    ctx.f1c = this->f1c;
    ctx.defmgr = &CGameDefinitionManager::Get();
    WorldObj* w = (WorldObj*)::operator new(0x198);
    if (w) w = w->Construct(&ctx);
    else w = 0;
    this->f24.DeleteData(w);
    if (g_flag1) this->InitEventPackageLoading(GEventPackageFileName);
    if (g_flag2) this->InitEventPackageSaving(GEventPackageFileName);
    {
        CCharString2 s((int)"Init World Init", -1);
        // NF239 (2026-11-22, 0x0122DEE0 census): retail pushes the -1.0f literal at 0x0122DEE0 here (Fable.exe: 00 00 80 BF); the row
        //   had 0.5f (__real@3f000000), which graded only because relocation fields are masked.
        DisplayProgress(&s, false, -1.0f, 0);
    }
    this->f24.ptr->vtbl9();
}
