// Behaviour test for 0x00f02100: EndMission (cinema test): as above with (true,true,false).
// The reconstructed logic runs over a host model of the retail classes and callees.
#include <cstdio>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d\n", __LINE__); } } while (0)
struct GSI { int completeCalls; bool a, b, c;
  void NewScriptFrame() { }
  void SetQuestAsCompleted(bool x, bool y, bool z) { ++completeCalls; a = x; b = y; c = z; } };
struct Script { GSI* gsi; bool succeeded, over; int termAfter, termCalls;
  bool Term() { return ++termCalls > termAfter; }
  void EndMission() { while (!over) { gsi->NewScriptFrame(); if (Term()) return; } if (Term()) return; gsi->SetQuestAsCompleted(true, true, false); succeeded = true; } };
static int run() {
  GSI g = {0,false,false,false}; Script s = {&g,false,true,5,0}; s.EndMission();
  CHECK(s.succeeded && g.a && g.b && !g.c && g.completeCalls == 1);
  Script s2 = {&g,false,true,0,0}; s2.EndMission(); CHECK(!s2.succeeded);
  return 0; }
int main()
{
    run();
    if (g_fail) { std::printf("00f02100_TEST FAIL\n"); return 1; }
    std::printf("00f02100_TEST PASS\n");
    return 0;
}