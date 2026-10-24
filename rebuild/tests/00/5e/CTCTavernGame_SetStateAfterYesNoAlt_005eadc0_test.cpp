// Behaviour test for CTCTavernGame::SetStateAfterYesNoAlt (0x005EADC0): with the skip flag set the flag is cleared and
// the skip handler runs, nothing is shown; otherwise the selection sound plays, the five-string subtitle overload shows
// title / "" / text / noText / altText (0x84, 0.0f, false, true) in the main font, a type-9 game event with a one-byte
// false payload is queued, Yes/No/Alt states are stored and the game enters 0x2E remembering the previous state -- unless
// a skip is raised meanwhile, which takes the skip path. Every string built or passed by value is destroyed.
#include <stdio.h>
#include <string.h>
#include <new>

// ---- reconstruction under test (verbatim copy of src/005eadc0.cpp) ----
// Leaf: ?SetStateAfterYesNoAlt@CTCTavernGame@@
// Flags: /GS
// Grade: RELOCATION_MATCH 354/354 on the real span 0x005EADC0..0x005EAF1F (ret 1Ch), /GS
// CTCTavernGame::SetStateAfterYesNoAlt(CWideString title, CWideString text, ETavernGameStates yesState, CWideString noText,
//   ETavernGameStates noState, CWideString altText, ETavernGameStates altState) -- show a three-way (yes / no / alternative)
//   prompt and remember the three follow-up states
// Retail: Fable.exe @ 0x005EADC0, 354 bytes, `ret 1Ch` at 0x005EAF1F, /GS (cookie 0x0139C8A8, __security_check_cookie
//   0x00BFE9F9). Name and signature: Ego_r.pdb `?SetStateAfterYesNoAlt@CTCTavernGame@@IAEXVCWideString@@0W4ETavernGameStates@1@0101@Z`.
// Catalog row: only the first 22 bytes (the /GS prologue up to `cmp al,bl`); the landed source was an asm_bake of those bytes.
// Body: the three-option sibling of SetStateAfterYesNo 0x005EAF30 (the model): skip flag 0x013BAE1C -> clear it and run
//   own slot 30 (+0x78); else the selection-change sound, the subtitle renderer's five-string SetText overload 0x00492973
//   (title, "", text, noText, altText, 0x84, 0.0f, false, true), the type-9 game event with a false payload, then
//   YesState / NoState / AltState (+0xA8 / +0xAC / +0xB0) take the three state arguments and the game state moves to
//   0x2E keeping the previous one; a skip raised meanwhile takes the skip path. The four by-value CWideStrings are
//   destroyed on return. Parameter names noText / altText are descriptive (the PDB keeps only types).
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);           // 0x0099EBF0
    ~CCharString();                                            // 0x0099EAE0
};
struct CWideString {
    void* PStringData;
    CWideString(const unsigned short* text);                   // 0x0099B6B0
    CWideString(const CWideString& other);
    ~CWideString();                                            // 0x0099B510
};
struct CFontBank;
struct CSubtitleRenderer {
    void SetText(CFontBank* font, const CWideString& title, const CWideString& speaker, const CWideString& text,
                 const CWideString& altText, unsigned long flags, float time, bool b0, bool b1);   // 0x00492F69
    void SetText(CFontBank* font, const CWideString& title, const CWideString& speaker, const CWideString& text,
                 const CWideString& altText, const CWideString& altText2, unsigned long flags, float time, bool b0,
                 bool b1);                                     // 0x00492973
};
struct CGameEventData {
    unsigned char Bytes[0x24];
    void Copy(long size, const void* data);                    // 0x009F1760
};
struct CGameEvent {
    long           Type;                                       // +0x00
    unsigned char  Flag;                                       // +0x04
    unsigned char  _pad05[0x20];
    unsigned char  B25, B26, B27;                              // +0x25..+0x27
    CGameEvent(long type) : Type(type), Flag(0xff), B25(0), B26(1), B27(0) { }
    template <class T> void SetData(T value) { ((CGameEventData*)this)->Copy(sizeof(T), &value); }
};
struct CMainGameComponent {
    unsigned char _pad[0x1614c];
    CFontBank*    PFont;                                       // +0x1614c
    void AddGameEvent(CGameEvent* event);                      // 0x0041649C
};
struct CSubtitleOwner { unsigned char _pad[0x18]; CSubtitleRenderer Renderer; };
extern CMainGameComponent* g_pMainGameComponent;               // 0x013B86A0
extern CSubtitleOwner* g_pSubtitleOwner;                       // 0x013B8790
extern bool g_bTavernSkipYesNo;                                // 0x013BAE1C
enum ETavernGameStates { TAVERN_STATE_YES_NO = 0x2d, TAVERN_STATE_YES_NO_ALT = 0x2e };
class CTCTavernGame {
public:
    virtual void v000() {} virtual void v001() {} virtual void v002() {} virtual void v003() {} virtual void v004() {} virtual void v005() {}
    virtual void v006() {} virtual void v007() {} virtual void v008() {} virtual void v009() {} virtual void v010() {} virtual void v011() {}
    virtual void v012() {} virtual void v013() {} virtual void v014() {} virtual void v015() {} virtual void v016() {} virtual void v017() {}
    virtual void v018() {} virtual void v019() {} virtual void v020() {} virtual void v021() {} virtual void v022() {} virtual void v023() {}
    virtual void v024() {} virtual void v025() {} virtual void v026() {} virtual void v027() {} virtual void v028() {} virtual void v029() {}
    virtual void OnYesNoSkipped();                             // slot 30 (+0x78) (name descriptive)
    void PlaySound(const CCharString& name);                   // 0x005EA950
    void SetStateAfterYesNoAlt(CWideString title, CWideString text, ETavernGameStates yesState, CWideString noText,
                               ETavernGameStates noState, CWideString altText, ETavernGameStates altState);
    unsigned char     _pad004[0xa4];
    ETavernGameStates YesState;                                // +0xa8
    ETavernGameStates NoState;                                 // +0xac
    ETavernGameStates AltState;                                // +0xb0
    unsigned char     _pad0b4[0xc];
    ETavernGameStates State;                                   // +0xc0
    ETavernGameStates PreviousState;                           // +0xc4
};
void CTCTavernGame::SetStateAfterYesNoAlt(CWideString title, CWideString text, ETavernGameStates yesState, CWideString noText,
                                          ETavernGameStates noState, CWideString altText, ETavernGameStates altState)
{
    if (g_bTavernSkipYesNo) {
        g_bTavernSkipYesNo = false;
        OnYesNoSkipped();
    } else {
        PlaySound("TAVERN_GAME_OBJECT_SELECTION_CHANGE");
        g_pSubtitleOwner->Renderer.SetText(g_pMainGameComponent->PFont, title, L"", text, noText, altText, 0x84, 0.0f, false, true);
        CGameEvent event(9);
        event.SetData(false);
        g_pMainGameComponent->AddGameEvent(&event);
        YesState = yesState;
        NoState = noState;
        AltState = altState;
        if (g_bTavernSkipYesNo) {
            g_bTavernSkipYesNo = false;
            OnYesNoSkipped();
        } else {
            PreviousState = State;
            State = TAVERN_STATE_YES_NO_ALT;
        }
    }
}

