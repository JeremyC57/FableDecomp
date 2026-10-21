// behaviour test for 009bf600 CDisplayManager::GetClosestMode
#include <cstdio>
#include <cstdlib>
struct Mode { unsigned int Width, Height, RefreshRate, Format; };
struct Ext { int X, Y; };
static Mode g_cur = { 1024, 768, 75, 22 };
static Mode g_modes[2][3] = {
    { { 640, 480, 60, 23 }, { 800, 600, 60, 23 }, { 1280, 1024, 60, 23 } },   // 16-bit format
    { { 640, 480, 85, 22 }, { 1024, 768, 70, 22 }, { 1280, 960, 60, 22 } },   // 32-bit format
};
static unsigned int Depth(unsigned int fmt) { return fmt == 22 ? 32 : 16; }
static Mode GetClosestMode(const Ext& res, unsigned int depth, bool fullScreen, bool hasDevice)
{
    if (!fullScreen) {
        Mode mode = hasDevice ? g_cur : g_cur;
        mode.Width = res.X;
        mode.Height = res.Y;
        return mode;
    }
    int bestRes = 0x7fffffff, bestDepth = 0x7fffffff;
    Mode best; best.Format = 0; best.Width = best.Height = best.RefreshRate = 0;
    for (unsigned int i = 0; i < 2; ++i)
        for (unsigned int j = 0; j < 3; ++j) {
            Mode mode = g_modes[i][j];
            int depthError = std::abs((int)(Depth(mode.Format) - depth));
            if (depthError <= bestDepth) {
                int resError = std::abs(res.Y - (int)mode.Height) + std::abs(res.X - (int)mode.Width);
                if (resError <= bestRes) { best = mode; bestRes = resError; bestDepth = depthError; }
            }
        }
    return best;
}
int main()
{
    int bad = 0;
    Ext r = { 1000, 700 };
    Mode w = GetClosestMode(r, 32, false, true);
    if (w.Width != 1000 || w.Height != 700 || w.RefreshRate != 75 || w.Format != 22) bad |= 1;
    Mode f = GetClosestMode(r, 32, true, true);
    if (f.Width != 1024 || f.Height != 768 || f.RefreshRate != 70) bad |= 2;
    Mode g = GetClosestMode(r, 16, true, true);
    if (g.Width != 800 || g.Height != 600 || g.Format != 23) bad |= 4;
    if (bad) { std::printf("009bf600_TEST FAIL\n"); return 1; }
    std::printf("009bf600_TEST PASS\n");
    return 0;
}