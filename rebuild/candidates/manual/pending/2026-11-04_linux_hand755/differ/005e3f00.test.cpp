// Behaviour test for CTCQuestCompletionUI::DrawPreparePrimitivesForRendering (0x005E3F00), g++ model of the parked source.
#include <cstdio>
#include <cmath>
struct UI { bool active, quitting; double lastRender; float accumFade; int blackUpdates, componentUpdates; float lastAlpha; int components; };
static double g_time;
static void Draw(UI& u, float fadeTime)
{
    if (u.active && !u.quitting) {
        float dt = (float)(g_time - u.lastRender);
        u.lastRender = g_time;
        ++u.blackUpdates;
        for (int i = 0; i < u.components; ++i) ++u.componentUpdates;
        u.accumFade += dt;
        float alpha = u.accumFade / fadeTime;
        if (alpha > 1.0f) alpha = 1.0f;
        u.lastAlpha = alpha;
    }
}
int main()
{
    UI u = { true, false, 1.0, 0.0f, 0, 0, -1.0f, 3 };
    g_time = 1.5; Draw(u, 2.0f);
    int ok = u.blackUpdates == 1 && u.componentUpdates == 3 && std::fabs(u.lastAlpha - 0.25f) < 1e-6f && u.lastRender == 1.5;
    g_time = 5.0; Draw(u, 2.0f); ok &= u.lastAlpha == 1.0f;
    u.quitting = true; g_time = 6.0; Draw(u, 2.0f); ok &= u.blackUpdates == 2 && u.lastRender == 5.0;
    std::printf(ok ? "005e3f00_TEST PASS\n" : "005e3f00_TEST FAIL\n");
    return ok ? 0 : 1;
}
