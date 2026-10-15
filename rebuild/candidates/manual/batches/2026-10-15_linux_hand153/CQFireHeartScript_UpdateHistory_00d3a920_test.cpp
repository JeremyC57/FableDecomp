// Behaviour test for 0x00d3a920: UpdateHistory: good/bad history state machine.
// The reconstructed logic runs over a host model of the retail classes and callees.
#include <cstdio>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d\n", __LINE__); } } while (0)
struct Script { long Guest; bool term;
  bool Term() { return term; }
  void UpdateHistory(bool good) {
    if (good) { if (Term()) return; if (Guest == 0 || Guest == 3) { if (Term()) return; Guest = 3; } else { if (Term()) return; Guest = 2; } }
    else { if (Term()) return; if (Guest == 0 || Guest == 1) { if (Term()) return; Guest = 1; } else { if (Term()) return; Guest = 2; } } } };
static int run() { Script s = {0,false};
  s.UpdateHistory(true); CHECK(s.Guest == 3); s.UpdateHistory(true); CHECK(s.Guest == 3); s.UpdateHistory(false); CHECK(s.Guest == 2);
  s.Guest = 0; s.UpdateHistory(false); CHECK(s.Guest == 1); s.UpdateHistory(true); CHECK(s.Guest == 2);
  s.term = true; s.Guest = 0; s.UpdateHistory(true); CHECK(s.Guest == 0); return 0; }
int main()
{
    run();
    if (g_fail) { std::printf("00d3a920_TEST FAIL\n"); return 1; }
    std::printf("00d3a920_TEST PASS\n");
    return 0;
}
