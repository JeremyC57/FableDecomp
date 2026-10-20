#include <cstdio>
#include <cstdlib>
#include <cstring>
#ifndef _MSC_VER
#define __declspec(x)
#endif
struct CThingBaseDef {
    unsigned char _pad00[0x42];
    unsigned char IsLoadableGlobal : 1;          // +0x42 bit 0
    unsigned char GameStatePersisted : 1;        // bit 1
};
struct CDefPointer {                             // CDefPointer<const CThingBaseDef>
    CThingBaseDef* Object;
};
struct CDefinitionManager {
    bool GetDef(long globalIndex, CDefPointer* out);   // 0x0042AF3C
};
struct CThingManager {
    unsigned char _pad00[0x20];
    CDefinitionManager* DefinitionManager;       // +0x20
    unsigned char _pad24[0x54 - 0x24];
    bool NonScriptedEntitesPauseMode;            // +0x54
    unsigned char _pad55[0x80 - 0x55];
    long GlobalThingLoadingBehaviour;            // +0x80
    void OnThingCreated();                       // 0x0051E2E0 (name not recovered)
};
extern CThingManager* g_pThingManager;
struct CTCBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void OnCreate();                     // slot 4 (+0x10)
    void* ParentThing;                           // +0x04
    bool ToBeRemoved;                            // +0x08
};
struct SComponentPair { long first; CTCBase* second; };
struct CKeyPairCompareLess { };
struct CComponentVector {                        // CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess>: vector + compare + sorted
    SComponentPair*     _M_start;
    SComponentPair*     _M_finish;
    SComponentPair*     _M_end_of_storage;
    CKeyPairCompareLess m_compare;
    bool                m_sorted;
    CComponentVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }   // test-only default ctor
    CComponentVector(const CComponentVector& other);   // 0x004CC290
    ~CComponentVector() { if (_M_start) free(_M_start); }
};
SComponentPair* CopyComponents_CComponentVector_0(const CComponentVector& other, SComponentPair** last);   // stand-in callee
// Stand-in body (the real copy constructor is out of line in retail): it fills the three words only inline, so VC7.1
// knows the local vector's address does not escape the call (the landed 0x00848730 idiom).
__declspec(noinline) CComponentVector::CComponentVector(const CComponentVector& other)
{
    SComponentPair* last;
    _M_start = CopyComponents_CComponentVector_0(other, &last);
    _M_finish = last;
    _M_end_of_storage = last;
}
struct CTCDriverBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28();
    virtual void OnThingCreated();               // slot 29 (+0x74)
};
struct CThing {
    virtual void v00();
    unsigned char  _pad04[0x44 - 0x04];
    CComponentVector Components;                 // +0x44 CVectorMap (0x10 with padding)
    unsigned char  _pad54[0x5c - 0x54];
    CTCDriverBase* PTCDriver;                    // +0x5c
    unsigned char  _pad60[0x70 - 0x60];
    CDefPointer    PBaseDef;                     // +0x70
    unsigned char  _pad74[0x8c - 0x74];
    unsigned short DefGlobalIndex;               // +0x8c
    unsigned char  _pad8e[0x91 - 0x8e];
    unsigned char  Dead : 1;                     // +0x91
    unsigned char  CreatedByOwnerThingCreation : 1;
    unsigned char  IncludeInThingScans : 1;
    unsigned char  Activated : 1;
    unsigned char  GloballyLoaded : 1;
    unsigned char  InLimbo : 1;
    unsigned char  KillLocked : 1;
    unsigned char  CreatedBySerialisation : 1;
    unsigned char  DeletedOnOwnerLevelOutOfScope : 1;   // +0x92
    unsigned char  Drawable : 1;
    unsigned char  Dying : 1;
    unsigned char  ForceFrameUpdate : 1;
    unsigned char  LevelPersistent : 1;
    unsigned char  GameStatePersisted : 1;
    unsigned char  PersistOnUnloadWhenInLimbo : 1;
    unsigned char  PausedDuringCutscenes : 1;
    unsigned char  NotPausedDuringCutscenes : 1;        // +0x93
    unsigned char  DeathPersistent : 1;
    unsigned char  KillOnLevelUnload : 1;
    unsigned char  ActivatedScript : 1;
    unsigned char  LevelIsUnloading : 1;
    unsigned char  IsAffectedByBulletTimeOverride : 1;
    unsigned char  AddingTCsPermitted : 1;
    unsigned char  InitialisationStage : 3;             // +0x94
    void RegisterWithManager(CThingManager* manager);   // 0x00A36F80 (name not recovered)
    CDefinitionManager* GetDefinitionManager() const { return g_pThingManager->DefinitionManager; }   // inline copy of 0x004C7990
    bool OnCreate();
};

