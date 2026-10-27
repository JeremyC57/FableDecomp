// behaviour test for 0x00D293F0 CGatewayUndead::Main() (model: one predicate registered, frames until the thread terminates)
#include <cstdio>
static int g_frames = 0, g_preds = 0, g_termAfter = 3;
struct Game { void NewScriptFrame() const { ++g_frames; } };
struct Script {
    Game* G;
    void SetPredicate(int) { ++g_preds; }
    bool IsActiveThreadTerminating() const { return g_frames >= g_termAfter; }
    void Main()
    {
        { SetPredicate(1); }
        G->NewScriptFrame();
        while (!IsActiveThreadTerminating())
            G->NewScriptFrame();
    }
};
int main()
{
    Game g; Script s; s.G = &g;
    s.Main();
    int ok = g_preds == 1 && g_frames == 3;
    g_frames = 0; g_termAfter = 0; s.Main();
    ok = ok && g_frames == 1 && g_preds == 2;
    if (!ok) { std::printf("00d293f0_TEST FAIL\n"); return 1; }
    std::printf("00d293f0_TEST PASS\n");
    return 0;
}