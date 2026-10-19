// Behaviour test for retail 0x008929d0: the candidate source below is embedded verbatim (from src/008929d0.cpp),
// followed by dependency stubs/spies and a main that asserts on the observable effects.
// ===== candidate source =====
// CGameScriptInterface::DisplayGameInfo(const CCharString&) const -- show narrow game-info text through the player GUI's subtitle
//   renderer, with four empty substitution strings
// Retail: Fable.exe (TLC, VC7.1) @ 0x008929D0, 176 bytes (virtual, manifest ?DisplayGameInfo@CGameScriptInterface@@UBEXABVCCharString@@@Z).
// Callees: CCharString(const char*, long = -1) 0x0099EBF0, ~CCharString() 0x0099EAE0, CSubtitleRenderer::SetText(CFontBank*, five const CCharString&, ulong, float, bool, bool)
//   @ 0x00493049 (manifest), on the subtitle renderer of the global CPlayerGui* @ 0x013B8790 (as in the landed users).
//   The empty literal is the shared .rdata "" (0x0122D70E).
// Layout: engine/CGameScriptInterface.h (Component +0x08) and engine/CPlayerGui.h (SubtitleRenderer +0x18); the main
//   component's font bank pointer at +0x1614C is a local view (no CMainGameComponent header; same view as the landed
//   NPlayerGui::CDrawSpiritScores::Draw 0x00644424). CGameScriptInterface_Methods is the lane's method carrier for a
//   generated data-only header, so the virtual is defined non-virtually here (the body is identical).
#include <stddef.h>
#include "engine/CGameScriptInterface.h"
#include "engine/CPlayerGui.h"
class CCharString {
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
struct CFontBank;
struct CSubtitleRenderer {
    void SetText(CFontBank* font, const CCharString& text, const CCharString& a, const CCharString& b, const CCharString& c, const CCharString& d,
                 unsigned long colour, float time, bool b1, bool b2);   // 0x00493049
};
struct CMainGameComponentFontView { unsigned char _prefix[0x1614c]; CFontBank* Font; };   // +0x1614C
extern CPlayerGui* g_pPlayerGui;                        // 0x013B8790
struct CGameScriptInterface_Methods : CGameScriptInterface {
    void DisplayGameInfo(const CCharString& text) const;
};
void CGameScriptInterface_Methods::DisplayGameInfo(const CCharString& text) const
{
    // the four empty substitutions are named locals: retail builds all four before pushing any argument
    CCharString d("");
    CCharString c("");
    CCharString b("");
    CCharString a("");
    reinterpret_cast<CSubtitleRenderer*>(g_pPlayerGui->SubtitleRenderer)->SetText(
        reinterpret_cast<CMainGameComponentFontView*>(Component)->Font, text, a, b, c, d, 0x2a, 0.0f, true, true);
}

// ===== harness =====
#include <stdio.h>
#include <string.h>
static int g_live, g_calls, g_lenok = 1; static const void* g_args[6]; static CFontBank* g_font; static void* g_this;
static unsigned long g_colour; static float g_time; static bool g_b1, g_b2;
CCharString::CCharString(const char* t, long len) { PStringData = (void*)t; ++g_live; g_lenok = g_lenok && len == -1 && t[0] == 0; }
CCharString::~CCharString() { --g_live; }
void CSubtitleRenderer::SetText(CFontBank* f, const CCharString& t, const CCharString& a, const CCharString& b, const CCharString& c, const CCharString& d,
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
    CCharString text = *(const CCharString*)&g_component;            /* any object: only its address is forwarded */
    g_live = 0;
    gsi.DisplayGameInfo(text);
    int ok = g_calls == 1 && g_this == (void*)g_gui.SubtitleRenderer && g_font == (CFontBank*)0x4321 && g_args[0] == &text;
    ok = ok && g_args[5] && g_lenok && g_live == 0 && g_colour == 0x2a && g_time == 0.0f && g_b1 && g_b2;
    ok = ok && g_args[1] != g_args[2] && g_args[2] != g_args[3] && g_args[3] != g_args[4];
    printf(ok ? "008929d0_TEST PASS\n" : "008929d0_TEST FAIL\n"); return ok ? 0 : 1;
}