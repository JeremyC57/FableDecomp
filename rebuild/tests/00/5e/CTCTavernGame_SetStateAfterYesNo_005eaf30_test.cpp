// Behaviour test for 0x005EAF30 CTCTavernGame::SetStateAfterYesNo: with the skip flag set the flag is cleared and the
// skip handler runs; otherwise the selection sound plays, the subtitle shows title / "" / text / alt text (0x84, 0, false,
// true) in the main font, a type-9 game event with a false payload is queued, Yes/No/Alt states are stored and the game
// enters state 0x2D remembering the previous state -- unless a skip was raised meanwhile, which takes the skip path.
#include <stdio.h>
#include <string.h>
static int g_live = 0;
class CCharString { public:
    const char* PStringData;
    CCharString(const char* text, long = -1) : PStringData(text) { ++g_live; }
    ~CCharString() { --g_live; }
};
class CWideString { public:
    const unsigned short* PStringData;
    CWideString(const unsigned short* text) : PStringData(text) { ++g_live; }
    CWideString(const CWideString& o) : PStringData(o.PStringData) { ++g_live; }
    ~CWideString() { --g_live; }
};
class CFontBank { public: int id; };
static const unsigned short* g_shown[4]; static unsigned long g_flags = 0; static CFontBank* g_font = 0; static int g_shows = 0;
static bool g_b0 = true, g_b1 = false; static float g_time = -1.0f;
static bool g_raiseSkipOnEvent = false;
class CSubtitleRenderer { public:
    void SetText(CFontBank* font, const CWideString& title, const CWideString& speaker, const CWideString& text,
                 const CWideString& altText, unsigned long flags, float time, bool b0, bool b1)
    { g_font = font; g_shown[0] = title.PStringData; g_shown[1] = speaker.PStringData; g_shown[2] = text.PStringData;
      g_shown[3] = altText.PStringData; g_flags = flags; g_time = time; g_b0 = b0; g_b1 = b1; ++g_shows; }
};
struct CGameEventData { unsigned char Bytes[0x28]; int Size; unsigned char Data;
    void Copy(long size, const void* data) { Size = size; Data = *(const unsigned char*)data; } };
class CGameEvent { public:
    int Type; unsigned char Flag; unsigned char _pad05[0x20]; unsigned char B25, B26, B27; int Size; unsigned char Data;
    CGameEvent(long type) : Type(type), Flag(0xff), B25(0), B26(1), B27(0) { }
    template <class T> void SetData(T value) { ((CGameEventData*)this)->Copy(sizeof(T), &value); }
};
static long g_eventType = -1, g_eventSize = -1; static int g_eventData = -1;
extern bool g_bTavernSkipYesNo;
class CMainGameComponent { public: CFontBank* PFont;
    void AddGameEvent(CGameEvent* e) { g_eventType = e->Type; g_eventSize = e->Size; g_eventData = e->Data; if (g_raiseSkipOnEvent) g_bTavernSkipYesNo = true; } };
struct CSubtitleOwner { CSubtitleRenderer Renderer; };
static CFontBank g_mainFont; static CMainGameComponent g_mgc; static CSubtitleOwner g_owner;
static CMainGameComponent* g_pMainGameComponent = &g_mgc;
static CSubtitleOwner* g_pSubtitleOwner = &g_owner;
bool g_bTavernSkipYesNo = false;
enum ETavernGameStates { TAVERN_STATE_YES_NO = 0x2d };
static const unsigned short g_empty[] = { 0 };
static int g_skipped = 0; static const char* g_sound = 0;
class CTCTavernGame {
public:
    virtual void OnYesNoSkipped() { ++g_skipped; }
    void PlaySound(const CCharString& name) { g_sound = name.PStringData; }
    void SetStateAfterYesNo(CWideString title, CWideString text, ETavernGameStates yesState, CWideString altText,
                            ETavernGameStates noState);
    ETavernGameStates YesState, NoState, AltState, State, PreviousState;
};
void CTCTavernGame::SetStateAfterYesNo(CWideString title, CWideString text, ETavernGameStates yesState, CWideString altText,
                                       ETavernGameStates noState)
{
    if (g_bTavernSkipYesNo) {
        g_bTavernSkipYesNo = false;
        OnYesNoSkipped();
    } else {
        PlaySound("TAVERN_GAME_OBJECT_SELECTION_CHANGE");
        g_pSubtitleOwner->Renderer.SetText(g_pMainGameComponent->PFont, title, (const unsigned short*)g_empty, text, altText, 0x84, 0.0f, false, true);
        CGameEvent event(9);
        event.SetData(false);
        g_pMainGameComponent->AddGameEvent(&event);
        YesState = yesState;
        NoState = noState;
        AltState = (ETavernGameStates)0;
        if (g_bTavernSkipYesNo) {
            g_bTavernSkipYesNo = false;
            OnYesNoSkipped();
        } else {
            PreviousState = State;
            State = TAVERN_STATE_YES_NO;
        }
    }
}

static const unsigned short T1[] = { 'T', 0 }, T2[] = { 'X', 0 }, T3[] = { 'A', 0 };
int main()
{
    g_mgc.PFont = &g_mainFont;
    CTCTavernGame game; game.State = (ETavernGameStates)7; game.PreviousState = (ETavernGameStates)0;
    game.YesState = game.NoState = game.AltState = (ETavernGameStates)99;
    game.SetStateAfterYesNo(CWideString(T1), CWideString(T2), (ETavernGameStates)3, CWideString(T3), (ETavernGameStates)4);
    bool ok = g_skipped == 0 && g_sound && strcmp(g_sound, "TAVERN_GAME_OBJECT_SELECTION_CHANGE") == 0 && g_shows == 1;
    ok = ok && g_font == &g_mainFont && g_shown[0] == T1 && g_shown[1][0] == 0 && g_shown[2] == T2 && g_shown[3] == T3;
    ok = ok && g_flags == 0x84 && g_time == 0.0f && !g_b0 && g_b1 && g_eventType == 9 && g_eventSize == 1 && g_eventData == 0;
    ok = ok && game.YesState == 3 && game.NoState == 4 && game.AltState == 0 && game.State == 0x2d && game.PreviousState == 7;
    ok = ok && g_live == 0;
    g_bTavernSkipYesNo = true; g_shows = 0;
    game.SetStateAfterYesNo(CWideString(T1), CWideString(T2), (ETavernGameStates)5, CWideString(T3), (ETavernGameStates)6);
    ok = ok && g_skipped == 1 && !g_bTavernSkipYesNo && g_shows == 0 && game.YesState == 3 && g_live == 0;
    g_raiseSkipOnEvent = true; game.State = (ETavernGameStates)8;
    game.SetStateAfterYesNo(CWideString(T1), CWideString(T2), (ETavernGameStates)5, CWideString(T3), (ETavernGameStates)6);
    ok = ok && g_skipped == 2 && !g_bTavernSkipYesNo && g_shows == 1 && game.YesState == 5 && game.NoState == 6 && game.State == 8 && g_live == 0;
    if (!ok) { printf("005eaf30_TEST FAIL\n"); return 1; }
    printf("005eaf30_TEST PASS\n");
    return 0;
}
