// Grade: RELOCATION_MATCH (Fable landing 2026-10-19 after the Codex review hold: test exercises a hand-written stand-in, not the candidate)
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