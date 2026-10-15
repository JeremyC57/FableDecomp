// Behaviour test for 0x0068FB90 (CInputProcessCombat::ProcessInput): at most one joystick-queue pull per frame, then the combat processing.
#include <cstdio>
static int g_frame = 0;
static int GFGetCurrentFrame() { return g_frame; }
struct Event { int kind; int player; };
struct InputManager {
    bool pending; int askedPlayer; int asks;
    bool IsJoystickEventInQueue(int type, int device, Event* out, int player) { ++asks; askedPlayer = player; if (!pending || type != 0x11 || device != 0) return false; out->kind = 0x11; out->player = player; return true; }
};
static InputManager g_input;
struct Combat {
    int playerNumber; int lastFrame; int queued; int processed;
    Combat() : playerNumber(1), lastFrame(-1), queued(0), processed(0) {}
    void ProcessCombatInput(const Event&) { ++processed; }
    void ProcessInput(const Event& event)
    {
        if (lastFrame != GFGetCurrentFrame()) {
            lastFrame = GFGetCurrentFrame();
            Event joystickEvent = { 0, 0 };
            if (g_input.IsJoystickEventInQueue(0x11, 0, &joystickEvent, playerNumber)) ++queued;
        }
        ProcessCombatInput(event);
    }
};
int main()
{
    Combat c; Event e = { 1, 0 }; g_input.pending = true; g_input.asks = 0;
    c.ProcessInput(e); c.ProcessInput(e);
    int ok = g_input.asks == 1 && c.queued == 1 && c.processed == 2 && g_input.askedPlayer == 1;
    g_frame = 1; g_input.pending = false; c.ProcessInput(e);
    ok = ok && g_input.asks == 2 && c.queued == 1 && c.processed == 3 && c.lastFrame == 1;
    if (!ok) { std::printf("0068fb90_TEST FAIL\n"); return 1; }
    std::printf("0068fb90_TEST PASS\n");
    return 0;
}