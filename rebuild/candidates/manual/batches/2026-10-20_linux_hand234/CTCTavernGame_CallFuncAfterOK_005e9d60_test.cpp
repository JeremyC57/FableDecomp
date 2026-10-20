// Behaviour test for 0x005e9d60 (copy of the source logic over spy callees).
#include <cstdio>
#include <stddef.h>
#ifndef _MSC_VER
#define __fastcall __attribute__((fastcall))
#endif
static void __fastcall YesCb() {}
static void __fastcall NoCb() {}
typedef long ETavernGameStates;
static bool g_bTavernGameQuitRequested = false;
static int g_liveStrings = 0;
class CWideString {
public:
    const wchar_t* text;
    CWideString(const wchar_t* t) : text(t) { ++g_liveStrings; }
    CWideString(const CWideString& o) : text(o.text) { ++g_liveStrings; }
    ~CWideString() { --g_liveStrings; }
};
class CFontBank;
static CFontBank* g_font; static const wchar_t* g_text; static const wchar_t* g_name; static const wchar_t* g_b1; static const wchar_t* g_b2;
static unsigned long g_flags; static float g_time; static bool g_bb1, g_bb2;
class CSubtitleRenderer {
public:
    void SetText(CFontBank* font, const CWideString& text, const CWideString& name, const CWideString& button1,
                 const CWideString& button2, unsigned long flags, float time, bool b1, bool b2)
    { g_font = font; g_text = text.text; g_name = name.text; g_b1 = button1.text; g_b2 = button2.text; g_flags = flags; g_time = time; g_bb1 = b1; g_bb2 = b2; }
};
struct CPlayerGui {
    unsigned char _pad00[0x18];
    CSubtitleRenderer SubtitleRenderer;                 // +0x18
};
static CPlayerGui g_gui; static CPlayerGui* g_pPlayerGui = &g_gui;
struct CMemoryBuffer {
    unsigned long copied; unsigned char last;
    void Copy(unsigned long length, const void* data) { copied = length; last = *(const unsigned char*)data; }
};
struct CGameEvent : CMemoryBuffer {
    long          Type;                                 // +0x00
    signed char   Player;                               // +0x04
    unsigned char Data[32];                             // +0x05
    unsigned char EndPos;                               // +0x25
    bool          Valid;                                // +0x26
    bool          Replacement;                          // +0x27
    CGameEvent(long type) : Type(type), Player(-1), EndPos(0), Valid(true), Replacement(false) { }
    void AddBOOL(bool value) { Copy(1, &value); }
};
static int g_events = 0; static long g_eventType = 0; static unsigned char g_eventByte = 0xff;
struct CMainGameComponent {
    CFontBank* Font;                                    // +0x1614c
    void AddGameEvent(const CGameEvent& event) { g_events++; g_eventType = event.Type; g_eventByte = event.last; }
};
static CMainGameComponent g_mgcObj; static CMainGameComponent* g_pMainGameComponent = &g_mgcObj;
typedef void (__fastcall *TTavernGameFunc)();
static int g_left = 0;
struct CTCTavernGame {
    void LeaveGame() { ++g_left; }
    ETavernGameStates YesState;                         // +0xa8
    ETavernGameStates NoState;                          // +0xac
    ETavernGameStates AltState;                         // +0xb0
    TTavernGameFunc   YesFunc;                          // +0xb4
    TTavernGameFunc   NoFunc;                           // +0xb8
    TTavernGameFunc   AltFunc;                          // +0xbc
    ETavernGameStates TavernGameState;                  // +0xc0
    ETavernGameStates PreviousTavernGameState;          // +0xc4
    void CallFuncAfterOK(CWideString text, CWideString okText, TTavernGameFunc okFunc);
};

void CTCTavernGame::CallFuncAfterOK(CWideString text, CWideString okText, TTavernGameFunc okFunc)
{
    g_pPlayerGui->SubtitleRenderer.SetText(g_pMainGameComponent->Font, text, L"", okText, L"", 0xa2, 0.0f, false, true);
    {
        CGameEvent event(9);
        event.AddBOOL(false);
        g_pMainGameComponent->AddGameEvent(event);
    }
    YesFunc = okFunc;
    if (g_bTavernGameQuitRequested) {
        g_bTavernGameQuitRequested = false;
        LeaveGame();
        return;
    }
    PreviousTavernGameState = TavernGameState;
    TavernGameState = 44;
}

static int fail() { std::printf("005e9d60_TEST FAIL\n"); return 1; }
int main()
{
    CTCTavernGame game; game.TavernGameState = 7; game.PreviousTavernGameState = 0;
    game.CallFuncAfterOK(CWideString(L"You win"), CWideString(L"OK"), &YesCb);
    if (g_text == 0 || g_text[0] == 0 || g_time != 0.0f || g_bb1 || !g_bb2 || g_font != g_mgcObj.Font) return fail();
    if (g_events != 1 || g_eventType != 9 || g_eventByte != 0 || g_left != 0 || g_liveStrings != 0) return fail();
    if (g_flags != 0xa2 || game.YesFunc != &YesCb || g_name[0] != 0 || g_b2[0] != 0 || g_b1[0] != L'O') return fail();
    if (game.TavernGameState != 44 || game.PreviousTavernGameState != 7) return fail();
    g_bTavernGameQuitRequested = true; game.TavernGameState = 3;
    game.CallFuncAfterOK(CWideString(L"You win"), CWideString(L"OK"), &YesCb);
    if (g_left != 1 || g_bTavernGameQuitRequested || game.TavernGameState != 3 || g_liveStrings != 0) return fail();
    std::printf("005e9d60_TEST PASS\n");
    return 0;
}
