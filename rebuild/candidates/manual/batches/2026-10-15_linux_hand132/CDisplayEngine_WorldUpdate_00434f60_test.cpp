#include <cstdio>
// Behaviour test for 00434f60.

// CDisplayEngine::WorldUpdate: the readiness count runs down to zero (stopping the progress display and clearing its strings once),
// SetAboutToRun is called while it is still pending, and the engine's draw-distance factor minus one is published.
static int g_stops = 0; static const char* g_title = "x"; static const char* g_text = "y"; static float g_scale = 0.0f;
static void StopProgress(bool a, bool b) { if (a && !b) ++g_stops; }
struct CIEngine { float factor; virtual float GetPrimitiveDrawDistanceFactor() { return factor; } virtual ~CIEngine() { } };
struct CDisplayEngine {
    CIEngine* Engine3D; long InitialFadeReadynessCount; int aboutToRun;
    void SetAboutToRun() { ++aboutToRun; }
    void WorldUpdate()
    {
        if (InitialFadeReadynessCount > 0) {
            if (--InitialFadeReadynessCount == 0) { StopProgress(true, false); g_title = ""; g_text = ""; }
        }
        if (InitialFadeReadynessCount > 0) SetAboutToRun();
        if (Engine3D != 0) g_scale = Engine3D->GetPrimitiveDrawDistanceFactor() - 1.0f;
    }
};
int main()
{
    CIEngine engine; engine.factor = 3.0f;
    CDisplayEngine e; e.Engine3D = 0; e.InitialFadeReadynessCount = 2; e.aboutToRun = 0;
    e.WorldUpdate();
    int ok = e.InitialFadeReadynessCount == 1 && e.aboutToRun == 1 && g_stops == 0 && g_scale == 0.0f;
    e.Engine3D = &engine; e.WorldUpdate();
    ok = ok && e.InitialFadeReadynessCount == 0 && e.aboutToRun == 1 && g_stops == 1 && g_title[0] == 0 && g_text[0] == 0 && g_scale == 2.0f;
    e.WorldUpdate();
    ok = ok && g_stops == 1 && e.InitialFadeReadynessCount == 0;
    if (!ok) { std::printf("00434f60_TEST FAIL\n"); return 1; }
    std::printf("00434f60_TEST PASS\n");
    return 0;
}
