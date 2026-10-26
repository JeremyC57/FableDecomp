// Behaviour test for CInputProcessBoastUI::ProcessInput @ 0x00690E70 (copy of the model with stub callees).
#include <cstdio>
class C2DVector { public: float X, Y; };
class CInputEvent { public: int button; long action; C2DVector MouseMovementXY; };   // button: 1 pressed, 2 held, 3 released
class CProcessedInput { public: bool handled; void SetHandled() { handled = true; } };
struct CFrontEndManager {
    long last; int sends; C2DVector MouseMovement;
    void SendEvent(long e) { last = e; ++sends; }
};
static CFrontEndManager g_fe;
static CFrontEndManager* GetInstance() { return &g_fe; }
class CInputProcessBoastUI { public:
    bool IsEventButtonPressed(const CInputEvent& e) { return e.button == 1; }
    bool IsEventButtonHeld(const CInputEvent& e) { return e.button == 2; }
    bool IsEventButtonReleased(const CInputEvent& e) { return e.button == 3; }
    bool IsEventGameAction(long a, const CInputEvent& e) { return e.action == a; }
    void ProcessInput(const CInputEvent& event, CProcessedInput& processed)
    {
        if (IsEventButtonPressed(event)) {
            if (IsEventGameAction(0x19, event)) GetInstance()->SendEvent(0x1a);
            else if (IsEventGameAction(0x4c, event)) GetInstance()->SendEvent(5);
            else if (IsEventGameAction(0x58, event)) GetInstance()->SendEvent(6);
            else if (IsEventGameAction(0x59, event)) GetInstance()->SendEvent(7);
            else return;
        } else if (IsEventButtonHeld(event)) {
            if (IsEventGameAction(0x19, event)) GetInstance()->SendEvent(0x1c);
            else if (IsEventGameAction(0x4c, event)) GetInstance()->SendEvent(0x1d);
            else return;
        } else if (IsEventButtonReleased(event)) {
            if (IsEventGameAction(0x19, event)) GetInstance()->SendEvent(0x1f);
            else if (IsEventGameAction(0x4c, event)) GetInstance()->SendEvent(0x20);
            else if (IsEventGameAction(0x58, event)) GetInstance()->SendEvent(6);
            else if (IsEventGameAction(0x59, event)) GetInstance()->SendEvent(7);
            else return;
        } else if (IsEventGameAction(0x47, event)) {
            C2DVector movement = event.MouseMovementXY;
            GetInstance()->MouseMovement = movement;
            GetInstance()->SendEvent(0x19);
        } else return;
        processed.SetHandled();
    }
};
static int check(int button, long action, long expect)
{
    CInputProcessBoastUI p; CInputEvent e; CProcessedInput out;
    e.button = button; e.action = action; e.MouseMovementXY.X = 1.5f; e.MouseMovementXY.Y = -2.0f;
    out.handled = false; g_fe.last = -1; g_fe.sends = 0; g_fe.MouseMovement.X = 0; g_fe.MouseMovement.Y = 0;
    p.ProcessInput(e, out);
    if (expect < 0) return (out.handled || g_fe.sends != 0) ? 1 : 0;
    if (!out.handled || g_fe.sends != 1 || g_fe.last != expect) return 1;
    if (action == 0x47 && button == 0 && (g_fe.MouseMovement.X != 1.5f || g_fe.MouseMovement.Y != -2.0f)) return 1;
    return 0;
}
int main()
{
    int bad = 0;
    bad += check(1, 0x19, 0x1a); bad += check(1, 0x4c, 5); bad += check(1, 0x58, 6); bad += check(1, 0x59, 7); bad += check(1, 0x47, -1);
    bad += check(2, 0x19, 0x1c); bad += check(2, 0x4c, 0x1d); bad += check(2, 0x58, -1);
    bad += check(3, 0x19, 0x1f); bad += check(3, 0x4c, 0x20); bad += check(3, 0x58, 6); bad += check(3, 0x59, 7); bad += check(3, 1, -1);
    bad += check(0, 0x47, 0x19); bad += check(0, 0x19, -1);
    if (bad) { std::printf("00690e70_TEST FAIL\n"); return 1; }
    std::printf("00690e70_TEST PASS\n");
    return 0;
}