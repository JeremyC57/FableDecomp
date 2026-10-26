// Behaviour test for CV_TourGuideScript::WatchForGuideKilled (0x00EE4A70): waits for the guide to be alive, then
// for his death; sets TourGuideKilled only when the thread is not terminating; a null guide never becomes alive.
#include <cstdio>
#include <cstring>
class CCharString { public: const char* Text; CCharString(const char* t) : Text(t) {} };
struct Impl {
    int AliveAfter, KilledAfter, aliveCalls, killedCalls;
    bool IsAlive() { return ++aliveCalls > AliveAfter; }
    bool MsgIsKilledBy(const CCharString& k) { return std::strcmp(k.Text, "") == 0 && ++killedCalls > KilledAfter; }
};
class CScriptThing { public:
    Impl* Data;
    bool IsAlive() const { if (!Data) return false; return Data->IsAlive(); }
    bool MsgIsKilledBy(const CCharString& k) const { if (!Data) return false; return Data->MsgIsKilledBy(k); }
};
static Impl* g_impl; static int g_frames; static int g_termAt; static int g_termCalls; static const char* g_asked;
class CGameScriptInterface { public:
    void NewScriptFrame() const { ++g_frames; }
    CScriptThing GetThingWithScriptName(const CCharString& n) { g_asked = n.Text; CScriptThing t; t.Data = g_impl; return t; }
};
struct CV_TourGuideScript {
    CGameScriptInterface* Game;
    bool TourGuideKilled;
    bool IsActiveThreadTerminating() { return ++g_termCalls >= g_termAt; }
    void WatchForGuideKilled()
    {
        CScriptThing guide = Game->GetThingWithScriptName("TourGuideGuide");
        while (!guide.IsAlive()) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        while (!guide.MsgIsKilledBy("")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        TourGuideKilled = true;
    }
};
static bool run(Impl* impl, int termAt, bool wantKilled, int wantFrames)
{
    g_impl = impl; g_frames = 0; g_termAt = termAt; g_termCalls = 0;
    CGameScriptInterface g; CV_TourGuideScript s; s.Game = &g; s.TourGuideKilled = false;
    s.WatchForGuideKilled();
    return s.TourGuideKilled == wantKilled && g_frames == wantFrames && std::strcmp(g_asked, "TourGuideGuide") == 0;
}
int main()
{
    Impl a = { 2, 3, 0, 0 };               // alive on 3rd poll, killed on 4th poll
    Impl b = { 0, 0, 0, 0 };
    bool ok = run(&a, 100, true, 5);       // 2 frames waiting alive + 3 waiting death
    ok = ok && run(&b, 2, false, 0);       // terminating at the macro test after loop 2
    ok = ok && run(0, 4, false, 4);        // no guide: loops until the thread terminates
    if (!ok) { std::printf("00ee4a70_TEST FAIL\n"); return 1; }
    std::printf("00ee4a70_TEST PASS\n");
    return 0;
}