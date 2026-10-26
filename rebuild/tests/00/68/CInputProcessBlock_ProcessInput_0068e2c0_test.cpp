// Behaviour test for CInputProcessBlock::ProcessInput @ 0x0068E2C0: when the input event is a button press
// (IsEventButtonPressed) and maps to game action 0x56 (block), one default "block" game event of type 0x3d is queued
// on the processed input; otherwise nothing. Checks: not pressed (action test short-circuited), pressed but another
// action, pressed block action (event type and default fields, the event and processed objects passed through).
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

void CInputProcessBlock::ProcessInput(const CInputEvent& event, CProcessedInput& processed)
{
    if (IsEventButtonPressed(event) && IsEventGameAction(0x56, event)) {
        CGameEvent gameEvent(0x3d);
        processed.AddGameEvent(gameEvent);
    }
}

// ---- instrumented stubs ----
class CInputEvent { public: bool pressed; long action; };
void CInputProcessGameBase::v00() { }
void CInputProcessBlock::Update(CProcessedInput&) { g_fail = 1; }
static int g_events = 0, g_pressCalls = 0, g_actionCalls = 0;
static long g_askedAction = 0;
static CGameEvent g_last(0);
static const CInputEvent* g_expectEvent = 0;
static CProcessedInput* g_expectProcessed = 0;
static CInputProcessGameBase* g_expectThis = 0;
void CProcessedInput::AddGameEvent(const CGameEvent& e) { if (this != g_expectProcessed) g_fail = 1; ++g_events; g_last = e; }
void CProcessedInput::SetHandled() { g_fail = 1; }
bool CInputProcessGameBase::IsEventButtonPressed(const CInputEvent& e)
{ ++g_pressCalls; if (&e != g_expectEvent || this != g_expectThis) g_fail = 1; return e.pressed; }
bool CInputProcessGameBase::IsEventGameAction(long action, const CInputEvent& e)
{ ++g_actionCalls; g_askedAction = action; if (&e != g_expectEvent || this != g_expectThis) g_fail = 1; return e.action == action; }

static void reset() { g_events = g_pressCalls = g_actionCalls = 0; g_askedAction = 0; g_last = CGameEvent(0); }

int main()
{
    CInputProcessBlock proc; CProcessedInput processed; CInputEvent ev;
    g_expectThis = &proc; g_expectProcessed = &processed; g_expectEvent = &ev;

    // Not a press: nothing, action never asked.
    reset(); ev.pressed = false; ev.action = 0x56;
    proc.ProcessInput(ev, processed);
    CHECK(g_pressCalls == 1 && g_actionCalls == 0 && g_events == 0);

    // A press of another action: nothing.
    reset(); ev.pressed = true; ev.action = 0x1d;
    proc.ProcessInput(ev, processed);
    CHECK(g_pressCalls == 1 && g_actionCalls == 1 && g_askedAction == 0x56 && g_events == 0);

    // A press of the block action: one 0x3d event.
    reset(); ev.pressed = true; ev.action = 0x56;
    proc.ProcessInput(ev, processed);
    CHECK(g_pressCalls == 1 && g_actionCalls == 1 && g_askedAction == 0x56 && g_events == 1);
    CHECK(g_last.Type == 0x3d && g_last.Player == -1 && g_last.EndPos == 0 && g_last.Valid && !g_last.Replacement);

    if (g_fail) { std::printf("0068e2c0_TEST FAIL\n"); return 1; }
    std::printf("0068e2c0_TEST PASS\n");
    return 0;
}