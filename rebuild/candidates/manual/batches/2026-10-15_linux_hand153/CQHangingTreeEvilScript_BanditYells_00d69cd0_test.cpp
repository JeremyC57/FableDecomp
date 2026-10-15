// Behaviour test for 0x00d69cd0: BanditYells: pause 5 s and clear the flag each time it is set.
// The reconstructed logic runs over a host model of the retail classes and callees.
#include <cstdio>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d\n", __LINE__); } } while (0)
struct Script; struct GSI { Script* s; int frames; int pauses; float last; void NewScriptFrame(); void Pause(float t) { ++pauses; last = t; } };
struct Script { GSI* gsi; bool yelled; int termAfter, termCalls;
  bool Term() { return ++termCalls > termAfter; }
  void BanditYells() { while (!Term()) { while (!yelled) { gsi->NewScriptFrame(); if (Term()) return; } if (Term()) return; gsi->Pause(5.0f); yelled = false; gsi->NewScriptFrame(); } } };
void GSI::NewScriptFrame() { ++frames; if (frames % 3 == 0) s->yelled = true; }
static int run() { GSI g = {0,0,0,0.0f}; Script s = {&g,false,12,0}; g.s = &s; s.BanditYells();
  CHECK(g.pauses >= 2 && g.last == 5.0f && s.termCalls == 13); return 0; }
int main()
{
    run();
    if (g_fail) { std::printf("00d69cd0_TEST FAIL\n"); return 1; }
    std::printf("00d69cd0_TEST PASS\n");
    return 0;
}
