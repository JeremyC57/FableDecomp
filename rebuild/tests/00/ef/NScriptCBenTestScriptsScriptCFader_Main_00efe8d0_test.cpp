#include <cstdio>
#include <cstring>

static int g_strings, g_frames, g_filters, g_talkCalls, g_useCalls, g_doneAfter = 1000, g_doneCalls;
static int g_talkAt = -1, g_useAt = -1;
static char g_log[64];
static void Log(char c) { size_t n = std::strlen(g_log); g_log[n] = c; g_log[n + 1] = 0; }
struct CCharString {
    const char* p;
    CCharString(const char* text, long = -1) : p(text) { ++g_strings; }
    ~CCharString() { --g_strings; }
};
struct CScriptThing {
    int id;
    bool MsgIsTalkedToBy(const CCharString& who) const { ++g_talkCalls; return std::strcmp(who.p, "SCRIPT_NAME_HERO") == 0 && g_frames == g_talkAt; }
    bool MsgIsUsedBy(const CCharString& who) const { ++g_useCalls; return std::strcmp(who.p, "SCRIPT_NAME_HERO") == 0 && g_frames == g_useAt; }
};
struct CGameScriptInterface_Calls {
    void NewScriptFrame() const { ++g_frames; }
    void SetThingHasInformation(const CScriptThing&, bool a, bool b, bool c) const { if (!a && b && !c) Log('I'); }
};
namespace N_00ef7ab0 { struct CIsThingAlive {
    CScriptThing m_thing;
    CIsThingAlive(const CScriptThing& thing) : m_thing(thing) { ++g_filters; }
    ~CIsThingAlive() { --g_filters; }
}; } using N_00ef7ab0::CIsThingAlive;
struct CBenTestScriptsScript {
    void FadeEntityOut(const CScriptThing& thing, bool fadeOut) { if (thing.id == 7 && fadeOut) Log('F'); }
};
struct CFader {
    CGameScriptInterface_Calls* Game;
    CScriptThing Me;
    CBenTestScriptsScript* ParentClass;
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
        Game->SetThingHasInformation(Me, false, true, false);
        for (;;) {
            bool waiting = !(Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO") || Me.MsgIsUsedBy("SCRIPT_NAME_HERO"));
            if (!waiting) break;
            Game->NewScriptFrame();
            if (IsDone()) return;
        }
        if (IsDone()) return;
        ParentClass->FadeEntityOut(Me, true);
    }
};
static void Reset() { g_log[0] = 0; g_frames = g_talkCalls = g_useCalls = g_doneCalls = 0; g_doneAfter = 1000; g_talkAt = g_useAt = -1; }
int main()
{
    CGameScriptInterface_Calls game; CBenTestScriptsScript parent;
    CFader f; f.Game = &game; f.Me.id = 7; f.ParentClass = &parent;
    Reset(); g_talkAt = 3;                       // talked to on the third frame: the use test is skipped there
    f.Main();
    if (std::strcmp(g_log, "RIF") != 0 || g_frames != 3 || g_talkCalls != 3 || g_useCalls != 2 || g_strings != 0 || g_filters != 0) {
        std::printf("00efe8d0_TEST FAIL\n"); return 1;
    }
    Reset(); g_useAt = 2;
    f.Main();
    if (std::strcmp(g_log, "RIF") != 0 || g_frames != 2 || g_useCalls != 2 || g_strings != 0) { std::printf("00efe8d0_TEST FAIL\n"); return 1; }
    Reset(); g_doneAfter = 1;                    // ending right after the first frame
    f.Main();
    if (std::strcmp(g_log, "R") != 0) { std::printf("00efe8d0_TEST FAIL\n"); return 1; }
    Reset(); g_doneAfter = 3;                    // ending inside the wait loop
    f.Main();
    if (std::strcmp(g_log, "RI") != 0 || g_frames != 3) { std::printf("00efe8d0_TEST FAIL\n"); return 1; }
    std::printf("00efe8d0_TEST PASS\n");
    return 0;
}