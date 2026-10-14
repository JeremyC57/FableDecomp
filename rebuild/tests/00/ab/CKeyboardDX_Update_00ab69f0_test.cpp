#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[128] = "";
static bool g_active = false, g_focus = false; struct CSystemManager { bool IsApplicationActive() { return g_active; } bool HasInputFocus() { return g_focus; } }; static CSystemManager g_sys; static CSystemManager* GFGetSystemManager() { return &g_sys; }
static int g_set = -1;
struct CKeyboardDX { virtual void SetHasInputFocus(bool f) { std::strcat(g_seq, f ? "1" : "0"); }
    void UpdateKeyStates() { std::strcat(g_seq, "K"); } void UpdateEvents() { std::strcat(g_seq, "E"); }
    void UpdateAcquired() { if (GFGetSystemManager()->IsApplicationActive() && GFGetSystemManager()->HasInputFocus()) SetHasInputFocus(true); else SetHasInputFocus(false); }
    bool Update() { UpdateAcquired(); UpdateKeyStates(); UpdateEvents(); return true; } };

int main()
{
    CKeyboardDX k; g_active = true; g_focus = true; if (!k.Update() || std::strcmp(g_seq, "1KE")) { std::printf("00ab69f0_TEST FAIL\n"); return 1; }
    g_seq[0] = 0; g_focus = false; if (!k.Update() || std::strcmp(g_seq, "0KE")) { std::printf("00ab69f0_TEST FAIL\n"); return 1; }
    std::printf("00ab69f0_TEST PASS\n");
    return 0;
}