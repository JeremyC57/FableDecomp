// Behaviour test for 0x00499c50: DisplayProgress: throttled render and last-tag bookkeeping.
// The reconstructed logic runs over a host model of the retail classes and callees.
#include <cstdio>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d\n", __LINE__); } } while (0)
#include <cstring>
static bool g_on = true, g_sup = false; static float g_eps = 0.0001f; static double g_now = 0.0;
struct PD { const char* last; double LastUpdateTime; bool Initialised; int renders;
  void Render() { ++renders; }
  void Display(const char* tag) { if (g_on && !g_sup && Initialised) { if (g_eps > LastUpdateTime || g_now - LastUpdateTime > 0.05) { Render(); LastUpdateTime = g_now; } if (std::strcmp(tag, last)) last = tag; } } };
static int run() { PD p = {"", 0.0, true, 0}; g_now = 1.0; p.Display("A"); CHECK(p.renders == 1 && p.LastUpdateTime == 1.0 && !std::strcmp(p.last, "A"));
  g_now = 1.01; p.Display("B"); CHECK(p.renders == 1 && !std::strcmp(p.last, "B"));
  g_now = 1.2; p.Display("B"); CHECK(p.renders == 2);
  g_sup = true; g_now = 5; p.Display("C"); CHECK(p.renders == 2 && !std::strcmp(p.last, "B")); return 0; }
int main()
{
    run();
    if (g_fail) { std::printf("00499c50_TEST FAIL\n"); return 1; }
    std::printf("00499c50_TEST PASS\n");
    return 0;
}
