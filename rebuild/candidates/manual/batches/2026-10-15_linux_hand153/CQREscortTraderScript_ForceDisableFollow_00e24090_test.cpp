// Behaviour test for 0x00e24090: ForceDisableFollow: flag cleared, timer reset, five interface calls in order.
// The reconstructed logic runs over a host model of the retail classes and callees.
#include <cstdio>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d\n", __LINE__); } } while (0)
struct GSI { int log[8]; int n; void rec(int v) { log[n++] = v; }
  void SetTimer(long, long) { rec(1); } void MiniMapRemoveMarker(int) { rec(2); } void EntityStopFollowing(int) { rec(3); }
  void Emote(int, bool b) { rec(b ? 40 : 4); } int GetHero() { rec(5); return 99; } void RegionFollow(int h, int t, bool f) { rec(h == 99 && t == 7 && !f ? 6 : 60); } };
struct Script { GSI* gsi; bool follow; long timer;
  void Force(int trader) { follow = false; gsi->SetTimer(timer, 10); gsi->MiniMapRemoveMarker(trader); gsi->EntityStopFollowing(trader); gsi->Emote(trader, false); gsi->RegionFollow(gsi->GetHero(), trader, false); } };
static int run() { GSI g; g.n = 0; Script s = {&g, true, 3}; s.Force(7);
  CHECK(!s.follow && g.n == 6 && g.log[0] == 1 && g.log[1] == 2 && g.log[2] == 3 && g.log[3] == 4 && g.log[4] == 5 && g.log[5] == 6); return 0; }
int main()
{
    run();
    if (g_fail) { std::printf("00e24090_TEST FAIL\n"); return 1; }
    std::printf("00e24090_TEST PASS\n");
    return 0;
}
