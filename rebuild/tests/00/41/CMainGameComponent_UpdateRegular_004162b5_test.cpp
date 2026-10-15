// Behaviour test for 0x004162B5 (CMainGameComponent::UpdateRegular): timed update/render while active; render skipped while the movie player draws.
#include <cstdio>
static double g_clock; static bool g_appActive, g_movieActive, g_drawingExclusively;
static long g_frameCounter = 5; static double g_updateTime, g_renderTime;
static double GFGetTime() { return g_clock; }
struct Component {
    int updates, renders;
    void Update() { ++updates; g_clock += 2.0; }
    void Render() { ++renders; g_clock += 3.0; }
    void UpdateRegular()
    {
        if (g_appActive) {
            g_frameCounter = 0;
            double start = GFGetTime();
            Update();
            g_updateTime = GFGetTime() - start;
            start = GFGetTime();
            if (!g_movieActive || !g_drawingExclusively) Render();
            g_renderTime = GFGetTime() - start;
        }
    }
};
int main()
{
    Component c; c.updates = 0; c.renders = 0;
    g_appActive = false; c.UpdateRegular();
    int ok = c.updates == 0 && g_frameCounter == 5;
    g_appActive = true; c.UpdateRegular();
    ok &= c.updates == 1 && c.renders == 1 && g_updateTime == 2.0 && g_renderTime == 3.0 && g_frameCounter == 0;
    g_movieActive = true; g_drawingExclusively = true; c.UpdateRegular();
    ok &= c.updates == 2 && c.renders == 1 && g_renderTime == 0.0;
    g_drawingExclusively = false; c.UpdateRegular(); ok &= c.renders == 2;
    if (!ok) { std::printf("004162b5_TEST FAIL\n"); return 1; }
    std::printf("004162b5_TEST PASS\n");
    return 0;
}