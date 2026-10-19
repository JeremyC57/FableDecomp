// Behaviour test for retail 0x00892a80: the candidate source below is embedded verbatim (from src/00892a80.cpp),
// followed by dependency stubs/spies and a main that asserts on the observable effects.
// ===== candidate source =====
// CGameScriptInterface::DisplayGameInfoText(const CWideString&) const -- show wide game-info text through the player GUI's subtitle
//   renderer, with four empty substitution strings
// Retail: Fable.exe (TLC, VC7.1) @ 0x00892A80, 176 bytes (virtual, manifest ?DisplayGameInfoText@CGameScriptInterface@@UBEXABVCWideString@@@Z).
// Callees: CWideString(const wchar_t*) 0x0099B6B0, ~CWideString() 0x0099B510, CSubtitleRenderer::SetText(CFontBank*, five const CWideString&, ulong, float, bool, bool)
//   @ 0x00492973 (manifest), on the subtitle renderer of the global CPlayerGui* @ 0x013B8790 (as in the landed users).
//   The empty literal is the shared .rdata L"" (0x0122D70C).
// Layout: engine/CGameScriptInterface.h (Component +0x08) and engine/CPlayerGui.h (SubtitleRenderer +0x18); the main
//   component's font bank pointer at +0x1614C is a local view (no CMainGameComponent header; same view as the landed
//   NPlayerGui::CDrawSpiritScores::Draw 0x00644424). CGameScriptInterface_Methods is the lane's method carrier for a
//   generated data-only header, so the virtual is defined non-virtually here (the body is identical).
#include <stddef.h>
#include "engine/CGameScriptInterface.h"
#include "engine/CPlayerGui.h"
class CWideString {
public:
    void* PStringData;
    CWideString(const wchar_t* text);                   // 0x0099B6B0
    ~CWideString();                                     // 0x0099B510
};
struct CFontBank;
struct CSubtitleRenderer {
    void SetText(CFontBank* font, const CWideString& text, const CWideString& a, const CWideString& b, const CWideString& c, const CWideString& d,
                 unsigned long colour, float time, bool b1, bool b2);   // 0x00492973
};
struct CMainGameComponentFontView { unsigned char _prefix[0x1614c]; CFontBank* Font; };   // +0x1614C
extern CPlayerGui* g_pPlayerGui;                        // 0x013B8790
struct CGameScriptInterface_Methods : CGameScriptInterface {
    void DisplayGameInfoText(const CWideString& text) const;
};
void CGameScriptInterface_Methods::DisplayGameInfoText(const CWideString& text) const
{
    // the four empty substitutions are named locals: retail builds all four before pushing any argument
    CWideString d(L"");
    CWideString c(L"");
    CWideString b(L"");
    CWideString a(L"");
    reinterpret_cast<CSubtitleRenderer*>(g_pPlayerGui->SubtitleRenderer)->SetText(
        reinterpret_cast<CMainGameComponentFontView*>(Component)->Font, text, a, b, c, d, 0x2a, 0.0f, true, true);
}

// ===== harness =====
#include <stdio.h>
#include <string.h>
static int g_live, g_calls, g_lenok = 1; static const void* g_args[6]; static CFontBank* g_font; static void* g_this;
static unsigned long g_colour; static float g_time; static bool g_b1, g_b2;
CWideString::CWideString(const wchar_t* t) { PStringData = (void*)t; ++g_live; g_lenok = g_lenok && t[0] == 0; }
CWideString::~CWideString() { --g_live; }
void CSubtitleRenderer::SetText(CFontBank* f, const CWideString& t, const CWideString& a, const CWideString& b, const CWideString& c, const CWideString& d,
                                unsigned long colour, float time, bool b1, bool b2) {
    ++g_calls; g_this = this; g_font = f; g_args[0] = &t; g_args[1] = &a; g_args[2] = &b; g_args[3] = &c; g_args[4] = &d;
    g_args[5] = (const void*)(g_live == 4 && a.PStringData && b.PStringData && c.PStringData && d.PStringData);
    g_colour = colour; g_time = time; g_b1 = b1; g_b2 = b2;
}
static CPlayerGui g_gui;
CPlayerGui* g_pPlayerGui = &g_gui;
static CMainGameComponentFontView g_component;
int main() {
    CGameScriptInterface_Methods gsi; memset(&gsi, 0, sizeof gsi);
    g_component.Font = (CFontBank*)0x4321; gsi.Component = (CMainGameComponent*)&g_component;
    CWideString text = *(const CWideString*)&g_component;            /* any object: only its address is forwarded */
    g_live = 0;
    gsi.DisplayGameInfoText(text);
    int ok = g_calls == 1 && g_this == (void*)g_gui.SubtitleRenderer && g_font == (CFontBank*)0x4321 && g_args[0] == &text;
    ok = ok && g_args[5] && g_lenok && g_live == 0 && g_colour == 0x2a && g_time == 0.0f && g_b1 && g_b2;
    ok = ok && g_args[1] != g_args[2] && g_args[2] != g_args[3] && g_args[3] != g_args[4];
    printf(ok ? "00892a80_TEST PASS\n" : "00892a80_TEST FAIL\n"); return ok ? 0 : 1;
}