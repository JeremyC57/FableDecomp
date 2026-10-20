// Behaviour test for 00e1f2d0: the model below is a copy of src/00e1f2d0.cpp (compiled separately), driven by test doubles.
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __fastcall
#define __stdcall
#define __cdecl
#define __thiscall
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
    virtual void v010() {} virtual void v011() {} virtual void v012() {} virtual void v013() {} virtual void v014() {} virtual void v015() {} virtual void v016() {} virtual void v017() {}
    virtual bool MsgIsKilledBy(const CCharString& killer) const;   // slot 18 (+0x48)
};

struct CGameScriptInterface {
    virtual void v000() {} virtual void v001() {} virtual void v002() {} virtual void v003() {} virtual void v004() {} virtual void v005() {} virtual void v006() {}
    virtual void NewScriptFrame() const;                // slot 7 (+0x1c)
    virtual void v008() {} virtual void v009() {} virtual void v010() {} virtual void v011() {} virtual void v012() {} virtual void v013() {} virtual void v014() {} virtual void v015() {} virtual void v016() {} virtual void v017() {}
    virtual void v018() {} virtual void v019() {} virtual void v020() {} virtual void v021() {} virtual void v022() {} virtual void v023() {} virtual void v024() {} virtual void v025() {} virtual void v026() {} virtual void v027() {}
    virtual void v028() {} virtual void v029() {} virtual void v030() {} virtual void v031() {} virtual void v032() {} virtual void v033() {} virtual void v034() {} virtual void v035() {} virtual void v036() {} virtual void v037() {}
    virtual void v038() {} virtual void v039() {} virtual void v040() {} virtual void v041() {} virtual void v042() {} virtual void v043() {} virtual void v044() {} virtual void v045() {} virtual void v046() {} virtual void v047() {}
    virtual void v048() {} virtual void v049() {} virtual void v050() {} virtual void v051() {} virtual void v052() {} virtual void v053() {} virtual void v054() {} virtual void v055() {} virtual void v056() {} virtual void v057() {}
    virtual void v058() {} virtual void v059() {} virtual void v060() {} virtual void v061() {} virtual void v062() {} virtual void v063() {} virtual void v064() {} virtual void v065() {} virtual void v066() {} virtual void v067() {}
    virtual void v068() {} virtual void v069() {}
    virtual CScriptThing& GetHero();                    // slot 70 (+0x118)
};

namespace NScript {
struct CScriptBase {                                    // retail 0x40 bytes (PDB 0x54: debug containers)
    virtual void v00() {}
    unsigned char _body[0x3c];                          // +0x04 .. +0x40 (PActiveThread at +0x2c)
    bool IsActiveThreadTerminating();                   // 0x00CB7940
};

struct CQR_EscortTraderScript : CScriptBase {
    CGameScriptInterface* GSI;                          // +0x40
    void*                 Owner;                        // +0x44
    bool MissionSucceeded;                              // +0x48
    bool MissionFailed;                                 // +0x49
    bool HiredByTrader;                                 // +0x4a
    bool TraderAtDestination;                           // +0x4b
    bool TraderDepartedLost;                            // +0x4c
    bool GreetedHero;                                   // +0x4d
    bool TraderKilled;                                  // +0x4e
    void WatchForFailure();
};

void CQR_EscortTraderScript::WatchForFailure()
{
    GSI->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    for (;;) {
        bool waiting = !(GSI->GetHero().MsgIsKilledBy("") || MissionFailed || MissionSucceeded || TraderKilled
                         || TraderDepartedLost);
        if (!waiting)
            break;
        GSI->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (!IsActiveThreadTerminating() && !MissionSucceeded)
        MissionFailed = true;
}
}

// ---- test doubles and driver ----
// test doubles: a scripted world where the frame counter drives the flags
static int g_frames = 0, g_termAt = 1000, g_killedAt = 1000, g_ctors = 0, g_dtors = 0, g_badKiller = 0;
static NScript::CQR_EscortTraderScript* g_script = 0;
static int g_setAt = 1000; static int g_setWhich = 0;
CCharString::CCharString(const char* text, long length) { ++g_ctors; if (std::strcmp(text, "") != 0 || length != -1) g_badKiller = 1; PStringData = (void*)text; }
CCharString::~CCharString() { ++g_dtors; }
bool CScriptThing::MsgIsKilledBy(const CCharString& killer) const { if (std::strcmp((const char*)killer.PStringData, "") != 0) g_badKiller = 1; return g_frames >= g_killedAt; }
static CScriptThing g_hero;
void CGameScriptInterface::NewScriptFrame() const
{
    ++g_frames;
    if (g_frames == g_setAt) {
        if (g_setWhich == 0) g_script->TraderKilled = true;
        if (g_setWhich == 1) g_script->MissionSucceeded = true;
        if (g_setWhich == 2) g_script->TraderDepartedLost = true;
        if (g_setWhich == 3) g_script->MissionFailed = true;
    }
}
CScriptThing& CGameScriptInterface::GetHero() { return g_hero; }
bool NScript::CScriptBase::IsActiveThreadTerminating() { return g_frames >= g_termAt; }

static bool Run(int termAt, int killedAt, int setAt, int which, int expectFrames, bool expectFailed, bool expectSucceeded)
{
    static CGameScriptInterface gsi;
    NScript::CQR_EscortTraderScript s;
    std::memset(s._body, 0, sizeof(s._body));
    s.GSI = &gsi; s.Owner = 0;
    s.MissionSucceeded = s.MissionFailed = s.HiredByTrader = s.TraderAtDestination = false;
    s.TraderDepartedLost = s.GreetedHero = s.TraderKilled = false;
    g_script = &s; g_frames = 0; g_termAt = termAt; g_killedAt = killedAt; g_setAt = setAt; g_setWhich = which;
    g_ctors = g_dtors = 0;
    s.WatchForFailure();
    if (g_frames != expectFrames) return false;
    if (s.MissionFailed != expectFailed && which != 3) return false;
    if (which == 3 && !s.MissionFailed) return false;
    if (s.MissionSucceeded != expectSucceeded) return false;
    if (g_ctors != g_dtors || g_badKiller) return false;
    return true;
}

int main()
{
    bool ok = true;
    ok = ok && Run(1, 1000, 1000, 0, 1, false, false);   // thread terminated after the first frame: nothing set
    ok = ok && Run(1000, 1000, 4, 0, 4, true, false);    // trader killed on frame 4: mission failed
    ok = ok && Run(1000, 3, 1000, 0, 3, true, false);    // hero killed on frame 3: mission failed
    ok = ok && Run(1000, 1000, 2, 1, 2, false, true);    // mission succeeded: failure not set
    ok = ok && Run(1000, 1000, 5, 2, 5, true, false);    // trader lost: mission failed
    ok = ok && Run(6, 1000, 1000, 0, 6, false, false);   // terminated while waiting: nothing set
    ok = ok && Run(1000, 1000, 3, 3, 3, true, false);    // already failed elsewhere: stays failed
    if (!ok) { std::printf("00e1f2d0_TEST FAIL\n"); return 1; }
    std::printf("00e1f2d0_TEST PASS\n");
    return 0;
}
