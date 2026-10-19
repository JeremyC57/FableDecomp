// Grade: RELOCATION_MATCH (Fable landing 2026-10-19 after the Codex review hold: test exercises a hand-written stand-in, not the candidate)
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