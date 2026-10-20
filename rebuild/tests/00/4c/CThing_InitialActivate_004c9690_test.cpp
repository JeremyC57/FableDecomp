#include <cstdio>
#include <cstdlib>
#ifndef _MSC_VER
#define __declspec(x)
#endif
struct CThingBaseDef {
    virtual void v00();
    virtual void Destroy();                      // slot 1
    long m_refCount;                             // +0x04
    unsigned char _pad08[0x41 - 0x08];
    unsigned char AvailableInEditor : 1;         // +0x41 bit 0
    unsigned char AlwaysFrameUpdate : 1;
    unsigned char StartKillLocked : 1;
    unsigned char LevelLess : 1;
    unsigned char IsDrawable : 1;                // bit 4
};
struct CDefPointer {                             // CDefPointer<const CThingBaseDef>
    CThingBaseDef* Object;
    CDefPointer() : Object(0) { }   // test-only default ctor
    CDefPointer(const CDefPointer& other) : Object(other.Object)
    {
        if (Object)
            ++Object->m_refCount;
    }
    ~CDefPointer()
    {
        if (Object && --Object->m_refCount == 0)
            Object->Destroy();
    }
    CThingBaseDef* operator->() const { return Object; }
    operator bool() const { return Object != 0; }
};
struct CThing;
struct CThingManager {
    void AddToUpdateList(CThing* thing);         // 0x0051EC50
};
struct CEngine {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08();
    virtual bool IsSpecialMode();                // slot 9 (+0x24; name not recovered)
};
extern CEngine* g_pEngine;
extern CThingManager* g_pThingManager;
struct CTCBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void OnInitialActivate();            // slot 5 (+0x14)
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
struct CThing {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27();
    virtual void SetDrawable(bool drawable);     // slot 28 (+0x70; name not recovered)
    unsigned char  _pad04[0x44 - 0x04];
    CComponentVector Components;                 // +0x44 CVectorMap (0x10 with padding)
    unsigned char  _pad54[0x5c - 0x54];
    void*          PTCDriver;                    // +0x5c
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
    CDefPointer GetPBaseDef() const { return PBaseDef; }
    void InitialActivate();
};

void CThing::InitialActivate()
{
    ++InitialisationStage;
    Activated = true;
    g_pThingManager->AddToUpdateList(this);
    CComponentVector components(Components);
    SComponentPair* end = components._M_finish;
    for (SComponentPair* it = components._M_start; it != end; ++it) {
        CTCBase* component = it->second;
        if (!component->ToBeRemoved)
            component->OnInitialActivate();
    }
    bool hide = !g_pEngine->IsSpecialMode() && GetPBaseDef() && !GetPBaseDef()->IsDrawable;
    if (hide)
        SetDrawable(false);
}
// ---- test doubles ----

void CThingBaseDef::v00() {} void CThingBaseDef::Destroy() {}
struct CTestDef : CThingBaseDef { int destroyed; virtual void Destroy() { ++destroyed; } };
static int g_added = 0;
void CThingManager::AddToUpdateList(CThing*) { ++g_added; }
static bool g_special = false;
struct CTestEngine : CEngine {
    virtual void v00() {} virtual void v01() {} virtual void v02() {} virtual void v03() {} virtual void v04() {}
    virtual void v05() {} virtual void v06() {} virtual void v07() {} virtual void v08() {}
    virtual bool IsSpecialMode() { return g_special; }
};
void CEngine::v00() {} void CEngine::v01() {} void CEngine::v02() {} void CEngine::v03() {} void CEngine::v04() {}
void CEngine::v05() {} void CEngine::v06() {} void CEngine::v07() {} void CEngine::v08() {} bool CEngine::IsSpecialMode() { return false; }
static CTestEngine g_engine; CEngine* g_pEngine = &g_engine;
static CThingManager g_tm; CThingManager* g_pThingManager = &g_tm;
struct CTestTC : CTCBase {
    int activated;
    virtual void v00() {} virtual void v01() {} virtual void v02() {} virtual void v03() {} virtual void v04() {}
    virtual void OnInitialActivate() { ++activated; }
};
void CTCBase::v00() {} void CTCBase::v01() {} void CTCBase::v02() {} void CTCBase::v03() {} void CTCBase::v04() {}
void CTCBase::OnInitialActivate() {}
SComponentPair* CopyComponents_CComponentVector_0(const CComponentVector& other, SComponentPair** last)
{
    long n = (long)(other._M_finish - other._M_start);
    SComponentPair* p = (SComponentPair*)std::malloc(sizeof(SComponentPair) * (n ? n : 1));
    for (long i = 0; i < n; ++i) p[i] = other._M_start[i];
    *last = p + n; return p;
}
static int g_setDrawable = 0; static bool g_lastDrawable = true;
struct CTestThing : CThing {
    virtual void SetDrawable(bool d) { ++g_setDrawable; g_lastDrawable = d; }
};
#define VT(n) void CThing::v##n() {}
VT(00) VT(01) VT(02) VT(03) VT(04) VT(05) VT(06) VT(07) VT(08) VT(09) VT(10) VT(11) VT(12) VT(13) VT(14)
VT(15) VT(16) VT(17) VT(18) VT(19) VT(20) VT(21) VT(22) VT(23) VT(24) VT(25) VT(26) VT(27)
void CThing::SetDrawable(bool) {}
int main()
{
    CTestDef def; def.m_refCount = 1; def.destroyed = 0; def.IsDrawable = 0;
    CTestTC tcs[2]; tcs[0].activated = 0; tcs[1].activated = 0; tcs[0].ToBeRemoved = true; tcs[1].ToBeRemoved = false;
    SComponentPair pairs[2] = { { 3, &tcs[0] }, { 4, &tcs[1] } };
    CTestThing t;
    t.Components._M_start = pairs; t.Components._M_finish = pairs + 2;
    t.PBaseDef.Object = &def; t.InitialisationStage = 1; t.Activated = 0; t.Dead = 0;
    t.InitialActivate();
    bool ok = t.InitialisationStage == 2 && t.Activated && !t.Dead && g_added == 1 && tcs[0].activated == 0
        && tcs[1].activated == 1 && g_setDrawable == 1 && !g_lastDrawable && def.m_refCount == 1 && def.destroyed == 0;
    def.IsDrawable = 1;
    t.InitialActivate();
    ok = ok && g_setDrawable == 1 && t.InitialisationStage == 3 && tcs[1].activated == 2;
    def.IsDrawable = 0; g_special = true;
    t.InitialActivate();
    ok = ok && g_setDrawable == 1 && def.m_refCount == 1;
    g_special = false; t.PBaseDef.Object = 0;
    t.InitialActivate();
    ok = ok && g_setDrawable == 1;
    t.Components._M_start = 0; t.PBaseDef.Object = 0;
    if (!ok) { std::printf("004c9690_TEST FAIL\n"); return 1; }
    std::printf("004c9690_TEST PASS\n");
    return 0;
}