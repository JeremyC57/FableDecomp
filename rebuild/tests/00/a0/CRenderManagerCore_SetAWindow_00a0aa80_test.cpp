// behaviour test for 0x00A0AA80 CRenderManagerCore::SetAWindow(const C2DBoxF&)
// (model: the core holds its CDisplayManager* at +0x3A3C and forwards the box, by reference, to CDisplayManager::SetViewport).
// Rewritten by agentNF56 on the new name's behaviour (the old test drove the CRenderManagerCoreWindowView stand-in of
// fable_render_window.h); the catalog PassPattern is kept.
#include <stdio.h>
#include <stddef.h>
namespace model {
struct Box { float X0, Y0, X1, Y1; };
struct DisplayManager;
static DisplayManager* g_ObservedManager = 0;
static const Box* g_ObservedViewport = 0;
struct DisplayManager {
    int Dummy;
    void SetViewport(const Box& viewport) { g_ObservedManager = this; g_ObservedViewport = &viewport; }
};
struct Core {
    char _pad[0x3a3c];
    DisplayManager* Display;
    void SetAWindow(const Box& window) { Display->SetViewport(window); }
};
}
int main()
{
    static model::Core core;
    model::DisplayManager displayManager;
    const model::Box viewport = { 1.0f, 2.0f, 639.0f, 479.0f };
    core.Display = &displayManager;
    core.SetAWindow(viewport);
    if (offsetof(model::Core, Display) != 0x3a3c ||
        model::g_ObservedManager != &displayManager || model::g_ObservedViewport != &viewport)
    {
        printf("FAIL_0x00a0aa80\n");
        return 1;
    }
    printf("FABLETLC_RENDER_MANAGER_SET_A_WINDOW_BEHAVIOR PASS\n");
    return 0;
}
