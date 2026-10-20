// Behaviour test for 00f02030: the model below is a copy of src/00f02030.cpp (compiled separately), driven by test doubles.
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __fastcall
#define __stdcall
#define __cdecl
#define __fastcall
#define __forceinline inline
#define __int64 long long
#endif
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};

struct CScriptThing {
    virtual void v000() {} virtual void v001() {} virtual void v002() {} virtual void v003() {} virtual void v004() {} virtual void v005() {} virtual void v006() {} virtual void v007() {} virtual void v008() {} virtual void v009() {}
    virtual void v010() {} virtual void v011() {} virtual void v012() {} virtual void v013() {} virtual void v014() {} virtual void v015() {} virtual void v016() {} virtual void v017() {} virtual void v018() {} virtual void v019() {}
    virtual void v020() {} virtual void v021() {} virtual void v022() {} virtual void v023() {} virtual void v024() {} virtual void v025() {} virtual void v026() {} virtual void v027() {} virtual void v028() {} virtual void v029() {}
    virtual void v030() {} virtual void v031() {} virtual void v032() {} virtual void v033() {} virtual void v034() {} virtual void v035() {} virtual void v036() {} virtual void v037() {} virtual void v038() {} virtual void v039() {}
    virtual void v040() {} virtual void v041() {} virtual void v042() {} virtual void v043() {} virtual void v044() {} virtual void v045() {} virtual void v046() {} virtual void v047() {} virtual void v048() {} virtual void v049() {}
    virtual void v050() {} virtual void v051() {} virtual void v052() {} virtual void v053() {} virtual void v054() {} virtual void v055() {} virtual void v056() {} virtual void v057() {} virtual void v058() {} virtual void v059() {}
    virtual void v060() {} virtual void v061() {} virtual void v062() {} virtual void v063() {} virtual void v064() {} virtual void v065() {} virtual void v066() {} virtual void v067() {} virtual void v068() {} virtual void v069() {}
    virtual void v070() {} virtual void v071() {} virtual void v072() {} virtual void v073() {} virtual void v074() {}
    virtual bool IsAlive() const;                       // slot 75 (+0x12c)
};

struct CGameScriptInterface {
    virtual void v000() {} virtual void v001() {} virtual void v002() {} virtual void v003() {} virtual void v004() {} virtual void v005() {} virtual void v006() {}
    virtual void NewScriptFrame() const;                // slot 7 (+0x1c)
    virtual void v008() {} virtual void v009() {} virtual void v010() {}
    virtual bool IsLevelLoaded(const CCharString& level) const;     // slot 11 (+0x2c)
    virtual void v012() {} virtual void v013() {} virtual void v014() {} virtual void v015() {} virtual void v016() {} virtual void v017() {} virtual void v018() {} virtual void v019() {} virtual void v020() {} virtual void v021() {}
    virtual void v022() {} virtual void v023() {} virtual void v024() {} virtual void v025() {} virtual void v026() {} virtual void v027() {} virtual void v028() {} virtual void v029() {} virtual void v030() {} virtual void v031() {}
    virtual void v032() {} virtual void v033() {} virtual void v034() {} virtual void v035() {} virtual void v036() {} virtual void v037() {} virtual void v038() {} virtual void v039() {} virtual void v040() {} virtual void v041() {}
    virtual void v042() {} virtual void v043() {} virtual void v044() {} virtual void v045() {} virtual void v046() {} virtual void v047() {} virtual void v048() {} virtual void v049() {} virtual void v050() {} virtual void v051() {}
    virtual void v052() {} virtual void v053() {} virtual void v054() {} virtual void v055() {} virtual void v056() {} virtual void v057() {} virtual void v058() {} virtual void v059() {} virtual void v060() {} virtual void v061() {}
    virtual void v062() {} virtual void v063() {} virtual void v064() {} virtual void v065() {} virtual void v066() {} virtual void v067() {} virtual void v068() {} virtual void v069() {}
    virtual CScriptThing& GetHero();                    // slot 70 (+0x118)
};

