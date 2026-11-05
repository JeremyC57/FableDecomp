#include <cstdio>
float g_themeHeight = 42.5f;
float __stdcall CObstacleNavigatorExternalInterface_GetThemeHeightAt(int at)
{
    return g_themeHeight;
}
int main()
{
    g_themeHeight = 7.25f;
    float r = CObstacleNavigatorExternalInterface_GetThemeHeightAt(99);
    if (r == 7.25f) { std::printf("OK_0x004df3dd\n"); return 0; }
    std::printf("FAIL r=%f\n", r); return 1;
}