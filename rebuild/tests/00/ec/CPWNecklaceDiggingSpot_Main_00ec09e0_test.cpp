#include <cstdio>

// CPW_NecklaceDiggingSpot::Main: tick while the parent keeps the spot hidden, abort out of the wait or
// before revealing, otherwise reveal the digging spot (SetDiggingSpotAsHidden(spot, false)).
struct CScriptThing { unsigned char _body[8]; };
struct CV_ParanoidWhispersScript { bool HideDiggingSpot; };
static CV_ParanoidWhispersScript* g_parent = 0;
static int g_ticks = 0, g_hideCalls = 0, g_unhideAfterTicks = 0, g_abortAtCheck = -1, g_checks = 0;
static const CScriptThing* g_hiddenThing = 0;
static bool g_hiddenValue = true;
struct CQuestScriptInterface {
    virtual void Tick() { ++g_ticks; if (g_ticks >= g_unhideAfterTicks) g_parent->HideDiggingSpot = false; }
    virtual void SetDiggingSpotAsHidden(const CScriptThing& thing, bool hidden) { ++g_hideCalls; g_hiddenThing = &thing; g_hiddenValue = hidden; }
};
struct CPW_NecklaceDiggingSpot {
    CQuestScriptInterface*      PQuest;
    CScriptThing                DiggingSpot;
    CV_ParanoidWhispersScript*  ParentClass;
    bool IsAborted() { return g_checks++ == g_abortAtCheck; }
    void Main();
};
void CPW_NecklaceDiggingSpot::Main()
{
    while (ParentClass->HideDiggingSpot) {
        PQuest->Tick();
        if (IsAborted())
            return;
    }
    if (IsAborted())
        return;
    PQuest->SetDiggingSpotAsHidden(DiggingSpot, false);
}

int main()
{
    CQuestScriptInterface quest;
    CV_ParanoidWhispersScript parent;
    g_parent = &parent;
    CPW_NecklaceDiggingSpot s;
    s.PQuest = &quest; s.ParentClass = &parent;
    parent.HideDiggingSpot = true; g_unhideAfterTicks = 3;
    s.Main();                                            // three ticks, then the reveal
    bool ok = g_ticks == 3 && g_hideCalls == 1 && g_hiddenThing == &s.DiggingSpot && g_hiddenValue == false && g_checks == 4;
    g_ticks = 0; g_hideCalls = 0; g_checks = 0; g_abortAtCheck = 1;
    parent.HideDiggingSpot = true;
    s.Main();                                            // aborted during the wait: no reveal
    ok = ok && g_ticks == 2 && g_hideCalls == 0;
    g_ticks = 0; g_checks = 0; g_abortAtCheck = 0;
    parent.HideDiggingSpot = false;
    s.Main();                                            // not hidden but aborted: no tick, no reveal
    ok = ok && g_ticks == 0 && g_hideCalls == 0 && g_checks == 1;
    if (!ok) {
        std::printf("00ec09e0_TEST FAIL\n");
        return 1;
    }
    std::printf("00ec09e0_TEST PASS\n");
    return 0;
}