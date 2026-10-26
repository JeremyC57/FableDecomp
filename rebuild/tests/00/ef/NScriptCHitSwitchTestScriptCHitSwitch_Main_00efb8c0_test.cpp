#include <cstdio>
#include <cstring>

static int g_frames, g_filters, g_doneAfter = 1000, g_doneCalls, g_hitCalls, g_paused;
static int g_hitAt[4]; static int g_nHits;
static char g_log[64];
static void Log(char c) { size_t n = std::strlen(g_log); g_log[n] = c; g_log[n + 1] = 0; }
class CCharString { public: const char* p; };
class CScriptThing { public:
    int id; CCharString name;
    const CCharString& GetName() const { return name; }
    bool MsgIsHitBy(const CCharString& who) const {
        ++g_hitCalls;
        if (std::strcmp(who.p, "HERO") != 0) return false;
        for (int i = 0; i < g_nHits; ++i) if (g_hitAt[i] == g_frames) return true;
        return false;
    }
    void SetActivationTriggerStatus(bool on) { Log(on ? 'A' : 'a'); }
};
static CScriptThing g_hero;
struct CGameScriptInterface_Calls {
    void NewScriptFrame() const { ++g_frames; }
    CScriptThing& GetHero() { return g_hero; }
    void Pause(float s) const { if (s == 10.0f) { Log('P'); ++g_paused; } }
};
namespace N_00ef7ab0 { struct CIsThingAlive {
    CScriptThing m_thing;
    CIsThingAlive(const CScriptThing& thing) : m_thing(thing) { ++g_filters; }
    ~CIsThingAlive() { --g_filters; }
}; } using N_00ef7ab0::CIsThingAlive;
static bool FoundMessageIsThingHitBy(CScriptThing* thing, CScriptThing* hitter) { return thing->MsgIsHitBy(hitter->GetName()); }
struct CHitSwitch {
    CGameScriptInterface_Calls* Game;
    CScriptThing Me;
    void RegisterFilter(CIsThingAlive& filter) { if (filter.m_thing.id == 7 && g_filters == 1) Log('R'); }
    bool IsDone() { ++g_doneCalls; return g_doneCalls >= g_doneAfter; }
    void Main()
    {
        {
            CIsThingAlive filter(Me);
            RegisterFilter(filter);
        }
        Game->NewScriptFrame();
        if (IsDone()) return;
        while (!IsDone()) {
            while (!FoundMessageIsThingHitBy(&Me, &Game->GetHero())) {
                Game->NewScriptFrame();
                if (IsDone()) return;
            }
            if (IsDone()) return;
            Me.SetActivationTriggerStatus(true);
            Game->Pause(10.0f);
            Me.SetActivationTriggerStatus(false);
            Game->NewScriptFrame();
        }
    }
};
static void Reset() { g_log[0] = 0; g_frames = g_doneCalls = g_hitCalls = g_paused = 0; g_doneAfter = 1000; g_nHits = 0; }
static int Fail() { std::printf("00efb8c0_TEST FAIL\n"); return 1; }
int main()
{
    CGameScriptInterface_Calls game; g_hero.name.p = "HERO";
    CHitSwitch s; s.Game = &game; s.Me.id = 7; s.Me.name.p = "SWITCH";
    // hit on frames 2 and 4; ends at the 9th IsDone call
    Reset(); g_hitAt[0] = 2; g_hitAt[1] = 4; g_nHits = 2; g_doneAfter = 9;
    s.Main();
    // calls: 1 entry, 2 while, 3 inner(f1->2), 4 after-hit, [APa f3], 5 while, 6 inner(f3->4), 7 after-hit, [APa f5], 8 while, 9 inner -> return
    if (std::strcmp(g_log, "RAPaAPa") != 0 || g_paused != 2 || g_filters != 0 || g_frames != 6) return Fail();
    Reset(); g_doneAfter = 1;                    // ending right after the first frame
    s.Main();
    if (std::strcmp(g_log, "R") != 0 || g_frames != 1 || g_hitCalls != 0) return Fail();
    Reset(); g_doneAfter = 2;                    // ending at the outer loop's entry test
    s.Main();
    if (std::strcmp(g_log, "R") != 0 || g_hitCalls != 0) return Fail();
    Reset(); g_hitAt[0] = 1; g_nHits = 1; g_doneAfter = 3;   // hit seen, but the script ends before acting
    s.Main();
    if (std::strcmp(g_log, "R") != 0 || g_hitCalls != 1) return Fail();
    std::printf("00efb8c0_TEST PASS\n");
    return 0;
}