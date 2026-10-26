// Behaviour test for CTCWife::InitiateDivorce (0x006CEF20): with a hero-information screen open nothing
// happens; otherwise the divorce subtitle is shown from the def text with the main font, the wife waits for
// the divorce in the global update list, and a type-9 pause game event carrying the current player and a
// true flag is queued.
#include <cstdio>
#include <cstring>
class CCharString { public: const char* text; };
class CFontBank { public: int id; };
static CFontBank g_font = { 7 };
struct CMemoryBuffer {
    unsigned char* m_cursor;
    void Copy(unsigned long length, const void* data) { std::memcpy(m_cursor, data, length); m_cursor += length; }
};
class CGameEvent : public CMemoryBuffer { public:
    long          Type;
    signed char   Player;
    unsigned char Data[32];
    unsigned char EndPos;
    bool          Valid;
    bool          Replacement;
    CGameEvent(long type) : Type(type), Player(-1), EndPos(0), Valid(true), Replacement(false) { m_cursor = Data; }
    void AddBool(bool value) { value = value ? true : false; Copy(1, &value); }
};
static long g_eventType = 0; static int g_eventPlayer = 99; static int g_eventFlag = 99; static int g_events = 0; static bool g_eventValid = false;
class CMainGameComponent { public:
    unsigned char _pad[0x1614c];
    CFontBank* m_pFontBank;
    void AddGameEvent(const CGameEvent& event) { ++g_events; g_eventType = event.Type; g_eventPlayer = event.Player; g_eventFlag = event.Data[0]; g_eventValid = event.Valid && !event.Replacement; }
};
static CMainGameComponent g_mainGame; static CMainGameComponent* g_pMainGameComponent = &g_mainGame;
static int g_subtitles = 0; static const char* g_subtitleText = 0; static CFontBank* g_subtitleFont = 0; static unsigned long g_subtitleColour = 0; static float g_subtitleTime = 1; static bool g_subA = false, g_subB = false;
class CSubtitleRenderer { public:
    void SetText(CFontBank* font, const CCharString& text, unsigned long colour, float time, bool a, bool b)
    { ++g_subtitles; g_subtitleFont = font; g_subtitleText = text.text; g_subtitleColour = colour; g_subtitleTime = time; g_subA = a; g_subB = b; }
};
namespace NHeroInformationScreens { class CManager { public: unsigned char _pad00[0x34]; unsigned char m_screenActive; }; }
static NHeroInformationScreens::CManager g_screens;
class CPlayerGui { public:
    unsigned char _pad00[0x18];
    CSubtitleRenderer m_subtitleRenderer;
    NHeroInformationScreens::CManager* GetPHeroInfoScreensManager() const { return &g_screens; }
};
static CPlayerGui g_gui; static CPlayerGui* g_pPlayerGui = &g_gui;
class CPlayer { public: unsigned char _pad_0x00[0x28]; signed char m_playerNumber; };
static CPlayer g_player;
class CWorld { public: CPlayer* GetCurrentPlayer() const { return &g_player; } };
static CWorld g_world;
class CHeroMarriageDef { public: unsigned char _pad00[0xa4]; unsigned char TextGuiWifeDivorceOccurred[4]; };
static int g_globalUpdates = 0;
class CTCWife { public:
    virtual void v00() {}
    void*             ParentThing;
    unsigned char     _pad_0x08[4];
    CHeroMarriageDef* PHeroMarriageDef;
    unsigned char     _pad_0x10[0x29 - 0x10];
    bool              WaitingForDivorce;
    void AddToGlobalUpdateTCs() { ++g_globalUpdates; }
    const CWorld* GetWorld() const { return &g_world; }
    void InitiateDivorce();
};
void CTCWife::InitiateDivorce()
{
    if (g_pPlayerGui->GetPHeroInfoScreensManager()->m_screenActive)
        return;
    g_pPlayerGui->m_subtitleRenderer.SetText(g_pMainGameComponent->m_pFontBank,
        *(const CCharString*)PHeroMarriageDef->TextGuiWifeDivorceOccurred, 0x22, 0.0f, true, true);
    WaitingForDivorce = true;
    AddToGlobalUpdateTCs();
    CGameEvent gameEvent(9);
    gameEvent.Player = GetWorld()->GetCurrentPlayer()->m_playerNumber;
    gameEvent.AddBool(true);
    g_pMainGameComponent->AddGameEvent(gameEvent);
}
static int fail(const char* why) { std::printf("006cef20_TEST FAIL: %s\n", why); return 1; }
int main()
{
    static CHeroMarriageDef def; static const char* divorceText = "TEXT_GUI_WIFE_DIVORCE_OCCURRED";
    std::memcpy(def.TextGuiWifeDivorceOccurred, &divorceText, sizeof(divorceText));
    g_mainGame.m_pFontBank = &g_font; g_player.m_playerNumber = 2;
    static CTCWife wife; wife.PHeroMarriageDef = &def; wife.WaitingForDivorce = false;
    // 1. a hero information screen is up: nothing happens
    g_screens.m_screenActive = 1;
    wife.InitiateDivorce();
    if (wife.WaitingForDivorce || g_subtitles || g_events || g_globalUpdates) return fail("acted while a hero screen was up");
    // 2. screens closed: subtitle, waiting flag, global update, pause event
    g_screens.m_screenActive = 0;
    wife.InitiateDivorce();
    if (g_subtitles != 1 || g_subtitleFont != &g_font || g_subtitleText != divorceText || g_subtitleColour != 0x22 || g_subtitleTime != 0.0f || !g_subA || !g_subB) return fail("subtitle call");
    if (!wife.WaitingForDivorce || g_globalUpdates != 1) return fail("waiting state");
    if (g_events != 1 || g_eventType != 9 || g_eventPlayer != 2 || g_eventFlag != 1 || !g_eventValid) return fail("pause game event");
    std::printf("006cef20_TEST PASS\n");
    return 0;
}