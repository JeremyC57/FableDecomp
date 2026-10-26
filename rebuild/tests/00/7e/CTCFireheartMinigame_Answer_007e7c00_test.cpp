// Behaviour test for CTCFireheartMinigame::Answer @ 0x007E7C00: with no pending quit prompt (QuitWithDelay clear)
// nothing happens; otherwise a "yes" sets ForceQuit and calls Quit first, then (either answer) game event 9 with one
// zero byte is queued on the main game component, the player GUI's subtitle renderer (+0x18) is cleared with false,
// and the pending flag is dropped. Checks the three paths, call order, the event contents and the Clear argument.
#include <cstdio>
#include <cstring>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d\n", __LINE__); } } while (0)

struct CMemoryBuffer {
    void Copy(unsigned long length, const void* data);   // 0x009F1760
};

class CGameEvent : public CMemoryBuffer { public:
    long          Type;                           // +0x00
    signed char   Player;                         // +0x04
    unsigned char Data[32];                       // +0x05
    unsigned char EndPos;                         // +0x25
    bool          Valid;                          // +0x26
    bool          Replacement;                    // +0x27
    CGameEvent(long type) : Type(type), Player(-1), EndPos(0), Valid(true), Replacement(false) { }
    void AddUBYTE(unsigned char value) { Copy(1, &value); }
};

class CMainGameComponent { public:
    void AddGameEvent(const CGameEvent& event);   // 0x0041649C
};
extern CMainGameComponent* g_pMainGameComponent;  // 0x013B86A0

class CSubtitleRenderer { public:
    void Clear(bool immediately);                 // 0x00492BAB
};
class CPlayerGui { public:
    unsigned char     _pad00[0x18];
    CSubtitleRenderer SubtitleRenderer;           // +0x18
};
extern CPlayerGui* g_pPlayerGui;                  // 0x013B8790

enum { GAME_EVENT_FIREHEART_ANSWERED = 9 };

class CThing;

class CTCFireheartMinigame { public:
    virtual void v00();
    CThing*       ParentThing;                    // +0x04 (CTCBase)
    unsigned char _pad08[0x4c - 0x08];
    bool          InSession;                      // +0x4c
    bool          ForceQuit;                      // +0x4d
    unsigned char _pad4e[0x88 - 0x4e];
    bool          QuitWithDelay;                  // +0x88
    void Quit();                                  // 0x007E7A40
    void Answer(bool yes);
};

void CTCFireheartMinigame::Answer(bool yes)
{
    if (QuitWithDelay)
    {
        if (yes)
        {
            ForceQuit = true;
            Quit();
        }
        CGameEvent event(GAME_EVENT_FIREHEART_ANSWERED);
        event.AddUBYTE(0);
        g_pMainGameComponent->AddGameEvent(event);
        g_pPlayerGui->SubtitleRenderer.Clear(false);
        QuitWithDelay = false;
    }
}

// ---- instrumented stubs ----
static int g_seq = 0, g_quitAt = 0, g_eventAt = 0, g_clearAt = 0, g_quits = 0, g_events = 0, g_clears = 0;
static bool g_clearArg = true, g_forceAtQuit = false;
static CGameEvent g_last(-5);
static CTCFireheartMinigame* g_expectGame = 0;
void CMemoryBuffer::Copy(unsigned long length, const void* data)
{
    CGameEvent* e = static_cast<CGameEvent*>(this);
    std::memcpy(e->Data + e->EndPos, data, length); e->EndPos = (unsigned char)(e->EndPos + length);
}
void CMainGameComponent::AddGameEvent(const CGameEvent& event)
{ ++g_events; g_eventAt = ++g_seq; g_last = event; if (this != g_pMainGameComponent) g_fail = 1; }
CMainGameComponent* g_pMainGameComponent;
CPlayerGui* g_pPlayerGui;
void CSubtitleRenderer::Clear(bool immediately)
{ ++g_clears; g_clearAt = ++g_seq; g_clearArg = immediately; if (this != &g_pPlayerGui->SubtitleRenderer) g_fail = 1; }
void CTCFireheartMinigame::v00() { }
void CTCFireheartMinigame::Quit() { ++g_quits; g_quitAt = ++g_seq; g_forceAtQuit = ForceQuit; if (this != g_expectGame) g_fail = 1; }

static void reset() { g_seq = g_quitAt = g_eventAt = g_clearAt = g_quits = g_events = g_clears = 0; g_clearArg = true; g_forceAtQuit = false; g_last = CGameEvent(-5); }
static bool eventOk() { return g_last.Type == 9 && g_last.Player == -1 && g_last.EndPos == 1 && g_last.Data[0] == 0 && g_last.Valid && !g_last.Replacement; }

int main()
{
    CMainGameComponent mgc; g_pMainGameComponent = &mgc;
    CPlayerGui gui; g_pPlayerGui = &gui;
    CTCFireheartMinigame game; g_expectGame = &game;
    game.InSession = true; game.ForceQuit = false; game.QuitWithDelay = false;

    // No pending prompt: nothing at all.
    reset();
    game.Answer(true);
    CHECK(g_quits == 0 && g_events == 0 && g_clears == 0 && !game.ForceQuit && !game.QuitWithDelay && game.InSession);

    // Pending, "no": no quit, event + clear, flag dropped.
    reset(); game.QuitWithDelay = true;
    game.Answer(false);
    CHECK(g_quits == 0 && !game.ForceQuit && g_events == 1 && eventOk() && g_clears == 1 && !g_clearArg);
    CHECK(g_eventAt < g_clearAt && !game.QuitWithDelay && game.InSession);

    // Pending, "yes": ForceQuit set before Quit, then event, then clear.
    reset(); game.QuitWithDelay = true;
    game.Answer(true);
    CHECK(g_quits == 1 && g_forceAtQuit && game.ForceQuit && g_events == 1 && eventOk() && g_clears == 1 && !g_clearArg);
    CHECK(g_quitAt == 1 && g_eventAt == 2 && g_clearAt == 3 && !game.QuitWithDelay && game.InSession);

    if (g_fail) { std::printf("007e7c00_TEST FAIL\n"); return 1; }
    std::printf("007e7c00_TEST PASS\n");
    return 0;
}