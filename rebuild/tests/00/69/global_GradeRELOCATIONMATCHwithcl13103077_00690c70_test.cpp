// Behaviour test for CInputProcessDigging::ProcessInput @ 0x00690C70: only when the event is a button press AND game
// action 3 AND the hero thing holds component kind 0xc6 (mask word 6, bit 6) is game event 0x1a queued, carrying the
// long 0x17 then the byte 1 (EndPos 5, Player -1, Valid, not a replacement); the && chain short-circuits.
#include <cstdio>
#include <cstring>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d\n", __LINE__); } } while (0)

class CInputEvent { public: int id; };
struct CMemoryBuffer {
    void Copy(unsigned long length, const void* data);   // 0x009F1760
};
struct CGameEvent : CMemoryBuffer {
    long          Type;                            // +0x00
    signed char   Player;                          // +0x04
    unsigned char Data[32];                        // +0x05
    unsigned char EndPos;                          // +0x25
    bool          Valid;                           // +0x26
    bool          Replacement;                     // +0x27
    CGameEvent(long type) : Type(type), Player(-1), EndPos(0), Valid(true), Replacement(false) { }
    void AddLong(long value) { Copy(sizeof(value), &value); }
    void AddUBYTE(unsigned char value) { Copy(1, &value); }
};
static int g_copies = 0;
void CMemoryBuffer::Copy(unsigned long length, const void* data)
{
    ++g_copies;
    CGameEvent* e = static_cast<CGameEvent*>(this);
    std::memcpy(e->Data + e->EndPos, data, length); e->EndPos = (unsigned char)(e->EndPos + length);
}

static int g_queued = 0;
static CGameEvent g_lastEvent(0);
class CProcessedInput {
public:
    void AddGameEvent(const CGameEvent& event);    // 0x00A0D340
    __forceinline void AddGameEvent(long type, long data, unsigned char flag)
    {
        CGameEvent gameEvent(type);
        gameEvent.AddLong(data);
        gameEvent.AddUBYTE(flag);
        AddGameEvent(gameEvent);
    }
};
void CProcessedInput::AddGameEvent(const CGameEvent& event) { ++g_queued; g_lastEvent = event; }

struct CThing {
    unsigned char _pad00[0x20];
    unsigned long m_tcMask[9];                     // +0x20: which component kinds the thing holds
    bool HasTC(long kind) const
    {
        if (m_tcMask[kind >> 5] & (1UL << (kind & 31)))
            return true;
        return false;
    }
};
static CThing g_hero;
static int g_heroCalls = 0;
class CPlayer {
public:
    CThing* GetHeroThing();                        // 0x00487DC0
};
CThing* CPlayer::GetHeroThing() { ++g_heroCalls; return &g_hero; }

struct CWorld; struct CGameDefinitionManager; struct CMainGameComponent; struct CGamePlayerInterface;
static bool g_pressed = false, g_action3 = false;
static int g_pressCalls = 0, g_actionCalls = 0;
static long g_lastAction = 0;
static const CInputEvent* g_lastEventArg = 0;
class CInputProcessGameBase {                      // engine/CInputProcessGameBase.h layout
public:
    virtual void v00() {}
    unsigned char           _pad04[0x10];          // +0x04 CAInputProcess
    CWorld*                 World;                 // +0x14
    CPlayer*                MyPlayer;              // +0x18
    CGameDefinitionManager* DefinitionManager;     // +0x1c
    CMainGameComponent*     Component;             // +0x20
    CGamePlayerInterface*   PGamePlayerInterface;  // +0x24
    bool IsEventButtonPressed(const CInputEvent& event)              // 0x00687A90 (kind 0x13)
    { ++g_pressCalls; g_lastEventArg = &event; return g_pressed; }
    bool IsEventGameAction(long action, const CInputEvent& event)    // 0x00687CF0
    { ++g_actionCalls; g_lastAction = action; g_lastEventArg = &event; return action == 3 && g_action3; }
};
class CInputProcessDigging : public CInputProcessGameBase {
public:
    virtual void ProcessInput(const CInputEvent& event, CProcessedInput& input);
};

void CInputProcessDigging::ProcessInput(const CInputEvent& event, CProcessedInput& input)
{
    if (IsEventButtonPressed(event) && IsEventGameAction(3, event) && MyPlayer->GetHeroThing()->HasTC(0xc6)) {
        input.AddGameEvent(0x1a, 0x17, 1);
    }
}

static void reset() { g_pressCalls = g_actionCalls = g_heroCalls = g_queued = g_copies = 0; g_lastAction = 0; g_lastEventArg = 0; }

int main()
{
    CHECK(sizeof(CGameEvent) == 0x28);
    CPlayer player; CInputProcessDigging p; p.MyPlayer = &player;
    CInputEvent ev; CProcessedInput in;
    std::memset(&g_hero, 0, sizeof(g_hero));

    reset(); g_pressed = false; g_action3 = true; g_hero.m_tcMask[6] = 1UL << 6;
    p.ProcessInput(ev, in);
    CHECK(g_pressCalls == 1 && g_actionCalls == 0 && g_heroCalls == 0 && g_queued == 0);

    reset(); g_pressed = true; g_action3 = false;
    p.ProcessInput(ev, in);
    CHECK(g_actionCalls == 1 && g_lastAction == 3 && g_lastEventArg == &ev && g_heroCalls == 0 && g_queued == 0);

    reset(); g_action3 = true; g_hero.m_tcMask[6] = ~(1UL << 6); g_hero.m_tcMask[5] = 1UL << 6; g_hero.m_tcMask[7] = 1UL << 6;
    p.ProcessInput(ev, in);
    CHECK(g_heroCalls == 1 && g_queued == 0);      // neighbouring words/bits do not count

    reset(); g_hero.m_tcMask[6] = 1UL << 6;
    p.ProcessInput(ev, in);
    CHECK(g_heroCalls == 1 && g_queued == 1 && g_copies == 2);
    CHECK(g_lastEvent.Type == 0x1a && g_lastEvent.Player == -1 && g_lastEvent.EndPos == 5 && g_lastEvent.Valid && !g_lastEvent.Replacement);
    long v = 0; std::memcpy(&v, g_lastEvent.Data, 4);
    CHECK(v == 0x17 && g_lastEvent.Data[4] == 1);

    if (g_fail) { std::printf("00690c70_TEST FAIL\n"); return 1; }
    std::printf("00690c70_TEST PASS\n");
    return 0;
}