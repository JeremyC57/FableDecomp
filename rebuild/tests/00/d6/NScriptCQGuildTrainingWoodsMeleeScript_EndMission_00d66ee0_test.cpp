// Behaviour test for 0x00d66ee0: EndMission (guild woods melee): frame loop until MissionOver, then complete the active quest (true,false,false) and succeed.
// The reconstructed logic runs over a host model of the retail classes and callees.
#include <cstdio>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d\n", __LINE__); } } while (0)
struct GSI { int frames; int completeCalls; bool a, b, c; bool* over; int overAfter;
  void NewScriptFrame() { ++frames; if (frames >= overAfter) *over = true; }
  const char* GetActiveQuestName() const { return "Q"; }
  void SetQuestAsCompleted(const char*, bool x, bool y, bool z) { ++completeCalls; a = x; b = y; c = z; } };
struct Script { GSI* gsi; bool succeeded, failed, over; int termAfter; int termCalls;
  bool IsActiveThreadTerminating() { return ++termCalls > termAfter; }
  void EndMission() { while (!over) { gsi->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
    if (IsActiveThreadTerminating()) return; gsi->SetQuestAsCompleted(gsi->GetActiveQuestName(), true, false, false); succeeded = true; } };
static int run() {
  GSI g = {0,0,false,false,false,0,3}; Script s = {&g,false,false,false,100,0}; g.over = &s.over;
  s.EndMission(); CHECK(g.frames == 3 && s.succeeded && g.completeCalls == 1 && g.a && !g.b && !g.c);
  GSI g2 = {0,0,false,false,false,0,10}; Script s2 = {&g2,false,false,false,1,0}; g2.over = &s2.over;
  s2.EndMission(); CHECK(!s2.succeeded && g2.completeCalls == 0 && g2.frames == 2);
  return 0; }
int main()
{
    run();
    if (g_fail) { std::printf("00d66ee0_TEST FAIL\n"); return 1; }
    std::printf("00d66ee0_TEST PASS\n");
    return 0;
}