// Behaviour test for CInputProcessJumpingAndRolling::ProcessJumping @ 0x006A4040: only while the global jumping flag
// is set, a button press of game action 0x1d queues one default game event of type 0x32 (jump). Checks: flag clear
// (no input queries at all), flag set but not a press (action never asked), a press of another action, a press of
// the jump action (event type and default fields; the event and processed objects passed through).
#include <cstdio>
#include <cstring>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d\n", __LINE__); } } while (0)

class CInputEvent;
struct CMemoryBuffer {
};
class CGameEvent : public CMemoryBuffer { public:
    long          Type;                            // +0x00
    signed char   Player;                          // +0x04
    unsigned char Data[32];                        // +0x05
    unsigned char EndPos;                          // +0x25
    bool          Valid;                           // +0x26
    bool          Replacement;                     // +0x27
    CGameEvent(long type) : Type(type), Player(-1), EndPos(0), Valid(true), Replacement(false) { }
};
class CProcessedInput {
public:
    void AddGameEvent(const CGameEvent& event);    // 0x00A0D340
};
class CPlayer;
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
    bool IsEventButtonPressed(const CInputEvent& event);              // 0x00687A90 (kind 0x13)
    bool IsEventButtonHeld(const CInputEvent& event);                 // 0x00687BD0 (kind 0x15)
    bool IsEventGameAction(long action, const CInputEvent& event);    // 0x00687CF0
};
extern bool g_JumpingEnabled;                      // 0x013B86E5
class CInputProcessJumpingAndRolling : public CInputProcessGameBase {
public:
    void ProcessJumping(const CInputEvent& event, CProcessedInput& processed);
};

void CInputProcessJumpingAndRolling::ProcessJumping(const CInputEvent& event, CProcessedInput& processed)
{
    if (g_JumpingEnabled && IsEventButtonPressed(event) && IsEventGameAction(0x1d, event)) {
        CGameEvent gameEvent(0x32);
        processed.AddGameEvent(gameEvent);
    }
}

// ---- instrumented stubs ----
bool g_JumpingEnabled = false;
class CInputEvent { public: bool pressed; long action; };
void CInputProcessGameBase::v00() { }
static int g_events = 0, g_pressCalls = 0, g_actionCalls = 0;
static long g_askedAction = 0;
static CGameEvent g_last(0);
static const CInputEvent* g_expectEvent = 0;
static CProcessedInput* g_expectProcessed = 0;
static CInputProcessGameBase* g_expectThis = 0;
void CProcessedInput::AddGameEvent(const CGameEvent& e) { if (this != g_expectProcessed) g_fail = 1; ++g_events; g_last = e; }
bool CInputProcessGameBase::IsEventButtonPressed(const CInputEvent& e)
{ ++g_pressCalls; if (&e != g_expectEvent || this != g_expectThis) g_fail = 1; return e.pressed; }
bool CInputProcessGameBase::IsEventGameAction(long action, const CInputEvent& e)
{ ++g_actionCalls; g_askedAction = action; if (&e != g_expectEvent || this != g_expectThis) g_fail = 1; return e.action == action; }

static void reset() { g_events = g_pressCalls = g_actionCalls = 0; g_askedAction = 0; g_last = CGameEvent(0); }

int main()
{
    CInputProcessJumpingAndRolling proc; CProcessedInput processed; CInputEvent ev;
    g_expectThis = &proc; g_expectProcessed = &processed; g_expectEvent = &ev;

    // Jumping disabled: nothing asked, nothing queued.
    reset(); g_JumpingEnabled = false; ev.pressed = true; ev.action = 0x1d;
    proc.ProcessJumping(ev, processed);
    CHECK(g_pressCalls == 0 && g_actionCalls == 0 && g_events == 0);

    // Enabled, not a press.
    reset(); g_JumpingEnabled = true; ev.pressed = false;
    proc.ProcessJumping(ev, processed);
    CHECK(g_pressCalls == 1 && g_actionCalls == 0 && g_events == 0);

    // Enabled, a press of another action.
    reset(); ev.pressed = true; ev.action = 0x56;
    proc.ProcessJumping(ev, processed);
    CHECK(g_pressCalls == 1 && g_actionCalls == 1 && g_askedAction == 0x1d && g_events == 0);

    // Enabled, a press of the jump action.
    reset(); ev.action = 0x1d;
    proc.ProcessJumping(ev, processed);
    CHECK(g_pressCalls == 1 && g_actionCalls == 1 && g_askedAction == 0x1d && g_events == 1);
    CHECK(g_last.Type == 0x32 && g_last.Player == -1 && g_last.EndPos == 0 && g_last.Valid && !g_last.Replacement);

    if (g_fail) { std::printf("006a4040_TEST FAIL\n"); return 1; }
    std::printf("006a4040_TEST PASS\n");
    return 0;
}