bool CThing::OnCreate()
{
    ++InitialisationStage;
    if (Dead)
        return false;
    if (!DefGlobalIndex || GetDefinitionManager()->GetDef(DefGlobalIndex, &PBaseDef)) {
        if ((!PBaseDef.Object || PBaseDef.Object->IsLoadableGlobal) && g_pThingManager->GlobalThingLoadingBehaviour == 1)
            GloballyLoaded = true;
    }
    if (PBaseDef.Object && PBaseDef.Object->GameStatePersisted) {
        LevelPersistent = true;
        GameStatePersisted = true;
    }
    g_pThingManager->OnThingCreated();
    if (PTCDriver)
        PTCDriver->OnThingCreated();
    CComponentVector components(Components);
    SComponentPair* end = components._M_finish;
    for (SComponentPair* it = components._M_start; it != end; ++it) {
        CTCBase* component = it->second;
        if (!component->ToBeRemoved)
            component->OnCreate();
    }
    RegisterWithManager(g_pThingManager);
    if (g_pThingManager->NonScriptedEntitesPauseMode) {
        PausedDuringCutscenes = false;
        NotPausedDuringCutscenes = true;
    }
    return true;
}
// ---- test doubles ----
static CThingBaseDef* g_defForIndex; static bool g_getDefOk; static int g_getDefCalls;
bool CDefinitionManager::GetDef(long, CDefPointer* out) { ++g_getDefCalls; if (!g_getDefOk) return false; out->Object = g_defForIndex; return true; }
static int g_mgrCalls = 0, g_registerCalls = 0;
void CThingManager::OnThingCreated() { ++g_mgrCalls; }
static CDefinitionManager g_dm; static CThingManager g_tm;
CThingManager* g_pThingManager = &g_tm;
struct CTestTC : CTCBase {
    int created;
    virtual void v00() {} virtual void v01() {} virtual void v02() {} virtual void v03() {}
    virtual void OnCreate() { ++created; }
};
void CTCBase::v00() {} void CTCBase::v01() {} void CTCBase::v02() {} void CTCBase::v03() {} void CTCBase::OnCreate() {}
SComponentPair* CopyComponents_CComponentVector_0(const CComponentVector& other, SComponentPair** last)
{
    long n = (long)(other._M_finish - other._M_start);
    SComponentPair* p = (SComponentPair*)std::malloc(sizeof(SComponentPair) * (n ? n : 1));
    for (long i = 0; i < n; ++i) p[i] = other._M_start[i];
    *last = p + n; return p;
}
struct CTestDriver : CTCDriverBase { int created; virtual void OnThingCreated() { ++created; } };
void CTCDriverBase::OnThingCreated() {}
#define VD(n) void CTCDriverBase::v##n() {}
VD(00) VD(01) VD(02) VD(03) VD(04) VD(05) VD(06) VD(07) VD(08) VD(09) VD(10) VD(11) VD(12) VD(13) VD(14)
VD(15) VD(16) VD(17) VD(18) VD(19) VD(20) VD(21) VD(22) VD(23) VD(24) VD(25) VD(26) VD(27) VD(28)
void CThing::v00() {}
void CThing::RegisterWithManager(CThingManager* m) { if (m == &g_tm) ++g_registerCalls; }
int main()
{
    g_tm.DefinitionManager = &g_dm; g_tm.NonScriptedEntitesPauseMode = true; g_tm.GlobalThingLoadingBehaviour = 1;
    CThingBaseDef def; std::memset(&def, 0, sizeof def); def.IsLoadableGlobal = 1; def.GameStatePersisted = 1;
    g_defForIndex = &def; g_getDefOk = true;
    CTestTC tcs[2]; tcs[0].created = 0; tcs[1].created = 0; tcs[0].ToBeRemoved = false; tcs[1].ToBeRemoved = true;
    SComponentPair pairs[2] = { { 1, &tcs[0] }, { 2, &tcs[1] } };
    CTestDriver driver; driver.created = 0;
    CThing t;
    unsigned char* flags = (unsigned char*)&t + 0;   // (fields set individually below)
    (void)flags;
    t.Components._M_start = pairs; t.Components._M_finish = pairs + 2; t.Components._M_end_of_storage = pairs + 2;
    t.PTCDriver = &driver; t.PBaseDef.Object = 0; t.DefGlobalIndex = 7;
    t.Dead = 0; t.GloballyLoaded = 0; t.LevelPersistent = 0; t.GameStatePersisted = 0; t.PausedDuringCutscenes = 1;
    t.NotPausedDuringCutscenes = 0; t.InitialisationStage = 2; t.Drawable = 1;
    bool ok = t.OnCreate() && t.InitialisationStage == 3 && g_getDefCalls == 1 && t.PBaseDef.Object == &def
        && t.GloballyLoaded && t.LevelPersistent && t.GameStatePersisted && g_mgrCalls == 1 && driver.created == 1
        && tcs[0].created == 1 && tcs[1].created == 0 && g_registerCalls == 1 && !t.PausedDuringCutscenes
        && t.NotPausedDuringCutscenes && t.Drawable;
    // dead: only the stage advances (7 wraps to 0)
    t.Dead = 1; t.InitialisationStage = 7;
    ok = ok && !t.OnCreate() && t.InitialisationStage == 0 && g_mgrCalls == 1;
    // failed lookup: not globally loaded, def flags still tested on the old pointer (null here)
    t.Dead = 0; t.GloballyLoaded = 0; t.LevelPersistent = 0; t.GameStatePersisted = 0; t.PBaseDef.Object = 0; g_getDefOk = false;
    g_tm.NonScriptedEntitesPauseMode = false; t.PausedDuringCutscenes = 1;
    ok = ok && t.OnCreate() && !t.GloballyLoaded && !t.LevelPersistent && t.PausedDuringCutscenes && g_mgrCalls == 2;
    // index 0: no lookup, no def -> globally loaded when the manager loads globally
    t.DefGlobalIndex = 0; g_getDefCalls = 0;
    ok = ok && t.OnCreate() && g_getDefCalls == 0 && t.GloballyLoaded;
    t.Components._M_start = 0;   // the borrowed pairs are not heap blocks
    if (!ok) { std::printf("004c9560_TEST FAIL\n"); return 1; }
    std::printf("004c9560_TEST PASS\n");
    return 0;
}
