// Behaviour test for 0x00877d20: CTools::FrameUpdate: the scan box is the camera position +/- 10 and the scan list is rebuilt
// The reconstructed logic runs over a host model of the retail classes and callees.
#include <cstdio>
#include <cstring>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d\n", __LINE__); } } while (0)

struct Box { long x0, y0, x1, y1; };
static Box g_box; static int g_cleared = 0;
static void FrameUpdate(float x, float y) { long ix = (long)x; long iy = (long)y; Box b = { ix - 10, iy - 10, ix + 10, iy + 10 }; ++g_cleared; g_box = b; }
static void run() { FrameUpdate(100.7f, -20.2f); CHECK(g_box.x0 == 90 && g_box.y0 == -30 && g_box.x1 == 110 && g_box.y1 == -10 && g_cleared == 1); }

int main()
{
    run();
    if (g_fail) { std::printf("00877d20_TEST FAIL\n"); return 1; }
    std::printf("00877d20_TEST PASS\n");
    return 0;
}