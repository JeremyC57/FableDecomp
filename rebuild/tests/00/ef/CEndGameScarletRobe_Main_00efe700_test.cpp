// Behaviour test for CEndGameScarletRobe::Main (0x00EFE700): filter, no-information flags, then repeated
// StartScriptingEntity attempts on a fresh local handle until it succeeds or the script is done.
#include <cstdio>
class CScriptThing { public: int id; };
static int g_frames = 0, g_filters = 0, g_infoCalls = 0, g_starts = 0, g_handleDtors = 0, g_isDone = 0;
static bool g_info[3]; static int g_priority = 0, g_succeedOn = 0;
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_HIGHEST = 4 };
class CScriptGameResourceObjectScriptedThingBase { public:
    void* Data; void* Info;
    CScriptGameResourceObjectScriptedThingBase() : Data(0), Info(0) {}
    ~CScriptGameResourceObjectScriptedThingBase() { ++g_handleDtors; }
    bool IsScripting() { return Data != 0; }
    void Release() { Data = 0; }
};
class CGameScriptInterface { public:
    virtual void NewScriptFrame() const { ++g_frames; }
    virtual bool StartScriptingEntity(const CScriptThing&, CScriptGameResourceObjectScriptedThingBase& h, EScriptAIPriority p)
    { ++g_starts; g_priority = p; if (g_starts == g_succeedOn) { h.Data = &h; return true; } return false; }
    virtual void SetThingHasInformation(const CScriptThing&, bool a, bool b, bool c) const { ++g_infoCalls; g_info[0] = a; g_info[1] = b; g_info[2] = c; }
};
struct CIsThingAlive { CScriptThing m_thing; CIsThingAlive(const CScriptThing& t) : m_thing(t) {} ~CIsThingAlive() {} };
class CEndGameScarletRobe {
public:
    CGameScriptInterface* Game; CScriptThing Me; int doneAfter;
    void RegisterFilter(CIsThingAlive& f) { if (f.m_thing.id == Me.id) ++g_filters; }
    bool IsDone() { ++g_isDone; return g_frames >= doneAfter; }
    void Main();
};
void CEndGameScarletRobe::Main()
{
    {
        CIsThingAlive filter(Me);
        RegisterFilter(filter);
    }
    Game->NewScriptFrame();
    if (IsDone())
        return;
    Game->SetThingHasInformation(Me, false, true, false);
    CScriptGameResourceObjectScriptedThingBase handle;
    if (handle.IsScripting())
        handle.Release();
    while (!Game->StartScriptingEntity(Me, handle, SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game->NewScriptFrame();
        if (IsDone())
            return;
    }
    IsDone();
}
int main()
{
    CGameScriptInterface game; CEndGameScarletRobe s; s.Game = &game; s.Me.id = 3;
    int bad = 0;
    s.doneAfter = 1; s.Main();
    if (g_filters != 1 || g_infoCalls || g_handleDtors) bad = 1;
    g_frames = 0; s.doneAfter = 50; g_succeedOn = 3; s.Main();
    if (g_infoCalls != 1 || g_info[0] || !g_info[1] || g_info[2] || g_starts != 3 || g_priority != 4 || g_frames != 3 || g_handleDtors != 1) bad = 2;
    g_frames = 0; g_starts = 0; g_succeedOn = 100; s.doneAfter = 4; s.Main();
    if (g_frames != 4 || g_handleDtors != 2) bad = 3;   // gives up when done, handle still destroyed
    if (bad) { std::printf("00efe700_TEST FAIL\n"); return 1; }
    std::printf("00efe700_TEST PASS\n");
    return 0;
}