// ---- harness ----
static int g_live, g_skipped, g_shows, g_shows4, g_events;
static const char* g_sound;
static const unsigned short* g_shown[5];
static CFontBank* g_font;
static unsigned long g_flags;
static float g_time = -1.0f;
static bool g_b0 = true, g_b1 = false, g_raiseSkipOnEvent;
static long g_eventType = -1, g_eventSize = -1;
static int g_eventData = -1;
static unsigned char g_fontStore[4];
static unsigned char g_mgcStore[sizeof(CMainGameComponent)];
static CSubtitleOwner g_owner;
CMainGameComponent* g_pMainGameComponent = (CMainGameComponent*)g_mgcStore;
CSubtitleOwner* g_pSubtitleOwner = &g_owner;
bool g_bTavernSkipYesNo;

CCharString::CCharString(const char* text, long) { PStringData = (void*)text; ++g_live; }
CCharString::~CCharString() { --g_live; }
CWideString::CWideString(const unsigned short* text) { PStringData = (void*)text; ++g_live; }
CWideString::CWideString(const CWideString& other) { PStringData = other.PStringData; ++g_live; }
CWideString::~CWideString() { --g_live; }
void CSubtitleRenderer::SetText(CFontBank*, const CWideString&, const CWideString&, const CWideString&, const CWideString&,
                                unsigned long, float, bool, bool) { ++g_shows4; }
