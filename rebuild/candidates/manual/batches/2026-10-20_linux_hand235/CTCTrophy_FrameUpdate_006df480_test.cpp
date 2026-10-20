// behaviour test for 006df480 CTCTrophy::FrameUpdate: counts the minigame frames down; at zero plays the out-of-time sound,
// shows the "time up" dialog and waits for terminate; otherwise updates the minigame display (left/(2*initial), gained, total)
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
static int g_witnessSounds = 0, g_sounds = 0, g_dialogs = 0; static unsigned long g_dialogFlags = 0;
static float g_fraction = -1.0f; static long g_gained = -1, g_total = -1;
namespace NGameText {
struct CDataBank {
    CWideString GetTextBySymbol(const CCharString& symbol) { return CWideString(symbol.m_text); }
};
}
struct CMainGameComponent { NGameText::CDataBank* PTextBank; };
static NGameText::CDataBank g_bank; static CMainGameComponent g_main; CMainGameComponent* g_pMainGameComponent = &g_main;
struct CDrawQuestInfo {
    void UpdateMiniGame(float timeFraction, long gained, long total) { g_fraction = timeFraction; g_gained = gained; g_total = total; }
};
struct CPlayerGui { CDrawQuestInfo* PDrawQuestInfo; };
static CDrawQuestInfo g_info; static CPlayerGui g_gui; CPlayerGui* g_pPlayerGui = &g_gui;
struct CThingManager { bool PausedA; bool PausedB; };
static CThingManager g_manager;
struct CTrophyDef { CCharString SoundOutOfTime; CTrophyDef() : SoundOutOfTime("OUT") {} };
static CTrophyDef g_def;
struct CTCBase {
    virtual void v00() {}
    CThingManager* GetThingManager() { return &g_manager; }
};
struct CTCTrophy : CTCBase {
    CTrophyDef*   PDef;
    bool          MinigameInProgress;
    long          MinigameInitialFramesLeft;
    long          MinigameCurrentFramesLeft;
    long          MinigameStartingWitnessCount;
    long          NumberOfWitnessesLeft;
    bool          WaitingForTerminate;
    void UpdateGainedWitnessSounds() { ++g_witnessSounds; }
    unsigned long PlaySound(const CCharString& name) { if (std::strcmp(name.m_text, "OUT") == 0) ++g_sounds; return 0; }
    void StartDialogBox(CWideString text, unsigned long flags, CWideString b1, CWideString b2, CWideString b3)
    { if (Same(text, "TEXT_GUI_TROPHY_TIME_UP") && Same(b1, "") && Same(b2, "") && Same(b3, "")) ++g_dialogs; g_dialogFlags = flags; }
    virtual void FrameUpdate();
};
void CTCTrophy::FrameUpdate()
{
    UpdateGainedWitnessSounds();
    if (!MinigameInProgress || GetThingManager()->PausedA || GetThingManager()->PausedB)
        return;
    long framesLeft = --MinigameCurrentFramesLeft;
    if (framesLeft <= 0) {
        PlaySound(PDef->SoundOutOfTime);
        CWideString text = g_pMainGameComponent->PTextBank->GetTextBySymbol("TEXT_GUI_TROPHY_TIME_UP");
        StartDialogBox(text, 10, L"", L"", L"");
        WaitingForTerminate = true;
        return;
    }
    float fraction = 0.0f;
    long initialFrames = MinigameInitialFramesLeft;
    if (initialFrames > 0)
        fraction = (float)framesLeft / ((float)initialFrames * 2.0f);
    g_pPlayerGui->PDrawQuestInfo->UpdateMiniGame(fraction, MinigameStartingWitnessCount - NumberOfWitnessesLeft,
                                                 MinigameStartingWitnessCount);
}
int main()
{
    g_main.PTextBank = &g_bank; g_gui.PDrawQuestInfo = &g_info; g_manager.PausedA = false; g_manager.PausedB = false;
    CTCTrophy trophy; trophy.PDef = &g_def; trophy.MinigameInProgress = true; trophy.MinigameInitialFramesLeft = 10;
    trophy.MinigameCurrentFramesLeft = 6; trophy.MinigameStartingWitnessCount = 7; trophy.NumberOfWitnessesLeft = 3;
    trophy.WaitingForTerminate = false;
    trophy.FrameUpdate();
    bool ok = g_witnessSounds == 1 && trophy.MinigameCurrentFramesLeft == 5 && g_fraction == 0.25f && g_gained == 4 && g_total == 7;
    g_manager.PausedB = true; trophy.FrameUpdate();
    ok = ok && g_witnessSounds == 2 && trophy.MinigameCurrentFramesLeft == 5;
    g_manager.PausedB = false; trophy.MinigameCurrentFramesLeft = 1; trophy.FrameUpdate();
    ok = ok && g_sounds == 1 && g_dialogs == 1 && g_dialogFlags == 10 && trophy.WaitingForTerminate;
    if (!ok) { std::printf("006df480_TEST FAIL\n"); return 1; }
    std::printf("006df480_TEST PASS\n");
    return 0;
}
