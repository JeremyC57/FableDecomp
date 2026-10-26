// Behaviour test for 0x00f03560: WatchForBoastSuccess: record boasts once guards/traders are dead.
// The reconstructed logic runs over a host model of the retail classes and callees.
#include <cstdio>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d\n", __LINE__); } } while (0)
struct Rec { bool guards, traders; };
struct Script; struct GSI { Script* s; int frames; void NewScriptFrame(); };
struct Script { GSI* gsi; Rec* owner; bool tradersDead, guardsDead; int termAfter, termCalls;
  bool Term() { return ++termCalls > termAfter; }
  void Watch() { while (!Term()) {
      if (guardsDead && !owner->guards) { if (Term()) return; owner->guards = true; }
      if (tradersDead && !owner->traders) { if (Term()) return; owner->traders = true; }
      gsi->NewScriptFrame(); } } };
void GSI::NewScriptFrame() { ++frames; if (frames == 2) s->guardsDead = true; if (frames == 4) s->tradersDead = true; }
static int run() { Rec r = {false,false}; GSI g = {0,0}; Script s = {&g,&r,false,false,20,0}; g.s = &s; s.Watch();
  CHECK(r.guards && r.traders);
  Rec r2 = {false,false}; GSI g2 = {0,0}; Script s2 = {&g2,&r2,false,true,0,0}; g2.s = &s2; s2.Watch(); CHECK(!r2.guards);
  return 0; }
int main()
{
    run();
    if (g_fail) { std::printf("00f03560_TEST FAIL\n"); return 1; }
    std::printf("00f03560_TEST PASS\n");
    return 0;
}