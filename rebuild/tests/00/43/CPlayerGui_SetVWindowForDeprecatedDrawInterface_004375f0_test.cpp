#include <cstdio>
class C2DBoxF {
public:
    float TLX, TLY, BRX, BRY;
    C2DBoxF() {}
};
class CDisplayManager {
public:
    const C2DBoxF GetVVisibleTelevisionScreenArea() const;   // 0x009BECE0
};
class CSystemManager {
public:
    unsigned char    _pad00[0x60];
    CDisplayManager* PDisplayManager;                        // +0x60 CCountedPointer<CDisplayManager>::Data
};
CSystemManager* __fastcall GFGetSystemManager();             // 0x009A4EC0
class CSubtitleRenderer {
public:
    void SetVWindowsForDeprecatedDrawInterface(const C2DBoxF& window, const C2DBoxF& tv_area);   // 0x0048DBA6
};
class CPlayerGui {
public:
    void*             _vfptr;                                // +0x000
    unsigned char     _pad004[0x18 - 0x4];
    CSubtitleRenderer SubtitleRenderer;                      // +0x018
    unsigned char     _pad019[0x280 - 0x19];
    C2DBoxF           VWindow;                               // +0x280
    void SetVWindowForDeprecatedDrawInterface(const C2DBoxF& window);
};
void CPlayerGui::SetVWindowForDeprecatedDrawInterface(const C2DBoxF& window)
{
    VWindow = window;
    CDisplayManager* display = GFGetSystemManager()->PDisplayManager;
    SubtitleRenderer.SetVWindowsForDeprecatedDrawInterface(window, display->GetVVisibleTelevisionScreenArea());
}

static CDisplayManager g_display;
static CSystemManager g_system;
static C2DBoxF g_seen_window, g_seen_area;
static int g_calls;
CSystemManager* __fastcall GFGetSystemManager() { g_system.PDisplayManager = &g_display; return &g_system; }
const C2DBoxF CDisplayManager::GetVVisibleTelevisionScreenArea() const { C2DBoxF b; b.TLX = 0.0f; b.TLY = 0.0f; b.BRX = 640.0f; b.BRY = 480.0f; return b; }
void CSubtitleRenderer::SetVWindowsForDeprecatedDrawInterface(const C2DBoxF& window, const C2DBoxF& tv_area) { g_seen_window = window; g_seen_area = tv_area; ++g_calls; }
static CPlayerGui g_gui;
int main()
{
    C2DBoxF w; w.TLX = 1.0f; w.TLY = 2.0f; w.BRX = 3.0f; w.BRY = 4.0f;
    g_gui.SetVWindowForDeprecatedDrawInterface(w);
    bool ok = g_gui.VWindow.TLX == 1.0f && g_gui.VWindow.BRY == 4.0f && g_calls == 1
           && g_seen_window.BRX == 3.0f && g_seen_area.BRX == 640.0f && g_seen_area.BRY == 480.0f && g_seen_area.TLX == 0.0f;
    std::printf(ok ? "004375f0_TEST PASS\n" : "004375f0_TEST FAIL\n");
    return ok ? 0 : 1;
}