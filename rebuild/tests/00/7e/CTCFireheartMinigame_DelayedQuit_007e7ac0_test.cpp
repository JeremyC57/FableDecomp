// Behaviour test for CTCFireheartMinigame::DelayedQuit (retail 0x007E7AC0): model copy + recording stubs.
#include <cstdio>
#include <cstring>
#include <stddef.h>

static int g_live = 0;
static char g_sound[64];
static int g_soundCalls = 0;
struct CCharString {
    const char* PStringData;
    CCharString(const char* text, long) : PStringData(text) { ++g_live; }
    ~CCharString() { --g_live; }
};
class CWideString {
public:
    long Id;                                                // test: the text id (-1 for a literal)
    CWideString(const wchar_t*) : Id(-1) { ++g_live; }
    CWideString(const CWideString& o) : Id(o.Id) { ++g_live; }
    explicit CWideString(long id) : Id(id) { ++g_live; }
    ~CWideString() { --g_live; }
};
class CFontBank { public: int tag; };
static long g_setText[4]; static unsigned long g_flags; static float g_time; static bool g_b1, g_b2; static CFontBank* g_font; static int g_setCalls = 0;
static int g_liveAtSetText = 0;
class CSubtitleRenderer {
public:
    int tag;
    void SetText(CFontBank* font, const CWideString& text, const CWideString& name, const CWideString& button1,
                 const CWideString& button2, unsigned long flags, float time, bool b1, bool b2)
    {
        ++g_setCalls; g_font = font; g_setText[0] = text.Id; g_setText[1] = name.Id; g_setText[2] = button1.Id;
        g_setText[3] = button2.Id; g_flags = flags; g_time = time; g_b1 = b1; g_b2 = b2; g_liveAtSetText = g_live;
    }
};
struct CPlayerGui {
    CSubtitleRenderer SubtitleRenderer;
};
CPlayerGui* g_pPlayerGui;
static unsigned char g_payload[8]; static unsigned long g_payloadLen = 0;
struct CMemoryBuffer {
    void Copy(unsigned long length, const void* data) { std::memcpy(g_payload, data, length); g_payloadLen = length; }
};
struct CGameEvent : CMemoryBuffer {
    long          Type;
    signed char   Player;
    unsigned char Data[32];
    unsigned char EndPos;
    bool          Valid;
    bool          Replacement;
    CGameEvent(long type) : Type(type), Player(-1), EndPos(0), Valid(true), Replacement(false) { }
    void AddBOOL(bool value) { Copy(1, &value); }
};
static long g_eventType = 0; static int g_eventPlayer = 0; static bool g_eventValid = false; static int g_eventCalls = 0;
struct CMainGameComponent {
    CFontBank* Font;
    void AddGameEvent(const CGameEvent& event) { ++g_eventCalls; g_eventType = event.Type; g_eventPlayer = event.Player; g_eventValid = event.Valid && !event.Replacement && event.EndPos == 0; }
};
CMainGameComponent* g_pMainGameComponent;
struct CFireheartMinigameDef {
    unsigned long QuitText;
    unsigned long YesText;
    unsigned long NoText;
};
template <class T> struct CDefPointer {
    T* Object;
    T* operator->() const { return Object; }
};
class CTCFireheartMinigame {
public:
    bool QuitWithDelay;
    CDefPointer<const CFireheartMinigameDef> PDef;
    unsigned long PlaySound(const CCharString& name) { ++g_soundCalls; std::strcpy(g_sound, name.PStringData); return 0; }
    CWideString GetText(unsigned long id) const { return CWideString((long)id); }
    void DelayedQuit();
};

void CTCFireheartMinigame::DelayedQuit()
{
    if (QuitWithDelay)
        return;
    QuitWithDelay = true;
    {
        CCharString sound("TAVERN_GAME_OBJECT_SELECTION_CHANGE", -1);
        PlaySound(sound);
    }
    {
        CWideString empty(L"");
        const CFireheartMinigameDef* def = PDef.operator->();
        CFontBank* font = g_pMainGameComponent->Font;
        CPlayerGui* gui = g_pPlayerGui;
        gui->SubtitleRenderer.SetText(font, GetText(def->QuitText), empty, GetText(def->YesText), GetText(def->NoText),
                                      0x1004, 0.0f, false, true);
    }
    {
        CGameEvent event(9);
        event.AddBOOL(true);
        g_pMainGameComponent->AddGameEvent(event);
    }
}

int main()
{
    int fail = 0;
    CFontBank font; CPlayerGui gui; CMainGameComponent main;
    main.Font = &font; g_pMainGameComponent = &main; g_pPlayerGui = &gui;
    CFireheartMinigameDef def = { 101, 202, 303 };
    CTCFireheartMinigame game; game.QuitWithDelay = false; game.PDef.Object = &def;

    game.DelayedQuit();
    if (!game.QuitWithDelay) fail = 1;
    if (g_soundCalls != 1 || std::strcmp(g_sound, "TAVERN_GAME_OBJECT_SELECTION_CHANGE") != 0) fail = 2;
    if (g_setCalls != 1 || g_font != &font) fail = 3;
    if (g_setText[0] != 101 || g_setText[1] != -1 || g_setText[2] != 202 || g_setText[3] != 303) fail = 4;
    if (g_flags != 0x1004 || g_time != 0.0f || g_b1 || !g_b2) fail = 5;
    if (g_liveAtSetText < 4) fail = 6;                      // empty + three GetText results alive during the call
    if (g_eventCalls != 1 || g_eventType != 9 || g_eventPlayer != -1 || !g_eventValid) fail = 7;
    if (g_payloadLen != 1 || g_payload[0] != 1) fail = 8;
    if (g_live != 0) fail = 9;                              // every string destroyed

    // second call: already quitting, nothing happens
    game.DelayedQuit();
    if (g_soundCalls != 1 || g_setCalls != 1 || g_eventCalls != 1) fail = 10;

    if (fail) { std::printf("007e7ac0_TEST FAIL %d\n", fail); return 1; }
    std::printf("007e7ac0_TEST PASS\n");
    return 0;
}