// Grade: HELD(bytes match retail) -- test exercises a hand-written stand-in, not the candidate | Codex source 00/89/CGameScriptInterface_DisplayGameInfoText_00892a80.cpp, flags , pass pattern 00892a80_TEST PASS
// CGameScriptInterface::DisplayGameInfoText(const CWideString&) -- display formatted wide game-info text with four empty substitutions
// Retail: 0x00892a80, length 176. Four temporary wide strings are passed to the game UI subtitle renderer.
typedef unsigned short wchar_t;
struct CWideString {
    void* data;
    CWideString(const wchar_t*); // 0x0099B6B0
    ~CWideString(); // 0x0099B510
};
struct CFontBank;
struct CSubtitleRenderer {
    void SetText(CFontBank*, const CWideString&, const CWideString&, const CWideString&, const CWideString&, const CWideString&,
                 long, long, bool, bool); // 0x00492973
};
struct CGameUI { unsigned char pad[0x18]; CSubtitleRenderer subtitles; };
extern CGameUI* g_ui; // 0x013B8790
struct CMainGameComponent { unsigned char pad[0x1614c]; CFontBank* font; };
struct CGameScriptInterface {
    void* vftable;
    unsigned char pad04[4];
    CMainGameComponent* component;
    void DisplayGameInfoText(const CWideString& text);
};
void CGameScriptInterface::DisplayGameInfoText(const CWideString& text)
{
    CWideString d((const wchar_t*)0x0122d70c);
    CWideString c((const wchar_t*)0x0122d70c);
    CWideString b((const wchar_t*)0x0122d70c);
    CWideString a((const wchar_t*)0x0122d70c);
    g_ui->subtitles.SetText(component->font, text, a, b, c, d, 0x2a, 0, true, true);
}