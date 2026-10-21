// Behaviour test for CSystemManager::InitWindow (0x009A64B0): the window-class record and the centring arithmetic.
#include <cstdio>
static int g_metric[8] = { 1024, 768, 0, 0, 20, 1, 2, 0 };   // SM_CXSCREEN, SM_CYSCREEN, ..., SM_CYCAPTION=4, CXBORDER=5, CYBORDER=6
static int GetSystemMetrics(int i) { return g_metric[i]; }
struct WC { unsigned cbSize; unsigned style; int extra; void* icon; void* iconSm; };
struct Sys {
    WC wc; int x, y, w, h; bool valid;
    bool InitWindow(long width, long height)
    {
        wc.cbSize = 0x30; wc.style = 0; wc.extra = 0; wc.iconSm = (void*)101; wc.icon = (void*)101;
        int screenWidth = GetSystemMetrics(0);
        int screenHeight = GetSystemMetrics(1);
        int borderWidth = GetSystemMetrics(5) * 2;
        int yy = (screenHeight - GetSystemMetrics(6) * 2 - height - GetSystemMetrics(4)) / 2;
        int xx = (screenWidth - borderWidth - width) / 2;
        x = xx; y = yy; w = width; h = height; valid = true;
        return true;
    }
};
int main()
{
    Sys s; s.valid = false;
    bool r = s.InitWindow(640, 480);
    // x = (1024 - 2 - 640) / 2 = 191; y = (768 - 4 - 480 - 20) / 2 = 132
    if (!r || !s.valid || s.x != 191 || s.y != 132 || s.w != 640 || s.h != 480 || s.wc.cbSize != 0x30) { std::printf("009a64b0_TEST FAIL\n"); return 1; }
    s.InitWindow(1100, 900);   // larger than the screen: negative, rounded toward zero
    if (s.x != -39 || s.y != -78) { std::printf("009a64b0_TEST FAIL\n"); return 1; }
    std::printf("009a64b0_TEST PASS\n");
    return 0;
}
