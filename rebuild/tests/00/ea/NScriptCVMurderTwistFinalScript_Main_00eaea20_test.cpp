// Behaviour test for NScript::CV_MurderTwistFinalScript::Main 0x00EAEA20: the four entity-script bindings, both
// PostAddScriptedEntities calls, the wait for sheriff state 3 or 4, SetQuestAsCompleted(V_MurderTwistFinal), the OakBay
// unload wait only in state 3, then DeactivateQuestLater(active quest, 1); plus the early exits on thread termination.
#include <cstdio>
#include <cstring>
static char g_log[1024];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct Script;
struct Game {
    Script* owner;
    int framesUntilState, finalState, framesUntilUnloaded;
    void NewScriptFrame();
    bool IsLevelLoaded(const char* name) { if (std::strcmp(name, "OakBay") != 0) return false; return framesUntilUnloaded-- > 0; }
    void PostAddScriptedEntities() { Log("GamePost"); }
    void SetQuestAsCompleted(const char* name, bool a, bool b, bool c) { char t[64]; std::sprintf(t, "Completed(%s,%d,%d,%d)", name, a, b, c); Log(t); }
    const char* GetActiveQuestName() const { return "V_MurderTwistFinal"; }
    void DeactivateQuestLater(const char* name, unsigned long delay) { char t[64]; std::sprintf(t, "Deactivate(%s,%lu)", name, delay); Log(t); }
};
struct Script {
    Game* Game_;
    int SheriffState;
    int terminateAt, calls;
    bool IsActiveThreadTerminating() { ++calls; return calls == terminateAt; }
    void AddEntityScriptBinding(const char* name) { Log(name); }
    void PostAddScriptedEntities() { Log("Post"); }
    void Main()
    {
        AddEntityScriptBinding("MurderTwistSheriff");
        AddEntityScriptBinding("MurderTwistSheriffGuard");
        AddEntityScriptBinding("MurderTwistAssassin");
        AddEntityScriptBinding("MurderTwistAssassinGuard");
        PostAddScriptedEntities();
        Game_->PostAddScriptedEntities();
        while (SheriffState != 4 && SheriffState != 3) {
            Game_->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        Game_->SetQuestAsCompleted("V_MurderTwistFinal", false, false, false);
        if (SheriffState == 3) {
            if (IsActiveThreadTerminating()) return;
            while (!(!Game_->IsLevelLoaded("OakBay"))) {
                Game_->NewScriptFrame();
                if (IsActiveThreadTerminating()) return;
            }
            if (IsActiveThreadTerminating()) return;
        }
        Game_->DeactivateQuestLater(Game_->GetActiveQuestName(), 1);
    }
};
void Game::NewScriptFrame() { Log("Frame"); if (--framesUntilState == 0) owner->SheriffState = finalState; }
static bool Run(int state0, int framesUntilState, int finalState, int framesUntilUnloaded, int terminateAt, const char* expect)
{
    g_log[0] = 0;
    Script s; Game g;
    s.Game_ = &g; s.SheriffState = state0; s.terminateAt = terminateAt; s.calls = 0;
    g.owner = &s; g.framesUntilState = framesUntilState; g.finalState = finalState; g.framesUntilUnloaded = framesUntilUnloaded;
    s.Main();
    if (std::strcmp(g_log, expect) != 0) { std::printf("got %s\n", g_log); return false; }
    return true;
}
int main()
{
    const char* head = "MurderTwistSheriff;MurderTwistSheriffGuard;MurderTwistAssassin;MurderTwistAssassinGuard;Post;GamePost;";
    const char* done = "Completed(V_MurderTwistFinal,0,0,0);";
    const char* tail = "Deactivate(V_MurderTwistFinal,1);";
    char s4[512], s3[512], s3now[512], term1[512], term3[512];
    std::sprintf(s4, "%sFrame;Frame;%s%s", head, done, tail);                 // state 4 after two frames: no OakBay wait
    std::sprintf(s3, "%sFrame;%sFrame;Frame;%s", head, done, tail);           // state 3: wait two frames for OakBay to unload
    std::sprintf(s3now, "%s%s%s", head, done, tail);                          // already state 3, OakBay not loaded
    std::sprintf(term1, "%sFrame;", head);                                    // terminated inside the first wait
    std::sprintf(term3, "%s%s", head, done);                                  // terminated right before the OakBay wait
    if (!Run(0, 2, 4, 5, 0, s4) || !Run(1, 1, 3, 2, 0, s3) || !Run(3, 0, 0, 0, 0, s3now)
        || !Run(0, 3, 4, 0, 1, term1) || !Run(3, 0, 0, 2, 2, term3)) {
        std::printf("00eaea20_TEST FAIL\n");
        return 1;
    }
    std::printf("00eaea20_TEST PASS\n");
    return 0;
}