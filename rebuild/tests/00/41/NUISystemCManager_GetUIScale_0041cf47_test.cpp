// Behaviour test for 0x0041CF47 NUISystem::CManager::GetUIScale: (1,1) without an engine or on a large screen, width/1024 and height/768 otherwise.
#include <cstdio>
class C2DVector { public: float X, Y; C2DVector(float x, float y) : X(x), Y(y) { } C2DVector(const C2DVector& o) : X(o.X), Y(o.Y) { } };
static void* g_pEngine = 0; static float g_w = 0, g_h = 0;
static float GetScreenWidth() { return g_w; }
static float GetScreenHeight() { return g_h; }
static C2DVector GetUIScale()
{
    C2DVector scale(1.0f, 1.0f);
    if (g_pEngine) {
        if (GetScreenWidth() < 1024.0f || GetScreenHeight() < 768.0f) {
            scale.X = GetScreenWidth() * 0.0009765625f;
            scale.Y = GetScreenHeight() * 0.0013020834f;
        }
    }
    return scale;
}
int main()
{
    g_w = 640; g_h = 480;
    C2DVector a = GetUIScale();
    static int engine; g_pEngine = &engine;
    C2DVector b = GetUIScale();
    g_w = 1280; g_h = 1024;
    C2DVector c = GetUIScale();
    bool ok = a.X == 1.0f && a.Y == 1.0f && b.X == 0.625f && b.Y > 0.6249f && b.Y < 0.6251f && c.X == 1.0f && c.Y == 1.0f;
    if (!ok) { std::printf("0041cf47_TEST FAIL\n"); return 1; }
    std::printf("0041cf47_TEST PASS\n");
    return 0;
}