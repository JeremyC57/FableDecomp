// Behaviour test for CInputProcessInventoryStatsScreen::ProcessInput @ 0x00689C40 (copy of the model with stub callees).
#include <cstdio>
struct C2DVector { float x, y; };
struct CInputEvent {
    long type; int button; long action; C2DVector stick;
    long GetEventType() const { return type; }
    C2DVector GetAnalogueVector() const { return stick; }
};
struct CProcessedInput { bool handled; void SetHandled() { handled = true; } };
struct CFrontEndManager { long last; int sends; void SendEvent(long e) { last = e; ++sends; } };
static CFrontEndManager g_fe;
static CFrontEndManager* GetInstance() { return &g_fe; }
struct CInputProcessInventoryStatsScreen {
    int refreshes;
    void Refresh() { ++refreshes; }
    bool IsEventButtonPressed(const CInputEvent& e) { return e.button == 1; }
    bool IsEventButtonReleased(const CInputEvent& e) { return e.button == 3; }
    bool IsEventGameAction(long a, const CInputEvent& e) { return e.action == a; }
    void ProcessInput(const CInputEvent& event, CProcessedInput& processed)
    {
        Refresh();
        if (event.GetEventType() == 0x11) {
            C2DVector stick = event.GetAnalogueVector();
            if (stick.y > 0.6f) GetInstance()->SendEvent(1);
            else if (stick.y < -0.6f) GetInstance()->SendEvent(0);
            else return;
        } else if (event.GetEventType() == 0x12) {
            C2DVector stick = event.GetAnalogueVector();
            if (stick.y > 0.6f) GetInstance()->SendEvent(7);
            else if (stick.y < -0.6f) GetInstance()->SendEvent(6);
            else return;
        } else if (IsEventButtonPressed(event)) {
            if (IsEventGameAction(0x25, event)) GetInstance()->SendEvent(0);
            else if (IsEventGameAction(0x26, event)) GetInstance()->SendEvent(1);
            else if (IsEventGameAction(0x21, event)) GetInstance()->SendEvent(4);
            else if (IsEventGameAction(0x22, event)) GetInstance()->SendEvent(5);
            else return;
        } else if (IsEventButtonReleased(event)) {
            if (IsEventGameAction(0x25, event)) GetInstance()->SendEvent(0);
            else if (IsEventGameAction(0x26, event)) GetInstance()->SendEvent(1);
            else return;
        } else return;
        processed.SetHandled();
    }
};
static int check(long type, float y, int button, long action, long expect)
{
    CInputProcessInventoryStatsScreen p; p.refreshes = 0; CInputEvent e; CProcessedInput out;
    e.type = type; e.stick.x = 0; e.stick.y = y; e.button = button; e.action = action;
    out.handled = false; g_fe.sends = 0; g_fe.last = -1;
    p.ProcessInput(e, out);
    if (p.refreshes != 1) return 1;
    if (expect < 0) return out.handled || g_fe.sends;
    return !out.handled || g_fe.sends != 1 || g_fe.last != expect;
}
int main()
{
    int bad = 0;
    bad += check(0x11, 0.7f, 0, 0, 1); bad += check(0x11, -0.7f, 0, 0, 0); bad += check(0x11, 0.6f, 0, 0, -1); bad += check(0x11, 0.0f, 0, 0, -1);
    bad += check(0x12, 0.9f, 0, 0, 7); bad += check(0x12, -0.9f, 0, 0, 6); bad += check(0x12, -0.5f, 0, 0, -1);
    bad += check(0, 0, 1, 0x25, 0); bad += check(0, 0, 1, 0x26, 1); bad += check(0, 0, 1, 0x21, 4); bad += check(0, 0, 1, 0x22, 5); bad += check(0, 0, 1, 7, -1);
    bad += check(0, 0, 3, 0x25, 0); bad += check(0, 0, 3, 0x26, 1); bad += check(0, 0, 3, 0x21, -1); bad += check(0, 0, 0, 0x25, -1);
    if (bad) { std::printf("00689c40_TEST FAIL\n"); return 1; }
    std::printf("00689c40_TEST PASS\n");
    return 0;
}