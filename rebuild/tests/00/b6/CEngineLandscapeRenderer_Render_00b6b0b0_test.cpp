// behaviour test for 00b6b0b0 CEngineLandscapeRenderer::Render
#include <cstdio>
static int g_log[64]; static int g_n;
static void Log(int v) { if (g_n < 64) g_log[g_n++] = v; }
struct Shader { bool VS, PS; long Layout;
    void SetVS(bool e, bool* old) { if (old) *old = VS; if (e != VS) VS = e; }
    void SetPS(bool e, bool* old) { if (old) *old = PS; if (e != PS) PS = e; } };
static Shader g_sh;
struct Scope { bool oldV, oldP; Scope() { g_sh.SetVS(true, &oldV); g_sh.SetPS(true, &oldP); } ~Scope() { g_sh.SetVS(oldV, 0); g_sh.SetPS(oldP, 0); } };
static int g_depth, g_maxDepth;
static void Create() { ++g_depth; if (g_depth > g_maxDepth) g_maxDepth = g_depth; }
static void Restore() { --g_depth; }
struct Renderer {
    bool fore, back; int nMaps;
    void Render(long pass)
    {
        Scope s;
        Create();
        switch (pass) {
        case 0x40:
            Log(1);
            if (fore) { Create(); Log(2); long layout = g_sh.Layout; g_sh.Layout = 2; for (int i = 0; i < nMaps; ++i) Log(10 + i); if (g_sh.Layout != 2) Log(-1); g_sh.Layout = layout; Restore(); }
            Log(9);
            break;
        case 4:
            Log(3);
            if (back) { Create(); Log(4); for (int i = 0; i < nMaps; ++i) Log(20 + i); Restore(); }
            Log(9);
            break;
        }
        if (!g_sh.VS || !g_sh.PS) Log(-2);
        Restore();
    }
};
int main()
{
    int bad = 0;
    Renderer r = { true, false, 2 };
    g_sh.VS = false; g_sh.PS = true; g_sh.Layout = 7;
    r.Render(0x40);
    int e1[] = { 1, 2, 10, 11, 9 };
    if (g_n != 5) bad |= 1; else for (int k = 0; k < 5; ++k) if (g_log[k] != e1[k]) bad |= 2;
    if (g_sh.VS || !g_sh.PS || g_sh.Layout != 7 || g_depth != 0 || g_maxDepth != 2) bad |= 4;
    g_n = 0; r.Render(4);
    if (g_n != 2 || g_log[0] != 3 || g_log[1] != 9) bad |= 8;
    g_n = 0; r.Render(1);
    if (g_n != 0 || g_depth != 0) bad |= 16;
    if (bad) { std::printf("00b6b0b0_TEST FAIL\n"); return 1; }
    std::printf("00b6b0b0_TEST PASS\n");
    return 0;
}