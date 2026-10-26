// Behaviour test for CInputProcessBlock::Update @ 0x0068E330: every call queues exactly one block game event on the
// processed input -- type 0xd ("keep blocking") when game action 7 still has a held-button event (kind 2) in the
// queue, else type 0x3d ("stop blocking"). Checks both arms, the (7, 2) query arguments, and the event's default
// fields (player -1, empty data, valid, not a replacement).
#include <cstdio>
#include <cstring>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d\n", __LINE__); } } while (0)

class CInputEvent;
class CProcessedInput;
struct CMemoryBuffer {
    void Copy(unsigned long length, const void* data);   // 0x009F1760
};
class CGameEvent : public CMemoryBuffer { public:
    long          Type;                            // +0x00
    signed char   Player;                          // +0x04
    unsigned char Data[32];                        // +0x05
    unsigned char EndPos;                          // +0x25
    bool          Valid;                           // +0x26
    bool          Replacement;                     // +0x27
    CGameEvent(long type) : Type(type), Player(-1), EndPos(0), Valid(true), Replacement(false) { }
    void AddLong(long value) { Copy(sizeof(value), &value); }
};
class CProcessedInput {
public:
    void AddGameEvent(const CGameEvent& event);    // 0x00A0D340
    void SetHandled();                             // 0x00A0D390
};
class CPlayer {
public:
    long GetCurrentMode() const;                   // 0x00633BE0
};
class CWorld; class CGameDefinitionManager; class CMainGameComponent; class CGamePlayerInterface;
class CInputProcessGameBase {                      // engine/CInputProcessGameBase.h layout
public:
    virtual void v00();
    unsigned char           _pad04[0x10];          // +0x04 CAInputProcess
    CWorld*                 World;                 // +0x14
    CPlayer*                MyPlayer;              // +0x18
    CGameDefinitionManager* DefinitionManager;     // +0x1c
    CMainGameComponent*     Component;             // +0x20
    CGamePlayerInterface*   PGamePlayerInterface;  // +0x24
    bool IsEventButtonPressed(const CInputEvent& event);              // 0x00687A90 (kind 0x13; manifest "IsEventButtonHeld")
    bool IsEventButtonHeld(const CInputEvent& event);                 // 0x00687BD0 (kind 0x15)
    bool IsEventGameAction(long action, const CInputEvent& event);    // 0x00687CF0
    bool IsGameActionEventInQueue(long action, long buttonEventType); // 0x00687D50
};
class CInputProcessBlock : public CInputProcessGameBase {
public:
    virtual void ProcessInput(const CInputEvent& event, CProcessedInput& processed);
    virtual void Update(CProcessedInput& processed);
};

void CInputProcessBlock::Update(CProcessedInput& processed)
{
    if (IsGameActionEventInQueue(7, 2)) {
        CGameEvent gameEvent(0xd);
        processed.AddGameEvent(gameEvent);
    } else {
        CGameEvent gameEvent(0x3d);
        processed.AddGameEvent(gameEvent);
    }
}

// ---- instrumented stubs ----
class CInputEvent { public: int unused; };
void CInputProcessGameBase::v00() { }
void CInputProcessBlock::ProcessInput(const CInputEvent&, CProcessedInput&) { g_fail = 1; }
static int g_events = 0, g_queueCalls = 0;
static long g_qAction = 0, g_qKind = 0;
static bool g_inQueue = false;
static CGameEvent g_last(0);
static CProcessedInput* g_expectProcessed = 0;
static CInputProcessGameBase* g_expectThis = 0;
void CProcessedInput::AddGameEvent(const CGameEvent& e) { if (this != g_expectProcessed) g_fail = 1; ++g_events; g_last = e; }
void CProcessedInput::SetHandled() { g_fail = 1; }
bool CInputProcessGameBase::IsGameActionEventInQueue(long action, long kind)
{ ++g_queueCalls; g_qAction = action; g_qKind = kind; if (this != g_expectThis) g_fail = 1; return g_inQueue; }

static void reset() { g_events = g_queueCalls = 0; g_qAction = g_qKind = 0; g_last = CGameEvent(0); }
static bool defaultsOk() { return g_last.Player == -1 && g_last.EndPos == 0 && g_last.Valid && !g_last.Replacement; }

int main()
{
    CInputProcessBlock proc; CProcessedInput processed;
    g_expectThis = &proc; g_expectProcessed = &processed;

    reset(); g_inQueue = true;
    proc.Update(processed);
    CHECK(g_queueCalls == 1 && g_qAction == 7 && g_qKind == 2 && g_events == 1 && g_last.Type == 0xd && defaultsOk());

    reset(); g_inQueue = false;
    proc.Update(processed);
    CHECK(g_queueCalls == 1 && g_qAction == 7 && g_qKind == 2 && g_events == 1 && g_last.Type == 0x3d && defaultsOk());

    reset(); g_inQueue = true;
    proc.Update(processed);
    CHECK(g_events == 1 && g_last.Type == 0xd && defaultsOk());

    if (g_fail) { std::printf("0068e330_TEST FAIL\n"); return 1; }
    std::printf("0068e330_TEST PASS\n");
    return 0;
}