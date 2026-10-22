// Behaviour test for 0x00EEA460: the call sequence of CExpression_WaitScript::Main on early termination, scripting retries and the full run.
#include <cstdio>
#include <cstring>
static char g_log[2048];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct CRGBColour {
    unsigned char B, G, R, A;
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};
struct CScriptThing { int id; };
struct CGame {
    int startFailures;
    CScriptThing hero;
    void NewScriptFrame() { Log("frame"); }
    const CScriptThing& GetHero() { Log("hero"); return hero; }
    bool StartScriptingEntity(const CScriptThing& t, int priority) { Log(t.id == 7 && priority == 4 ? "start4" : "startBad"); if (startFailures > 0) { --startFailures; return false; } return true; }
    void StartMovieSequence(const char* name) { Log(name[0] == 0 ? "movie" : "movieBad"); }
    void FadeScreenOut(float a, float b, CRGBColour c) { Log(a == 0.5f && b == 0.5f && c.R == 0 && c.G == 0 && c.B == 0 && c.A == 255 ? "fadeout" : "fadeoutBad"); }
    void FadeScreenIn() { Log("fadein"); }
    void Pause(float s) { Log(s == 1.0f ? "pause1" : "pauseBad"); }
    void DeactivateQuest(const char* n, unsigned int f) { Log(std::strcmp(n, "Expression_Follow") == 0 && f == 0 ? "deact" : "deactBad"); }
    void DeactivateQuestLater(const char* n, unsigned int f) { Log(std::strcmp(n, "ACTIVE") == 0 && f == 0 ? "deactLater" : "deactLaterBad"); }
    void CameraDefault() { Log("camera"); }
    const char* GetActiveQuestName() { Log("name"); return "ACTIVE"; }
};
struct CScript {
    CGame* GSI;
    int terminateAt;   // IsActiveThreadTerminating returns true on this call number (1-based); 0 = never
    int calls;
    bool IsActiveThreadTerminating() { ++calls; Log("term?"); return calls == terminateAt; }
    void SetPredicate(const CScriptThing& t) { Log(t.id == 7 ? "pred" : "predBad"); }
    void KillSpawnedFunction(const char* n, unsigned int id) { Log(std::strcmp(n, "FollowThread") == 0 && id == 0 ? "kill" : "killBad"); }
    void Main()
    {
        SetPredicate(GSI->GetHero());
        GSI->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        Log("handle");
        while (!GSI->StartScriptingEntity(GSI->GetHero(), 4)) {
            GSI->NewScriptFrame();
            if (IsActiveThreadTerminating()) { Log("~handle"); return; }
        }
        if (IsActiveThreadTerminating()) { Log("~handle"); return; }
        GSI->StartMovieSequence("");
        GSI->FadeScreenOut(0.5f, 0.5f, CRGBColour(0, 0, 0, 255));
        GSI->Pause(1.0f);
        KillSpawnedFunction("FollowThread", 0);
        GSI->DeactivateQuest("Expression_Follow", 0);
        GSI->CameraDefault();
        GSI->Pause(1.0f);
        GSI->FadeScreenIn();
        GSI->DeactivateQuestLater(GSI->GetActiveQuestName(), 0);
        Log("~movie;~handle");
    }
};
static int Run(int failures, int terminateAt, const char* expect)
{
    CGame game; game.startFailures = failures; game.hero.id = 7;
    CScript s; s.GSI = &game; s.terminateAt = terminateAt; s.calls = 0;
    g_log[0] = 0;
    s.Main();
    if (std::strcmp(g_log, expect) != 0) { std::printf("got %s\nexp %s\n", g_log, expect); return 1; }
    return 0;
}
int main()
{
    int bad = 0;
    bad += Run(0, 1, "hero;pred;frame;term?;");
    bad += Run(2, 2, "hero;pred;frame;term?;handle;hero;start4;frame;term?;~handle;");
    bad += Run(1, 0, "hero;pred;frame;term?;handle;hero;start4;frame;term?;hero;start4;term?;movie;fadeout;pause1;kill;deact;camera;pause1;fadein;name;deactLater;~movie;~handle;");
    bad += Run(0, 2, "hero;pred;frame;term?;handle;hero;start4;term?;~handle;");
    if (bad) { std::printf("00eea460_TEST FAIL\n"); return 1; }
    std::printf("00eea460_TEST PASS\n");
    return 0;
}
