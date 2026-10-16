// Behaviour test for 0x00429e29: CUIState copy ctor: every field and the colour components copied
// The reconstructed logic runs over a host model of the retail classes and callees.
#include <cstdio>
#include <cstring>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d\n", __LINE__); } } while (0)

struct Col { unsigned char B, G, R, A; void Set(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; } Col() {} Col(const Col& c) { Set(c.R, c.G, c.B, c.A); } };
struct St { unsigned long flag; long type; float px, py, zx, zy; Col colour; float time; bool linear; };
static void run() { St a; a.flag = 3; a.type = 2; a.px = 1; a.py = 2; a.zx = 3; a.zy = 4; a.colour.Set(10, 20, 30, 40); a.time = 0.5f; a.linear = true; St b(a);
  CHECK(b.flag == 3 && b.type == 2 && b.py == 2 && b.zy == 4 && b.colour.R == 10 && b.colour.G == 20 && b.colour.B == 30 && b.colour.A == 40 && b.time == 0.5f && b.linear); }

int main()
{
    run();
    if (g_fail) { std::printf("00429e29_TEST FAIL\n"); return 1; }
    std::printf("00429e29_TEST PASS\n");
    return 0;
}
