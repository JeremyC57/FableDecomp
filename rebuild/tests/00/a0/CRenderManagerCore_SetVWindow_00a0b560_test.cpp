// Behaviour test for 0x00A0B560 CRenderManagerCore::SetVWindow(const C2DBoxF&): the virtual box is converted to an
// absolute box and passed to the display manager's SetViewport(const C2DBoxF&) (DisplayManager at +0x3a3c).
#include <stdio.h>
#include <stddef.h>

class C2DBoxF { public: float left, top, right, bottom; };

static void GFVBoxToABox(const C2DBoxF& vbox, C2DBoxF& abox)          // stand-in: scale virtual 0..1 to 640x480
{
    abox.left = vbox.left * 640.0f; abox.top = vbox.top * 480.0f;
    abox.right = vbox.right * 640.0f; abox.bottom = vbox.bottom * 480.0f;
}

class CDisplayManager { public: void SetViewport(const C2DBoxF& viewport); };
static CDisplayManager* g_observedManager = 0;
static C2DBoxF g_observedViewport;
void CDisplayManager::SetViewport(const C2DBoxF& viewport) { g_observedManager = this; g_observedViewport = viewport; }

struct CRenderManagerCore {
    unsigned char _pad[0x3a3c];
    CDisplayManager* DisplayManager;                                  // +0x3a3c
    void SetVWindow(const C2DBoxF& vwindow)
    {
        C2DBoxF awindow;
        GFVBoxToABox(vwindow, awindow);
        DisplayManager->SetViewport(awindow);
    }
};

int main()
{
    static CRenderManagerCore manager;
    CDisplayManager display;
    manager.DisplayManager = &display;
    C2DBoxF box = { 0.0f, 0.25f, 0.5f, 1.0f };
    manager.SetVWindow(box);
    bool ok = offsetof(CRenderManagerCore, DisplayManager) == 0x3a3c && g_observedManager == &display
        && g_observedViewport.left == 0.0f && g_observedViewport.top == 120.0f && g_observedViewport.right == 320.0f && g_observedViewport.bottom == 480.0f;
    if (!ok) { printf("00a0b560_TEST FAIL\n"); return 1; }
    printf("00a0b560_TEST PASS\n");
    return 0;
}
