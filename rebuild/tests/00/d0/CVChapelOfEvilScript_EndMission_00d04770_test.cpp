// Behaviour test for 0x00D04770 (EndMission, bandit-camp gate): open Gate2Inner while BanditCampCentre is loaded.
#include <cstdio>
#include <cstring>
// Host model: the region is loaded during frames [loadFrom, unloadFrom); the game logs 'F' frame, 'O' door opened
// (only when the thing fetched by name is Gate2Inner); terminating is a countdown of checks.
struct Thing { bool gate; };
struct Game {
    char log[256]; int frames; int loadFrom; int unloadFrom; int liveThings;
    Game(int a, int b) : frames(0), loadFrom(a), unloadFrom(b), liveThings(0) { log[0] = 0; }
    void Add(char c) { size_t n = std::strlen(log); if (n < 250) { log[n] = c; log[n + 1] = 0; } }
    void NewScriptFrame() { Add('F'); ++frames; }
    bool IsRegionLoaded(const char* r) { return std::strcmp(r, "BanditCampCentre") == 0 && frames >= loadFrom && frames < unloadFrom; }
    Thing GetThingWithScriptName(const char* n) { Thing t; t.gate = std::strcmp(n, "Gate2Inner") == 0; return t; }
    void OpenDoor(const Thing& t) { Add(t.gate ? 'O' : '?'); }
};
struct Handle {                       // counts live handles to check every path releases the gate
    Game* g; Thing t;
    Handle(Game* game, const Thing& thing) : g(game), t(thing) { ++g->liveThings; }
    ~Handle() { --g->liveThings; }
};
struct Script {
    Game* GSI; int termAfter; int checks;
    Script(Game* g, int after) : GSI(g), termAfter(after), checks(0) {}
    bool IsActiveThreadTerminating() { return ++checks > termAfter; }
    void EndMission()
    {
        while (!IsActiveThreadTerminating()) {
            if (GSI->IsRegionLoaded("BanditCampCentre")) {
                if (IsActiveThreadTerminating()) return;
                Handle gate(GSI, GSI->GetThingWithScriptName("Gate2Inner"));
                GSI->OpenDoor(gate.t);
                while (!(!GSI->IsRegionLoaded("BanditCampCentre"))) {
                    GSI->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return;
                }
                if (IsActiveThreadTerminating()) return;
            }
            GSI->NewScriptFrame();
        }
    }
};
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d: %s\n", __LINE__, #c); } } while (0)
int main()
{
    { Game g(100, 200); Script s(&g, 3); s.EndMission(); CHECK(std::strcmp(g.log, "FFF") == 0); }
    // loaded from frame 2 to 5: wait two frames, open, wait until unloaded, continue
    { Game g(2, 5); Script s(&g, 12); s.EndMission(); CHECK(std::strcmp(g.log, "FFOFFFFFFFF") == 0); CHECK(g.liveThings == 0); }
    // terminating inside the wait still releases the gate
    { Game g(0, 50); Script s(&g, 4); s.EndMission(); CHECK(std::strcmp(g.log, "OFFF") == 0); CHECK(g.liveThings == 0); }
    if (g_fail) { std::printf("00d04770_TEST FAIL\n"); return 1; }
    std::printf("00d04770_TEST PASS\n");
    return 0;
}