#include <cstdio>
#include <cstring>
#include <cmath>

static bool g_visible; static int g_peek = 0; void* PeekGuiDef() { g_peek++; return 0; }
struct CDrawMouseCursor { unsigned char p[0x18]; bool Drawing; void Draw(long) { if (g_visible) { Drawing = true; PeekGuiDef(); } } };
int main() {
    CDrawMouseCursor c; c.Drawing = false; g_visible = false; c.Draw(0); if (c.Drawing || g_peek) { std::printf("006571d0_TEST FAIL\n"); return 1; }
    g_visible = true; c.Draw(0); if (!c.Drawing || g_peek != 1) { std::printf("006571d0_TEST FAIL\n"); return 1; }
    std::printf("006571d0_TEST PASS\n"); return 0;
}