#include <cstdio>
static int g_base = 0, g_refresh = 0, g_clear = 0;
struct CRedefinerList {
    bool RefreshAfterFade; float FadeOutTime;
    void BaseUpdate(float) { ++g_base; }
    void Refresh() { ++g_refresh; }
    void ClearList() { ++g_clear; }
    void Update(float dt)
    {
        BaseUpdate(dt);
        if (FadeOutTime > 0.0f) {
            FadeOutTime -= dt;
            if (FadeOutTime <= 0.0f) {
                FadeOutTime = 0.0f;
                if (RefreshAfterFade) Refresh(); else ClearList();
            }
        }
    }
};
int main()
{
    CRedefinerList l; l.RefreshAfterFade = true; l.FadeOutTime = 0.5f;
    l.Update(0.25f);
    if (l.FadeOutTime != 0.25f || g_refresh != 0 || g_clear != 0) { std::printf("00557080_TEST FAIL\n"); return 1; }
    l.Update(0.5f);
    if (l.FadeOutTime != 0.0f || g_refresh != 1 || g_clear != 0) { std::printf("00557080_TEST FAIL\n"); return 1; }
    l.Update(0.5f);
    if (g_refresh != 1 || g_clear != 0) { std::printf("00557080_TEST FAIL\n"); return 1; }
    l.RefreshAfterFade = false; l.FadeOutTime = 0.1f; l.Update(0.1f);
    if (g_clear != 1 || l.FadeOutTime != 0.0f || g_base != 4) { std::printf("00557080_TEST FAIL\n"); return 1; }
    std::printf("00557080_TEST PASS\n");
    return 0;
}