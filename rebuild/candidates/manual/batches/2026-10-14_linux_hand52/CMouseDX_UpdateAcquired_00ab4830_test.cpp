#include <cstdio>
#include <cstdlib>
#include <cstring>
static bool g_active = false, g_focus = false; struct CSystemManager { bool IsApplicationActive() { return g_active; } bool HasInputFocus() { return g_focus; } }; static CSystemManager g_sys; static CSystemManager* GFGetSystemManager() { return &g_sys; }
static int g_set = -1;
struct CDev { virtual void SetHasInputFocus(bool f) { g_set = f ? 1 : 0; }
    void UpdateAcquired() { if (GFGetSystemManager()->IsApplicationActive() && GFGetSystemManager()->HasInputFocus()) SetHasInputFocus(true); else SetHasInputFocus(false); } };

int main()
{
    CDev d; g_active = true; g_focus = true; d.UpdateAcquired(); if (g_set != 1) { std::printf("00ab4830_TEST FAIL\n"); return 1; }
    g_focus = false; d.UpdateAcquired(); if (g_set != 0) { std::printf("00ab4830_TEST FAIL\n"); return 1; } g_active = false; g_focus = true; g_set = -1; d.UpdateAcquired(); if (g_set != 0) { std::printf("00ab4830_TEST FAIL\n"); return 1; }
    std::printf("00ab4830_TEST PASS\n");
    return 0;
}
