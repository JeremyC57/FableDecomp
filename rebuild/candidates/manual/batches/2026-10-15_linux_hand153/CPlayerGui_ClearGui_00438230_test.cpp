// Behaviour test for 0x00438230: ClearGui: five displays cleared, illustrators stopped and cleared, subtitles cleared.
// The reconstructed logic runs over a host model of the retail classes and callees.
#include <cstdio>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d\n", __LINE__); } } while (0)
struct D { int clears; void Clear() { ++clears; } };
struct Ill { bool drawing; int clears; void Clear() { ++clears; } };
struct Sub { int clears; bool f; void Clear(bool b) { ++clears; f = b; } };
struct Gui { D* d[5]; Ill* ill[3]; int nIll; Sub sub;
  void ClearGui() { for (int i = 0; i < 5; ++i) d[i]->Clear(); for (int i = 0; i < nIll; ++i) ill[i]->drawing = false; for (int i = 0; i < nIll; ++i) ill[i]->Clear(); sub.Clear(true); } };
static int run() { D d[5]; Ill il[3]; for (int i = 0; i < 5; ++i) d[i].clears = 0; for (int i = 0; i < 3; ++i) { il[i].drawing = true; il[i].clears = 0; }
  Gui g = {{&d[0],&d[1],&d[2],&d[3],&d[4]}, {&il[0],&il[1],&il[2]}, 3, {0,false}}; g.ClearGui();
  CHECK(d[4].clears == 1 && !il[2].drawing && il[0].clears == 1 && g.sub.clears == 1 && g.sub.f); return 0; }
int main()
{
    run();
    if (g_fail) { std::printf("00438230_TEST FAIL\n"); return 1; }
    std::printf("00438230_TEST PASS\n");
    return 0;
}
