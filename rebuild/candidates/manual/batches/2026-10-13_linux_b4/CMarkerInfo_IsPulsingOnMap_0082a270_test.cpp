#include <cstdio>

struct G { unsigned char p[4]; long a; unsigned char p2[4]; long b; unsigned char p3[0x20]; long c; long d; };
struct Def { unsigned char pad[0x958]; G* MarkerGraphics; };
static G g_g; static Def g_def;
Def* __fastcall PeekGuiDef() { return &g_def; }
struct M { unsigned char pad[8]; long GraphicIndex;
    bool Is() { G* g = PeekGuiDef()->MarkerGraphics; long x = GraphicIndex; if (x == g->a || x == g->b || x == g->c || x == g->d) return true; return false; } };
int main()
{
    g_def.MarkerGraphics = &g_g; g_g.a = 1; g_g.b = 2; g_g.c = 3; g_g.d = 4; M m;
    m.GraphicIndex = 3; if (!m.Is()) { std::printf("0082a270_TEST FAIL\n"); return 1; }
    m.GraphicIndex = 9; if (m.Is()) { std::printf("0082a270_TEST FAIL\n"); return 1; }
    std::printf("0082a270_TEST PASS\n");
    return 0;
}
