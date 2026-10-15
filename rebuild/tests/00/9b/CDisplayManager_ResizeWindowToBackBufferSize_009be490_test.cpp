#include <cstdio>
// Behaviour test for 009be490.

// CDisplayManager::ResizeWindowToBackBufferSize: windowed and not attached -> window moved to the client origin and sized to
// the back buffer plus 2*frame (and the caption).
static int g_moves = 0; static int g_mx, g_my, g_mw, g_mh;
static int GetSystemMetrics_(int i) { return i == 32 ? 4 : i == 33 ? 3 : i == 4 ? 20 : 0; }
struct POINT_ { long x, y; };
static void ClientToScreen_(void* w, POINT_* p) { (void)w; p->x += 100; p->y += 50; }
static void MoveWindow_(void* w, int x, int y, int cx, int cy, int repaint) { (void)w; (void)repaint; ++g_moves; g_mx = x; g_my = y; g_mw = cx; g_mh = cy; }
struct CSystemManager { bool AttachedToExternalWindow; };
struct CDisplayManager {
    CSystemManager* SystemManager; long BBX, BBY; bool ExclusiveMode; void* WindowHandle;
    void ResizeWindowToBackBufferSize()
    {
        if (!ExclusiveMode && !SystemManager->AttachedToExternalWindow) {
            long borderWidth = GetSystemMetrics_(32) * 2;
            long borderHeight = GetSystemMetrics_(33) * 2 + GetSystemMetrics_(4);
            POINT_ origin; origin.x = 0; origin.y = 0;
            ClientToScreen_(WindowHandle, &origin);
            MoveWindow_(WindowHandle, origin.x, origin.y, BBX + borderWidth, BBY + borderHeight, 1);
        }
    }
};
int main()
{
    CSystemManager sm; sm.AttachedToExternalWindow = false;
    CDisplayManager m; m.SystemManager = &sm; m.BBX = 800; m.BBY = 600; m.ExclusiveMode = false; m.WindowHandle = 0;
    m.ResizeWindowToBackBufferSize();
    int ok = g_moves == 1 && g_mx == 100 && g_my == 50 && g_mw == 808 && g_mh == 626;
    m.ExclusiveMode = true; m.ResizeWindowToBackBufferSize();
    m.ExclusiveMode = false; sm.AttachedToExternalWindow = true; m.ResizeWindowToBackBufferSize();
    ok = ok && g_moves == 1;
    if (!ok) { std::printf("009be490_TEST FAIL\n"); return 1; }
    std::printf("009be490_TEST PASS\n");
    return 0;
}