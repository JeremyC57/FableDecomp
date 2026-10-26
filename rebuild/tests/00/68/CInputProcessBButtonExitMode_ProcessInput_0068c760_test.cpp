// Behaviour test for 0068c760: the model below is a copy of src/0068c760.cpp (compiled separately), driven by test doubles.
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __forceinline inline
#endif
static int g_pressedCalls = 0, g_heldCalls = 0, g_actionCalls = 0, g_addCalls = 0, g_handledCalls = 0, g_copyCalls = 0;
static bool g_pressed = false, g_held = false; static long g_actions[4] = {-1,-1,-1,-1}; static long g_lastAction = 0;
static long g_mode = 0; static long g_hasMode = -1; static long g_controller = 0;
static long g_addedType = -1; static long g_addedPlayer = 0; static int g_addedValid = -1, g_addedReplacement = -1, g_addedEndPos = -1; static long g_addedLong = -12345;
static bool ActionMatches(long action) { for (int i = 0; i < 4; ++i) if (g_actions[i] == action) return true; return false; }
class CInputEvent {
public:
    long GetControllerIndex() const { return g_controller; }
};
struct CMemoryBuffer {
    void Copy(unsigned long length, const void* data) { ++g_copyCalls; if (length == 4) std::memcpy(&g_addedLong, data, 4); }
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
    void AddGameEvent(const CGameEvent& event) { ++g_addCalls; g_addedType = event.Type; g_addedPlayer = event.Player; g_addedValid = event.Valid; g_addedReplacement = event.Replacement; g_addedEndPos = event.EndPos; }
    __forceinline void AddGameEvent(long type) { CGameEvent gameEvent(type); AddGameEvent(gameEvent); }
    void SetHandled() { ++g_handledCalls; }
};
class CPlayer {
public:
    long GetCurrentMode() const { return g_mode; }
    bool HasMode(long mode) { return mode == g_hasMode; }
};
class CWorld; class CGameDefinitionManager; class CMainGameComponent; class CGamePlayerInterface;
class CInputProcessGameBase {                      // engine/CInputProcessGameBase.h layout
public:
    unsigned char           _pad04[0x10];          // +0x04 CAInputProcess
    CWorld*                 World;                 // +0x14
    CPlayer*                MyPlayer;              // +0x18
    CGameDefinitionManager* DefinitionManager;     // +0x1c
    CMainGameComponent*     Component;             // +0x20
    CGamePlayerInterface*   PGamePlayerInterface;  // +0x24
    bool IsEventButtonPressed(const CInputEvent& event) { ++g_pressedCalls; return g_pressed; }
    bool IsEventButtonHeld(const CInputEvent& event) { ++g_heldCalls; return g_held; }
    bool IsEventGameAction(long action, const CInputEvent& event) { ++g_actionCalls; g_lastAction = action; return ActionMatches(action); }
};
static void Reset() { g_pressedCalls = g_heldCalls = g_actionCalls = g_addCalls = g_handledCalls = g_copyCalls = 0; g_addedType = -1; g_addedLong = -12345; g_addedPlayer = 0; g_addedValid = g_addedReplacement = g_addedEndPos = -1; }
static bool AddedOk(long type) { return g_addCalls == 1 && g_addedType == type && g_addedPlayer == -1 && g_addedValid == 1 && g_addedReplacement == 0 && g_addedEndPos == 0; }

class CInputProcessBButtonExitMode : public CInputProcessGameBase {
public:
    virtual void ProcessInput(const CInputEvent& event, CProcessedInput& processed);
};

void CInputProcessBButtonExitMode::ProcessInput(const CInputEvent& event, CProcessedInput& processed)
{
    if (IsEventButtonHeld(event) && IsEventGameAction(0x30, event)) {
        CGameEvent gameEvent(0x3f);
        gameEvent.AddLong(MyPlayer->GetCurrentMode());
        processed.AddGameEvent(gameEvent);
    }
}

// ---- test doubles and driver ----
int main()
{
    CInputProcessBButtonExitMode p; std::memset(&p, 0, sizeof(p)); CPlayer player; p.MyPlayer = &player;
    CInputEvent ev; CProcessedInput out; bool ok = true;
    Reset(); g_held = true; g_actions[0] = 0x30; g_mode = 0x2f; p.ProcessInput(ev, out); ok = ok && AddedOk(0x3f) && g_copyCalls == 1 && g_addedLong == 0x2f && g_heldCalls == 1 && g_actionCalls == 1;
    Reset(); g_held = true; g_actions[0] = 0x31; p.ProcessInput(ev, out); ok = ok && g_addCalls == 0 && g_copyCalls == 0 && g_actionCalls == 1;
    Reset(); g_held = false; g_actions[0] = 0x30; p.ProcessInput(ev, out); ok = ok && g_addCalls == 0 && g_actionCalls == 0 && g_heldCalls == 1;
    if (!ok) { std::printf("0068c760_TEST FAIL\n"); return 1; }
    std::printf("0068c760_TEST PASS\n");
    return 0;
}