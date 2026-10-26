// Behaviour test for 0x00d254b0: CQ_DragonBossFightScript::Init: resets counters, copies the fly-by count, registers the region.
// The reconstructed logic runs over a host model of the retail classes and callees.
#include <cstdio>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d\n", __LINE__); } } while (0)
struct GSI { int calls; const char* q; const char* r; void AddQuestRegion(const char* a, const char* b) { ++calls; q = a; r = b; } };
struct Globals { long flyBys; };
static Globals g_glob = {7};
struct Script { GSI* gsi; long DragonState, Next, NumMinions, NumSummoners, FlyBys;
  void Init() { DragonState = 0; Next = 0; NumMinions = 0; NumSummoners = 0; FlyBys = g_glob.flyBys; gsi->AddQuestRegion("Q_DragonBossFight", "DragonCliff2"); } };
static int run() { GSI g = {0,0,0}; Script s = {&g,5,5,5,5,0}; s.Init();
  CHECK(s.DragonState == 0 && s.Next == 0 && s.NumMinions == 0 && s.NumSummoners == 0 && s.FlyBys == 7 && g.calls == 1 && g.q[0] == 'Q' && g.r[0] == 'D'); return 0; }
int main()
{
    run();
    if (g_fail) { std::printf("00d254b0_TEST FAIL\n"); return 1; }
    std::printf("00d254b0_TEST PASS\n");
    return 0;
}