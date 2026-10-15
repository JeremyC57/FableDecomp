// Behaviour test for 0x00f0b780: PhaseOne (ambush): wait for the region.
// The reconstructed logic runs over a host model of the retail classes and callees.
#include <cstdio>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d\n", __LINE__); } } while (0)
struct GSI { int frames; int loadedAt; int queries; int extra;
  bool IsLoaded(const char*) { ++queries; return frames >= loadedAt; }
  void NewScriptFrame() { ++frames; } };
struct Script { GSI* gsi; int termAfter, termCalls; int next;
  bool Term() { return ++termCalls > termAfter; }
  bool Missing() { return gsi->IsLoaded("x") ? false : true; }
  void PhaseOne() { while (Missing()) { gsi->NewScriptFrame(); if (Term()) return; } if (Term()) return; ; } };
static int run() {
  GSI g = {0,4,0,0}; Script s = {&g,100,0,0}; s.PhaseOne(); CHECK(g.frames == 4 && s.termCalls == 5 && s.next == 0);
  GSI g2 = {0,10,0,0}; Script s2 = {&g2,2,0,0}; s2.PhaseOne(); CHECK(g2.frames == 3 && s2.next == 0);
  return 0; }
int main()
{
    run();
    if (g_fail) { std::printf("00f0b780_TEST FAIL\n"); return 1; }
    std::printf("00f0b780_TEST PASS\n");
    return 0;
}
