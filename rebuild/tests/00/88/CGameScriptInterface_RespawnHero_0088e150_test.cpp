// Behaviour test for CGameScriptInterface::RespawnHero @ 0x0088E150: queues one "respawn hero" game event (type 8) on
// the main game component carrying the flag as a single byte, stamped with the main player number from the
// interface's player manager (+0x14) -- the number overwrites the default Player -1 after the byte is appended.
// Checks both flag values with different player numbers, the copy length, and the event's remaining fields.
#include <cstdio>
#include <cstring>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d\n", __LINE__); } } while (0)

struct CMemoryBuffer {
    void Copy(unsigned long length, const void* data);   // 0x009F1760
};

struct CGameEvent : CMemoryBuffer {
    long          Type;                           // +0x00
    signed char   Player;                         // +0x04
    unsigned char Data[32];                       // +0x05
    unsigned char EndPos;                         // +0x25
    bool          Valid;                          // +0x26
    bool          Replacement;                    // +0x27
    CGameEvent(long type) : Type(type), Player(-1), EndPos(0), Valid(true), Replacement(false) { }
    __forceinline void AddBOOL(bool value) { Copy(1, &value); }   // __forceinline + bool: the by-value copy is homed in the caller's dead bool slot
};

struct CMainGameComponent {
    void AddGameEvent(const CGameEvent& event);   // 0x0041649C
};
extern CMainGameComponent* g_pMainGameComponent;  // 0x013B86A0

struct CPlayerManager_Retail {
    long GetMainPlayerNumber();                   // 0x00449700
};

enum { GAME_EVENT_RESPAWN_HERO = 8 };

struct CWorld;
struct CGameScriptInterface {
    virtual void v000();
    CWorld*                World;                 // +0x04 (engine/CGameScriptInterface.h)
    void*                  Component;             // +0x08
    void*                  DisplayEngine;         // +0x0c
    void*                  DefinitionManager;     // +0x10
    CPlayerManager_Retail* PlayerManager;         // +0x14
    void RespawnHero(bool flag) const;
};

void CGameScriptInterface::RespawnHero(bool flag) const
{
    CGameEvent event(GAME_EVENT_RESPAWN_HERO);
    event.AddBOOL(flag);
    event.Player = (signed char)PlayerManager->GetMainPlayerNumber();
    g_pMainGameComponent->AddGameEvent(event);
}

// ---- instrumented stubs ----
static int g_copies = 0, g_events = 0, g_playerCalls = 0;
static bool g_copyOk = true;
static long g_mainPlayer = 0;
static CGameEvent g_last(-5);
static CPlayerManager_Retail* g_expectPm = 0;
void CMemoryBuffer::Copy(unsigned long length, const void* data)
{
    CGameEvent* e = static_cast<CGameEvent*>(this);
    ++g_copies; if (length != 1 || e->Player != -1 || e->Type != 8) g_copyOk = false;
    std::memcpy(e->Data + e->EndPos, data, length); e->EndPos = (unsigned char)(e->EndPos + length);
}
long CPlayerManager_Retail::GetMainPlayerNumber() { ++g_playerCalls; if (this != g_expectPm) g_fail = 1; return g_mainPlayer; }
void CMainGameComponent::AddGameEvent(const CGameEvent& event) { ++g_events; g_last = event; if (this != g_pMainGameComponent) g_fail = 1; }
CMainGameComponent* g_pMainGameComponent;
void CGameScriptInterface::v000() { }

static void reset() { g_copies = g_events = g_playerCalls = 0; g_copyOk = true; g_last = CGameEvent(-5); }

int main()
{
    CMainGameComponent mgc; g_pMainGameComponent = &mgc;
    CPlayerManager_Retail pm; g_expectPm = &pm;
    CGameScriptInterface gsi; gsi.World = 0; gsi.Component = 0; gsi.DisplayEngine = 0; gsi.DefinitionManager = 0; gsi.PlayerManager = &pm;

    reset(); g_mainPlayer = 2;
    gsi.RespawnHero(true);
    CHECK(g_copies == 1 && g_copyOk && g_playerCalls == 1 && g_events == 1);
    CHECK(g_last.Type == 8 && g_last.Player == 2 && g_last.EndPos == 1 && g_last.Data[0] == 1 && g_last.Valid && !g_last.Replacement);

    reset(); g_mainPlayer = 0;
    gsi.RespawnHero(false);
    CHECK(g_copies == 1 && g_copyOk && g_playerCalls == 1 && g_events == 1);
    CHECK(g_last.Type == 8 && g_last.Player == 0 && g_last.EndPos == 1 && g_last.Data[0] == 0 && g_last.Valid && !g_last.Replacement);

    reset(); g_mainPlayer = 5;
    gsi.RespawnHero(true);
    CHECK(g_last.Type == 8 && g_last.Player == 5 && g_last.EndPos == 1 && g_last.Data[0] == 1);

    if (g_fail) { std::printf("0088e150_TEST FAIL\n"); return 1; }
    std::printf("0088e150_TEST PASS\n");
    return 0;
}