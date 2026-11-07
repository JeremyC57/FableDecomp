// Behaviour test for NScript::CQ_BowerstoneTownLifeIntroScript::Main (0x00D23DC0): a copy of the Main logic over a mock game
// that records the calls; checks the binding order, the region-exit / guard handling, the waits and the final deactivation.
#include <cstdio>
#include <cstring>

struct Thing { int id; bool alive; bool IsAlive() const { return id != 0 && alive; } };

struct MockGame {
    char log[64][40];
    int n;
    int frames;
    int regionLoadedFrames;      // frames until BowerstoneSlums unloads
    bool terminateAfterFrames;
    int terminateAt;
    struct Script* script;
    MockGame() : n(0), frames(0), regionLoadedFrames(0), terminateAfterFrames(false), terminateAt(0), script(0) { }
    void Log(const char* s) { if (n < 64) { std::strncpy(log[n], s, 39); log[n][39] = 0; ++n; } }
    void NewScriptFrame();
    void PostAddScriptedEntities() { Log("GamePost"); }
    Thing GetThingWithScriptName(const char* name) {
        Log(name);
        Thing t; t.id = 1; t.alive = std::strcmp(name, "BowerstoneIntroGuardToPosh") != 0; return t;
    }
    void SetRegionExitAsActive(const Thing&, bool on) { Log(on ? "ExitOn" : "ExitOff"); }
    void RemoveThing(const Thing&, bool, bool) { Log("Remove"); }
    void FadeScreenIn() { Log("Fade"); }
    bool IsRegionLoaded(const char*) { return frames < regionLoadedFrames; }
    void DeactivateQuestLater(const char* q, unsigned long) { Log(q); }
};

struct Script {
    MockGame* Game;
    bool ArenaFinished, DoneGuardSpeech, PoshGuardComplete;
    int bindings;
    bool IsActiveThreadTerminating() const { return Game->terminateAfterFrames && Game->frames >= Game->terminateAt; }
    void AddEntityScriptBinding(const char* name) { Game->Log(name); ++bindings; }
    void PostAddScriptedEntities() { Game->Log("Post"); }
    void Main();
};

void MockGame::NewScriptFrame()
{
    ++frames;
    if (frames == 2) script->DoneGuardSpeech = true;
    if (frames == 4) script->ArenaFinished = true;
    if (frames == 6) script->PoshGuardComplete = true;
}

void Script::Main()
{
    AddEntityScriptBinding("BowerstoneIntroGuard");
    AddEntityScriptBinding("BowerstoneIntroGuardToPosh");
    PostAddScriptedEntities();
    Game->PostAddScriptedEntities();
    Thing RegionExit = Game->GetThingWithScriptName("BowerstoneIntroRegionExit");
    if (RegionExit.IsAlive()) {
        if (IsActiveThreadTerminating()) return;
        Game->SetRegionExitAsActive(RegionExit, false);
    }
    Thing Guard; Guard.id = 0; Guard.alive = false;
    while (!DoneGuardSpeech) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
    if (IsActiveThreadTerminating()) return;
    Guard = Game->GetThingWithScriptName("BowerstoneIntroGuard");
    if (Guard.IsAlive()) {
        if (IsActiveThreadTerminating()) return;
        Game->RemoveThing(Guard, false, true);
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    Game->FadeScreenIn();
    while (!ArenaFinished) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
    if (IsActiveThreadTerminating()) return;
    while (!PoshGuardComplete) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
    if (IsActiveThreadTerminating()) return;
    Guard = Game->GetThingWithScriptName("BowerstoneIntroGuardToPosh");
    if (Guard.IsAlive()) {
        if (IsActiveThreadTerminating()) return;
        Game->RemoveThing(Guard, false, true);
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    Game->FadeScreenIn();
    if (RegionExit.IsAlive()) {
        if (IsActiveThreadTerminating()) return;
        Game->SetRegionExitAsActive(RegionExit, true);
    }
    while (!PoshGuardComplete || !DoneGuardSpeech || Game->IsRegionLoaded("BowerstoneSlums")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    Game->DeactivateQuestLater("Q_BowerstoneTownLifeIntro", 0);
}

static int Fail() { std::printf("00d23dc0_TEST FAIL\n"); return 1; }

int main()
{
    static const char* expect[] = { "BowerstoneIntroGuard", "BowerstoneIntroGuardToPosh", "Post", "GamePost",
        "BowerstoneIntroRegionExit", "ExitOff", "BowerstoneIntroGuard", "Remove", "Fade", "BowerstoneIntroGuardToPosh",
        "Fade", "ExitOn", "Q_BowerstoneTownLifeIntro" };
    MockGame g; Script s; s.Game = &g; g.script = &s; s.ArenaFinished = s.DoneGuardSpeech = s.PoshGuardComplete = false; s.bindings = 0;
    g.regionLoadedFrames = 9;
    s.Main();
    if (g.n != (int)(sizeof(expect) / sizeof(expect[0]))) return Fail();
    for (int i = 0; i < g.n; ++i) if (std::strcmp(g.log[i], expect[i]) != 0) return Fail();
    if (g.frames != 9) return Fail();
    // terminating during the first wait: only the bindings, the posts, the region exit fetch and close happen
    MockGame g2; Script s2; s2.Game = &g2; g2.script = &s2; s2.ArenaFinished = s2.DoneGuardSpeech = s2.PoshGuardComplete = false; s2.bindings = 0;
    g2.terminateAfterFrames = true; g2.terminateAt = 1;
    s2.Main();
    if (g2.n != 6 || g2.frames != 1 || s2.bindings != 2) return Fail();
    std::printf("00d23dc0_TEST PASS\n");
    return 0;
}