void CSubtitleRenderer::SetText(CFontBank* font, const CWideString& title, const CWideString& speaker, const CWideString& text,
                                const CWideString& altText, const CWideString& altText2, unsigned long flags, float time,
                                bool b0, bool b1)
{
    if (this != &g_owner.Renderer) printf("FAIL renderer this\n");
    g_font = font; g_shown[0] = (const unsigned short*)title.PStringData; g_shown[1] = (const unsigned short*)speaker.PStringData;
    g_shown[2] = (const unsigned short*)text.PStringData; g_shown[3] = (const unsigned short*)altText.PStringData;
    g_shown[4] = (const unsigned short*)altText2.PStringData;
    g_flags = flags; g_time = time; g_b0 = b0; g_b1 = b1; ++g_shows;
}
void CGameEventData::Copy(long size, const void* data) { g_eventSize = size; g_eventData = *(const unsigned char*)data; }
void CMainGameComponent::AddGameEvent(CGameEvent* event)
{
    g_eventType = event->Type; ++g_events;
    if (event->Flag != 0xff || event->B25 != 0 || event->B26 != 1 || event->B27 != 0) printf("FAIL event header\n"), g_eventType = -2;
    if (g_raiseSkipOnEvent) g_bTavernSkipYesNo = true;
}
void CTCTavernGame::OnYesNoSkipped() { ++g_skipped; }
void CTCTavernGame::PlaySound(const CCharString& name) { g_sound = (const char*)name.PStringData; }

static const unsigned short T1[] = { 'T', 0 }, T2[] = { 'X', 0 }, T3[] = { 'N', 0 }, T4[] = { 'A', 0 };
static int g_fail;
static void check(int ok, const char* what) { if (!ok) { printf("FAIL %s\n", what); g_fail = 1; } }

int main()
{
    static unsigned char gameStore[sizeof(CTCTavernGame)];
    CTCTavernGame* game = new (gameStore) CTCTavernGame;
    g_pMainGameComponent->PFont = (CFontBank*)g_fontStore;
    game->State = (ETavernGameStates)7; game->PreviousState = (ETavernGameStates)0;
    game->YesState = game->NoState = game->AltState = (ETavernGameStates)99;

    game->SetStateAfterYesNoAlt(CWideString(T1), CWideString(T2), (ETavernGameStates)3, CWideString(T3), (ETavernGameStates)4,
                                CWideString(T4), (ETavernGameStates)5);
    check(g_skipped == 0, "no skip");
    check(g_sound != 0 && strcmp(g_sound, "TAVERN_GAME_OBJECT_SELECTION_CHANGE") == 0, "selection sound");
    check(g_shows == 1 && g_shows4 == 0, "five-string SetText overload");
    check(g_font == (CFontBank*)g_fontStore, "main font");
    check(g_shown[0] == T1 && g_shown[1] != 0 && g_shown[1][0] == 0 && g_shown[2] == T2 && g_shown[3] == T3 && g_shown[4] == T4, "strings in order");
    check(g_flags == 0x84 && g_time == 0.0f && !g_b0 && g_b1, "SetText flags");
    check(g_events == 1 && g_eventType == 9 && g_eventSize == 1 && g_eventData == 0, "type-9 event with a false byte");
    check(game->YesState == 3 && game->NoState == 4 && game->AltState == 5, "three states stored");
    check(game->State == 0x2e && game->PreviousState == 7, "enters 0x2E keeping the previous state");
    check(g_live == 0, "strings destroyed (prompt path)");

    g_bTavernSkipYesNo = true; g_shows = 0;
    game->SetStateAfterYesNoAlt(CWideString(T1), CWideString(T2), (ETavernGameStates)8, CWideString(T3), (ETavernGameStates)9,
                                CWideString(T4), (ETavernGameStates)10);
    check(g_skipped == 1 && !g_bTavernSkipYesNo && g_shows == 0 && g_events == 1, "skip path");
    check(game->YesState == 3 && game->State == 0x2e, "skip path leaves the states");
    check(g_live == 0, "strings destroyed (skip path)");

    g_raiseSkipOnEvent = true; game->State = (ETavernGameStates)12;
    game->SetStateAfterYesNoAlt(CWideString(T1), CWideString(T2), (ETavernGameStates)8, CWideString(T3), (ETavernGameStates)9,
                                CWideString(T4), (ETavernGameStates)10);
    check(g_skipped == 2 && !g_bTavernSkipYesNo && g_shows == 1 && g_events == 2, "skip raised while prompting");
    check(game->YesState == 8 && game->NoState == 9 && game->AltState == 10, "states stored before the late skip");
    check(game->State == 12 && game->PreviousState == 7, "late skip keeps the game state");
    check(g_live == 0, "strings destroyed (late skip)");
    if (g_fail) return 1;
    printf("005eadc0_TEST PASS\n");
    return 0;
}