namespace NScript {
struct CScriptBase {                                    // retail 0x40 bytes (PDB 0x54: debug containers)
    virtual void v00() {}
    unsigned char _body[0x3c];                          // +0x04 .. +0x40 (PActiveThread at +0x2c)
    bool IsActiveThreadTerminating();                   // 0x00CB7940
};

struct CQ_CinemaTestScript : CScriptBase {
    CGameScriptInterface* GSI;                          // +0x40
    void*                 Owner;                        // +0x44
    bool MissionSucceeded;                              // +0x48
    bool MissionFailed;                                 // +0x49
    bool MissionOver;                                   // +0x4a
    void WatchForLeaving();
};

void CQ_CinemaTestScript::WatchForLeaving()
{
    while (GSI->IsLevelLoaded("ArenaHallOfHeroes") && GSI->GetHero().IsAlive() && !MissionFailed && !MissionSucceeded) {
        GSI->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (!IsActiveThreadTerminating() && !MissionSucceeded)
        MissionFailed = true;
}
}

// ---- test doubles and driver ----
static int g_frames = 0, g_termAt = 1000, g_unloadAt = 1000, g_deadAt = 1000, g_ctors = 0, g_dtors = 0, g_bad = 0;
static int g_setAt = 1000, g_setWhich = 0;
static NScript::CQ_CinemaTestScript* g_script = 0;
CCharString::CCharString(const char* text, long length) { ++g_ctors; if (std::strcmp(text, "ArenaHallOfHeroes") != 0 || length != -1) g_bad = 1; PStringData = (void*)text; }
CCharString::~CCharString() { ++g_dtors; }
bool CScriptThing::IsAlive() const { return g_frames < g_deadAt; }
static CScriptThing g_hero;
void CGameScriptInterface::NewScriptFrame() const
{
    ++g_frames;
    if (g_frames == g_setAt) {
        if (g_setWhich == 0) g_script->MissionSucceeded = true;
        if (g_setWhich == 1) g_script->MissionFailed = true;
    }
}
bool CGameScriptInterface::IsLevelLoaded(const CCharString& level) const
{
    if (std::strcmp((const char*)level.PStringData, "ArenaHallOfHeroes") != 0) g_bad = 1;
    return g_frames < g_unloadAt;
}
CScriptThing& CGameScriptInterface::GetHero() { return g_hero; }
bool NScript::CScriptBase::IsActiveThreadTerminating() { return g_frames >= g_termAt; }

static bool Run(int termAt, int unloadAt, int deadAt, int setAt, int which, int expectFrames, bool expectFailed, bool expectSucceeded)
{
    static CGameScriptInterface gsi;
    NScript::CQ_CinemaTestScript s;
    std::memset(s._body, 0, sizeof(s._body));
    s.GSI = &gsi; s.Owner = 0; s.MissionSucceeded = s.MissionFailed = s.MissionOver = false;
    g_script = &s; g_frames = 0; g_termAt = termAt; g_unloadAt = unloadAt; g_deadAt = deadAt; g_setAt = setAt; g_setWhich = which;
    g_ctors = g_dtors = 0;
    s.WatchForLeaving();
    return g_frames == expectFrames && s.MissionFailed == expectFailed && s.MissionSucceeded == expectSucceeded
        && g_ctors == g_dtors && g_ctors > 0 && !g_bad && !s.MissionOver;
}

int main()
{
    bool ok = true;
    ok = ok && Run(1000, 3, 1000, 1000, 0, 3, true, false);    // hero left the arena on frame 3: failed
    ok = ok && Run(1000, 1000, 2, 1000, 0, 2, true, false);    // hero died on frame 2: failed
    ok = ok && Run(1000, 1000, 1000, 4, 0, 4, false, true);    // mission succeeded: not failed
    ok = ok && Run(5, 1000, 1000, 1000, 0, 5, false, false);   // thread terminated: nothing set
    ok = ok && Run(1000, 0, 1000, 1000, 0, 0, true, false);    // level never loaded: failed at once
    ok = ok && Run(1000, 1000, 1000, 2, 1, 2, true, false);    // failed elsewhere: stays failed
    if (!ok) { std::printf("00f02030_TEST FAIL\n"); return 1; }
    std::printf("00f02030_TEST PASS\n");
    return 0;
}