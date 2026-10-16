// Behaviour test for 0x00408900: CUserProfileManager::GetMaxResolution: last mode without a cap, otherwise the last mode within the width cap (0 if none)
// The reconstructed logic runs over a host model of the retail classes and callees.
#include <cstdio>
#include <cstring>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d\n", __LINE__); } } while (0)

struct Mode { long w, h, bpp, hz; };
static long g_cap = -1;
static long GetMax(const Mode* b, const Mode* e) { if (g_cap != -1) { long index = -1; for (const Mode* it = b; it != e; ++it) { if (it->w > g_cap) break; ++index; } if (index != -1) return index; return 0; } return (long)(e - b) - 1; }
static void run() { Mode m[4] = { { 640, 480, 32, 60 }, { 800, 600, 32, 60 }, { 1024, 768, 32, 60 }, { 1280, 1024, 32, 60 } };
  CHECK(GetMax(m, m + 4) == 3); g_cap = 1024; CHECK(GetMax(m, m + 4) == 2); g_cap = 100; CHECK(GetMax(m, m + 4) == 0); g_cap = 5000; CHECK(GetMax(m, m + 4) == 3); }

int main()
{
    run();
    if (g_fail) { std::printf("00408900_TEST FAIL\n"); return 1; }
    std::printf("00408900_TEST PASS\n");
    return 0;
}
