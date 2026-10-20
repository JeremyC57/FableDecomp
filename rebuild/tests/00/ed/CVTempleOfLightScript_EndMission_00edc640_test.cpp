// Behaviour test for CV_TempleOfLightScript::EndMission (00edc640): two script waits, then hide the quest info and set QuestFinished.
#include <cstdio>
#include <cstring>

struct CCharString {
    char text[32];
    CCharString(const char* t, long) { std::strcpy(text, t); }
};

struct FakeGSI {
    int frames, regionCalls, regionTrueFor, displayCalls; bool displayArg; bool badName;
    void NewScriptFrame() { ++frames; }
    bool IsRegionLoaded(const CCharString& n) { if (std::strcmp(n.text, "Witchwood3") != 0) badName = true; return regionCalls++ < regionTrueFor; }
    void DisplayQuestInfo(bool d) { ++displayCalls; displayArg = d; }
};

struct Script {
    FakeGSI* GSI;
    bool QuestFinished, QuestComplete;
    int terminateAfter, termCalls;
    bool IsActiveThreadTerminating() { return ++termCalls > terminateAfter; }
    bool InRegion() { CCharString region("Witchwood3", -1); return GSI->IsRegionLoaded(region) ? true : false; }
    void EndMission()
    {
        while (!QuestComplete) {
            GSI->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        while (InRegion()) {
            GSI->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        GSI->DisplayQuestInfo(false);
        QuestFinished = true;
    }
};

int main()
{
    FakeGSI g = { 0, 0, 3, 0, true, false };
    Script s; s.GSI = &g; s.QuestFinished = false; s.QuestComplete = true; s.terminateAfter = 100; s.termCalls = 0;
    s.EndMission();
    bool ok = s.QuestFinished && g.frames == 3 && g.regionCalls == 4 && g.displayCalls == 1 && !g.displayArg && !g.badName;
    FakeGSI g2 = { 0, 0, 100, 0, true, false };
    Script t; t.GSI = &g2; t.QuestFinished = false; t.QuestComplete = true; t.terminateAfter = 2; t.termCalls = 0;
    t.EndMission();
    ok = ok && !t.QuestFinished && g2.displayCalls == 0 && g2.frames == 2;
    Script u; FakeGSI g3 = { 0, 0, 0, 0, true, false }; u.GSI = &g3; u.QuestFinished = false; u.QuestComplete = false; u.terminateAfter = 0; u.termCalls = 0;
    u.EndMission();
    ok = ok && !u.QuestFinished && g3.frames == 1 && g3.regionCalls == 0;
    if (!ok) { std::printf("00edc640_TEST FAIL\n"); return 1; }
    std::printf("00edc640_TEST PASS\n");
    return 0;
}