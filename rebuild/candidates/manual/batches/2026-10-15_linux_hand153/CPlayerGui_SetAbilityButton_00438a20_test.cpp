// Behaviour test for 0x00438a20: SetAbilityButton: bounded write after the lazy def load.
// The reconstructed logic runs over a host model of the retail classes and callees.
#include <cstdio>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d\n", __LINE__); } } while (0)
static unsigned long g_defCount = 0; static bool g_loaded = false; static int g_loads = 0;
static unsigned long NumButtons() { if (!g_loaded) { g_loaded = true; ++g_loads; g_defCount = 4; } return g_defCount; }
struct Gui { long buttons[4];
  long Get(long i) { if (i >= 0 && (unsigned long)i < NumButtons()) return buttons[i]; return 0; }
  void Set(long i, long b) { if (i >= 0 && (unsigned long)i < NumButtons()) buttons[i] = b; } };

static int run() { Gui g = {{0,0,0,0}}; g.Set(2, 9); g.Set(4, 1); g.Set(-1, 1); CHECK(g.buttons[2] == 9 && g.buttons[3] == 0 && g.buttons[0] == 0 && g_loads == 1); return 0; }
int main()
{
    run();
    if (g_fail) { std::printf("00438a20_TEST FAIL\n"); return 1; }
    std::printf("00438a20_TEST PASS\n");
    return 0;
}
