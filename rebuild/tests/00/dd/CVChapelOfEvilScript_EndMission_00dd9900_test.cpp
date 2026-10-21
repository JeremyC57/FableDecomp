// Behaviour test for 0x00DD9900 (EndMission, prison-race step): start flag -> activate quest -> wait -> reset cells.
#include <cstdio>
#include <cstring>
// Host model: logs 'F' frame, 'A' quest activated, 'R' cells reset; the quest stays active for 'activeFrames' frames
// after activation; the start flag is raised at frame 'startAt'; terminating is a countdown of checks.
struct Game {
    char log[256]; int frames; int startAt; int activeFrames; int activatedAt; bool* start;
    Game() : frames(0), startAt(0), activeFrames(0), activatedAt(-1000), start(0) { log[0] = 0; }
    void Add(char c) { size_t n = std::strlen(log); if (n < 250) { log[n] = c; log[n + 1] = 0; } }
    void NewScriptFrame() { Add('F'); ++frames; if (frames == startAt) *start = true; }
    void ActivateQuest(const char* q) { Add(std::strcmp(q, "Q_PrisonRace") == 0 ? 'A' : '?'); activatedAt = frames; }
    bool IsQuestActive(const char* q) { return std::strcmp(q, "Q_PrisonRace") == 0 && frames - activatedAt < activeFrames; }
};
struct Script {
    Game* GSI; bool StartRequested; bool Skip; int termAfter; int checks;
    Script(Game* g, bool skip, int after) : GSI(g), StartRequested(false), Skip(skip), termAfter(after), checks(0) { g->start = &StartRequested; }
    bool IsActiveThreadTerminating() { return ++checks > termAfter; }
    void ResetCells() { GSI->Add('R'); }
    void EndMission()
    {
        if (Skip) { if (IsActiveThreadTerminating()) return; return; }
        while (!IsActiveThreadTerminating()) {
            while (!StartRequested) { GSI->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
            if (IsActiveThreadTerminating()) return;
            StartRequested = false;
            GSI->ActivateQuest("Q_PrisonRace");
            GSI->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
            while (!(!GSI->IsQuestActive("Q_PrisonRace"))) { GSI->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
            if (IsActiveThreadTerminating()) return;
            ResetCells();
            GSI->NewScriptFrame();
        }
    }
};
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d: %s\n", __LINE__, #c); } } while (0)
int main()
{
    { Game g; Script s(&g, true, 50); s.EndMission(); CHECK(g.log[0] == 0 && s.checks == 1); }
    // start at frame 2, quest active for 3 frames after activation
    { Game g; g.startAt = 2; g.activeFrames = 3; Script s(&g, false, 10); s.EndMission();
      CHECK(std::strcmp(g.log, "FFAFFFRFFF") == 0); CHECK(!s.StartRequested); }
    if (g_fail) { std::printf("00dd9900_TEST FAIL\n"); return 1; }
    std::printf("00dd9900_TEST PASS\n");
    return 0;
}