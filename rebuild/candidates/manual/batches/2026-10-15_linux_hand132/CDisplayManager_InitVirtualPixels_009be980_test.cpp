#include <cstdio>
#include <math.h>
// Behaviour test for 009be980.

// CDisplayManager::InitVirtualPixels: whole virtual pixels per screen pixel (floor(65536 / size)) and the virtual screen size.
static double g_scaleX, g_scaleY, g_width, g_height;
struct CDisplayManager {
    long X, Y; bool VirtualCoordsResolutionIndependant;
    void InitVirtualPixels()
    {
        VirtualCoordsResolutionIndependant = true;
        g_scaleX = 65536.0 / X; g_scaleY = 65536.0 / Y;
        g_scaleX = floor(g_scaleX); g_scaleY = floor(g_scaleY);
        g_width = X * g_scaleX; g_height = Y * g_scaleY;
    }
};
int main()
{
    CDisplayManager m; m.X = 640; m.Y = 480; m.VirtualCoordsResolutionIndependant = false;
    m.InitVirtualPixels();
    int ok = m.VirtualCoordsResolutionIndependant && g_scaleX == 102.0 && g_scaleY == 136.0 && g_width == 65280.0 && g_height == 65280.0;
    if (!ok) { std::printf("009be980_TEST FAIL\n"); return 1; }
    std::printf("009be980_TEST PASS\n");
    return 0;
}
