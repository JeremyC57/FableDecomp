#include <cstdio>

static int g_handled, g_events, g_lastEvent, g_messages, g_badMsg;
struct CInputEvent {
    int button, action, type, device;
    long GetType() const { return type; }
    long GetDeviceType() const { return device; }
};
struct CGameEvent {
    long Type; signed char Player; unsigned char Data[32]; unsigned char EndPos; bool Valid; bool Replacement;
    CGameEvent(long type) : Type(type), Player(-1), EndPos(0), Valid(true), Replacement(false) { }
};
struct CProcessedInput {
    void SetHandled() { ++g_handled; }
    void AddGameEvent(const CGameEvent& e) { ++g_events; g_lastEvent = (g_handled && e.Player == -1 && e.Valid && !e.EndPos) ? e.Type : -1; }
};
struct C3DVector { float X, Y, Z; C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {} };
struct CThing;
struct CMessageEventManager {
    void* AddMessage(long type, unsigned long flags, const C3DVector* p, CThing* t1, CThing* t2, float a, float b, long prio)
    {
        ++g_messages;
        if (type != 0x4b || flags != 0x32 || p->X != 0 || p->Y != 0 || p->Z != 0 || t1 || t2 || a != 0 || b != 0 || prio != 0) ++g_badMsg;
        return 0;
    }
};
struct CWorld { CMessageEventManager* PMessageEventManager; };
struct CPlayerGui { bool CutsceneActionMode; };
static CPlayerGui g_gui; static CPlayerGui* g_pPlayerGui = &g_gui;
static const C3DVector* Addr(const C3DVector& v) { return &v; }
struct CInputProcessCutScene {
    CWorld* World;
    bool IsEventButtonPressed(const CInputEvent& e) { return e.button == 0x13; }
    bool IsEventGameAction(long action, const CInputEvent& e) { return e.action == action; }
    void ProcessInput(const CInputEvent& event, CProcessedInput& processed)
    {
        if (!IsEventButtonPressed(event))
            return;
        if (IsEventGameAction(0x15, event)) {
            CGameEvent gameEvent(0x39);
            processed.SetHandled();
            processed.AddGameEvent(gameEvent);
            return;
        }
        if (g_pPlayerGui->CutsceneActionMode) {
            if (event.GetDeviceType() == 3 && event.GetType() == 4)
                World->PMessageEventManager->AddMessage(0x4b, 0x32, Addr(C3DVector(0.0f, 0.0f, 0.0f)), 0, 0, 0.0f, 0.0f, 0);
        } else if (IsEventGameAction(0x16, event))
            World->PMessageEventManager->AddMessage(0x4b, 0x32, Addr(C3DVector(0.0f, 0.0f, 0.0f)), 0, 0, 0.0f, 0.0f, 0);
    }
};
static int Fail(int n) { std::printf("006909a0_TEST FAIL %d\n", n); return 1; }
int main()
{
    CMessageEventManager mgr; CWorld world = { &mgr }; CInputProcessCutScene p; p.World = &world; CProcessedInput out;
    CInputEvent e = { 0x13, 0x15, 4, 3 };
    p.ProcessInput(e, out);                                  // skip action: handled + event 0x39, no message
    if (g_handled != 1 || g_events != 1 || g_lastEvent != 0x39 || g_messages != 0) return Fail(1);
    e.action = 0x16; p.ProcessInput(e, out);                 // normal mode, action 0x16: message
    if (g_messages != 1 || g_events != 1) return Fail(2);
    e.action = 0x17; p.ProcessInput(e, out);                 // normal mode, other action: nothing
    if (g_messages != 1) return Fail(3);
    g_gui.CutsceneActionMode = true;
    p.ProcessInput(e, out);                                  // action mode, joystick type 4: message
    if (g_messages != 2) return Fail(4);
    e.device = 2; p.ProcessInput(e, out);                    // action mode, keyboard: nothing
    e.device = 3; e.type = 5; p.ProcessInput(e, out);        // action mode, other type: nothing
    e.action = 0x16; p.ProcessInput(e, out);                 // action 0x16 is ignored in action mode
    if (g_messages != 2) return Fail(5);
    e.button = 0; e.action = 0x15; p.ProcessInput(e, out);   // not a press: nothing at all
    if (g_handled != 1 || g_events != 1 || g_badMsg != 0) return Fail(6);
    std::printf("006909a0_TEST PASS\n");
    return 0;
}