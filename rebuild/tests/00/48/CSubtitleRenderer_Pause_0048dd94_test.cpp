// Behaviour test for CSubtitleRenderer::Pause @ 0x0048DD94: stores the update-while-paused flag, sets PausedGame,
// and queues one game event of type 9 (pause) carrying a single byte 1 for player -1 (Valid set, not a replacement).
// Checks both values of the argument, that neighbouring bytes are untouched, and the event's full contents.
#include <cstdio>
#include <cstring>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d\n", __LINE__); } } while (0)

#pragma optimize("y", off)
#pragma optimize("s", on)
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
enum { GAME_EVENT_PAUSE = 9 };
class CSubtitleRenderer { public:
    unsigned char _pad000[0xdc];
    bool          PausedGame;                     // +0xdc
    unsigned char _pad0dd[9];
    bool          UpdateWhilePaused;              // +0xe6
    void Pause(bool updateWhilePaused);
};

void CSubtitleRenderer::Pause(bool updateWhilePaused)
{
    UpdateWhilePaused = updateWhilePaused;
    PausedGame = true;
    CGameEvent event(GAME_EVENT_PAUSE);
    event.AddUBYTE(1);
    g_pMainGameComponent->AddGameEvent(event);
}
#pragma optimize("", on)

void CMemoryBuffer::Copy(unsigned long length, const void* data)
{
    CGameEvent* e = static_cast<CGameEvent*>(this);
    std::memcpy(e->Data + e->EndPos, data, length); e->EndPos = (unsigned char)(e->EndPos + length);
}
static int g_events = 0;
static CGameEvent g_last(0);
static CMainGameComponent* g_seenThis = 0;
void CMainGameComponent::AddGameEvent(const CGameEvent& event) { ++g_events; g_last = event; g_seenThis = this; }
CMainGameComponent* g_pMainGameComponent;

static CSubtitleRenderer g_r;

static bool eventOk()
{
    return g_last.Type == 9 && g_last.Player == -1 && g_last.EndPos == 1 && g_last.Data[0] == 1
        && g_last.Valid && !g_last.Replacement && g_seenThis == g_pMainGameComponent;
}

int main()
{
    CMainGameComponent mgc; g_pMainGameComponent = &mgc;
    std::memset(&g_r, 0xcc, sizeof(g_r));
    g_r.PausedGame = false; g_r.UpdateWhilePaused = false;

    g_r.Pause(true);
    CHECK(g_r.PausedGame && g_r.UpdateWhilePaused && g_events == 1 && eventOk());
    for (int i = 0xdd; i < 0xe6; ++i) CHECK(((unsigned char*)&g_r)[i] == 0xcc);
    CHECK(((unsigned char*)&g_r)[0xdb] == 0xcc);

    g_r.PausedGame = false;
    g_r.Pause(false);
    CHECK(g_r.PausedGame && !g_r.UpdateWhilePaused && g_events == 2 && eventOk());

    g_r.Pause(true);
    CHECK(g_r.PausedGame && g_r.UpdateWhilePaused && g_events == 3 && eventOk());

    if (g_fail) { std::printf("0048dd94_TEST FAIL\n"); return 1; }
    std::printf("0048dd94_TEST PASS\n");
    return 0;
}