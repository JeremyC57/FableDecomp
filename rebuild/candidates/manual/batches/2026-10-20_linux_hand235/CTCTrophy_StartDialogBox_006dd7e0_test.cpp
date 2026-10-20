// behaviour test for 006dd7e0 CTCTrophy::StartDialogBox: no dialog while an info screen is up; otherwise the three-button
// SetText when button3 is not empty, else the two-button form; then waits for confirmation and registers once
// ---- test-side string stubs (narrow text buffers; the wide type stores chars) ----
#include <cstdio>
#include <cstring>
struct CWideString {
    char m_text[160];
    CWideString() { m_text[0] = 0; }
    CWideString(const char* s) { std::strcpy(m_text, s); }
    CWideString(const wchar_t* s) { int i = 0; for (; s[i]; ++i) m_text[i] = (char)s[i]; m_text[i] = 0; }
    CWideString(const CWideString& o) { std::strcpy(m_text, o.m_text); }
    ~CWideString() {}
    const CWideString& operator+=(const CWideString& o) { std::strcat(m_text, o.m_text); return *this; }
    bool operator!=(const wchar_t* s) const { return CWideString(s).m_text[0] != m_text[0] || std::strcmp(CWideString(s).m_text, m_text) != 0; }
};
inline CWideString operator+(const CWideString& a, const CWideString& b) { CWideString r(a); r += b; return r; }
inline CWideString operator+(const CWideString& a, const wchar_t* b) { CWideString r(a); r += CWideString(b); return r; }
struct CCharString {
    char m_text[160];
    CCharString(const char* s, long length = -1) { (void)length; std::strcpy(m_text, s); }
    CCharString(const CCharString& o) { std::strcpy(m_text, o.m_text); }
    ~CCharString() {}
    CWideString ToWideString() const { return CWideString(m_text); }
    static CCharString FormatDouble(long digits, double value) { char b[64]; std::sprintf(b, "%.*f", (int)digits, value); return CCharString(b); }
};
inline CCharString GFIntToCharString(long v) { char b[32]; std::sprintf(b, "%ld", v); return CCharString(b); }
inline bool Same(const CWideString& w, const char* s) { return std::strcmp(w.m_text, s) == 0; }
// ---- end stubs ----
struct CFontBank { int id; };
static int g_three = 0, g_two = 0, g_registered = 0; static char g_lastText[64];
struct CSubtitleRenderer {
    void SetText(CFontBank*, const CWideString& text, const CWideString& name, const CWideString&, const CWideString&,
                 const CWideString&, unsigned long, float, bool, bool) { ++g_three; std::strcpy(g_lastText, text.m_text); (void)name; }
    void SetText(CFontBank*, const CWideString& text, const CWideString&, const CWideString&,
                 const CWideString&, unsigned long, float, bool, bool) { ++g_two; std::strcpy(g_lastText, text.m_text); }
};
namespace NHeroInformationScreens { struct CManager { bool ScreenActive; }; }
static NHeroInformationScreens::CManager g_screens;
struct CPlayerGui {
    CSubtitleRenderer SubtitleRenderer;
    NHeroInformationScreens::CManager* GetPHeroInfoScreensManager() { return &g_screens; }
};
static CPlayerGui g_gui; CPlayerGui* g_pPlayerGui = &g_gui;
struct CMainGameComponent { CFontBank* Font; };
static CMainGameComponent g_main; CMainGameComponent* g_pMainGameComponent = &g_main;
struct CTCBase {
    virtual void v00() {}
    void AddToGlobalUpdateTCs() { ++g_registered; }
};
struct CTCTrophy : CTCBase {
    bool WaitingForConfirmation;
    bool IsGlobalTC;
    void StartDialogBox(CWideString text, unsigned long flags, CWideString button1, CWideString button2, CWideString button3);
};
void CTCTrophy::StartDialogBox(CWideString text, unsigned long flags, CWideString button1, CWideString button2, CWideString button3)
{
    if (g_pPlayerGui->GetPHeroInfoScreensManager()->ScreenActive)
        return;
    if (button3 != L"")
        g_pPlayerGui->SubtitleRenderer.SetText(g_pMainGameComponent->Font, text, L"", button1, button2, button3, flags, 0.0f, true, true);
    else
        g_pPlayerGui->SubtitleRenderer.SetText(g_pMainGameComponent->Font, text, L"", button1, button2, flags, 0.0f, true, true);
    WaitingForConfirmation = true;
    if (!IsGlobalTC) {
        AddToGlobalUpdateTCs();
        IsGlobalTC = true;
    }
}
int main()
{
    CTCTrophy trophy; trophy.WaitingForConfirmation = false; trophy.IsGlobalTC = false;
    g_screens.ScreenActive = true;
    trophy.StartDialogBox(CWideString("a"), 1, CWideString("b"), CWideString("c"), CWideString("d"));
    bool ok = g_three == 0 && g_two == 0 && !trophy.WaitingForConfirmation;
    g_screens.ScreenActive = false;
    trophy.StartDialogBox(CWideString("a"), 1, CWideString("b"), CWideString("c"), CWideString("d"));
    ok = ok && g_three == 1 && Same(CWideString(g_lastText), "a") && trophy.WaitingForConfirmation && g_registered == 1;
    trophy.StartDialogBox(CWideString("x"), 1, CWideString("b"), CWideString("c"), CWideString(""));
    ok = ok && g_two == 1 && Same(CWideString(g_lastText), "x") && g_registered == 1;
    if (!ok) { std::printf("006dd7e0_TEST FAIL\n"); return 1; }
    std::printf("006dd7e0_TEST PASS\n");
    return 0;
}
