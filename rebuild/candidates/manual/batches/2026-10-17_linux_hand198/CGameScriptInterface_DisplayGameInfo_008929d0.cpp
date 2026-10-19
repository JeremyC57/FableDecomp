// CGameScriptInterface::DisplayGameInfo(const CCharString&) -- display formatted game-info text with four empty substitutions
// Retail: 0x008929d0, length 176. The UI subtitle renderer receives the component font bank, source text,
// four temporary empty CCharStrings, and the fixed display flags 0x2a,0,1,1.
// Callees: CCharString(const char*,long) @0x0099EBF0; CCharString::~CCharString @0x0099EAE0;
// CSubtitleRenderer::SetText @0x00493049; operator new/delete are hidden in the string helpers.
struct CCharString {
    void* data;
    CCharString(const char*, long); // 0x0099EBF0
    ~CCharString(); // 0x0099EAE0
};
struct CFontBank;
struct CSubtitleRenderer {
    void SetText(CFontBank*, const CCharString&, const CCharString&, const CCharString&, const CCharString&, const CCharString&,
                 long, long, bool, bool); // 0x00493049
};
struct CGameUI { unsigned char pad[0x18]; CSubtitleRenderer subtitles; };
extern CGameUI* g_ui; // 0x013B8790
struct CMainGameComponent { unsigned char pad[0x1614c]; CFontBank* font; };
struct CGameScriptInterface {
    void* vftable;
    unsigned char pad04[4];
    CMainGameComponent* component;
    void DisplayGameInfo(const CCharString& text);
};
void CGameScriptInterface::DisplayGameInfo(const CCharString& text)
{
    CCharString d((const char*)0x0122d70e, -1);
    CCharString c((const char*)0x0122d70e, -1);
    CCharString b((const char*)0x0122d70e, -1);
    CCharString a((const char*)0x0122d70e, -1);
    g_ui->subtitles.SetText(component->font, text, a, b, c, d, 0x2a, 0, true, true);
}
