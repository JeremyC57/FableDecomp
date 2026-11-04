// Behaviour test for NUISystem::CScrollBar::ProcessEvent (0x00559A90): a standalone model of the thumb drag -- the first mouse
// movement after a left click starts the drag (position saved, accumulated delta zeroed); the vertical movement accumulates (a
// direction change restarts it); dragging up past whole DeltaForActionUp steps fires one dragged-up action per step (capped at
// MaxAmountActionsUp, the overshoot trimmed off the movement), then moves the step budget from up to down; dragging down mirrors
// it. Every mouse movement then goes to the CClickable handler.
#include <cstdio>
#include <cmath>
struct Bar {
    bool clicked, dragged; float accum, posY, topY, stepUp, stepDown; unsigned long maxUp, maxDown, amountUp, amountDown;
    int upActions, downActions, clickable; float moved;
};
static void ProcessEvent(Bar& b, bool mouseMove, float dy)
{
    if (mouseMove) {
        if (b.clicked && !b.dragged) { b.dragged = true; b.accum = 0.0f; }
        if (b.dragged) {
            float m = dy;
            b.accum += m;
            if (b.accum == 0.0f || m / b.accum < 0.0f) b.accum = m;
            if (m < 0.0f && b.maxUp > 0) {
                float start = b.stepUp - (b.maxUp * b.stepUp - (b.posY - b.topY));
                float end = (b.maxUp - 1) * b.stepUp + start;
                float dragged = std::fabs(b.accum);
                if (end < dragged) { m = -(end - (m - b.accum)); b.amountUp = b.maxUp; }
                else if (dragged >= start) b.amountUp = (unsigned long)std::fabs((dragged - start) / b.stepUp) + 1;
                else b.amountUp = 0;
                b.posY += m; b.moved = m;
                b.upActions += (int)b.amountUp;
                if (b.amountUp > 0) {
                    b.accum = b.maxUp == b.amountUp ? 0.0f : b.accum + b.amountUp * b.stepUp + start;
                    if (b.maxDown == 0) b.stepDown = b.stepUp;
                    b.maxDown += b.amountUp; b.maxUp -= b.amountUp; b.amountUp = 0;
                }
            } else if (m > 0.0f && b.maxDown > 0) {
                float start = b.stepDown - ((b.posY - b.topY) - b.maxUp * b.stepUp);
                float end = (b.maxDown - 1) * b.stepDown + start;
                if (end < std::fabs(b.accum)) { m = end - (b.accum - m); b.amountDown = b.maxDown; }
                else if (b.accum >= start) b.amountDown = (unsigned long)(std::fabs(b.accum - start) / b.stepDown) + 1;
                else b.amountDown = 0;
                b.posY += m; b.moved = m;
                b.downActions += (int)b.amountDown;
                if (b.amountDown != 0) {
                    b.accum = b.maxDown == b.amountDown ? 0.0f : b.accum - start - b.amountDown * b.stepDown;
                    if (b.maxUp == 0) b.stepUp = b.stepDown;
                    b.maxUp += b.amountDown; b.maxDown -= b.amountDown; b.amountDown = 0;
                }
            }
        }
    }
    ++b.clickable;
}
int main()
{
    int fail = 0;
    Bar b = { true, false, 0, 100.0f, 0.0f, 10.0f, 0.0f, 3, 0, 0, 0, 0, 0, 0, 0 };
    ProcessEvent(b, true, -4.0f);                    // start the drag, 4 up: not a whole step yet... start = 10-(30-100) = 80
    if (!b.dragged || b.upActions != 0 || b.posY != 96.0f || b.clickable != 1) fail = 1;
    Bar c = { true, true, 0, 30.0f, 0.0f, 10.0f, 0.0f, 3, 0, 0, 0, 0, 0, 0, 0 };
    ProcessEvent(c, true, -25.0f);                   // start = 10-(30-30) = 10, end = 30: two whole steps
    if (c.upActions != 2 || c.maxUp != 1 || c.maxDown != 2 || c.stepDown != 10.0f) fail = 2;
    Bar d = { true, true, 0, 30.0f, 0.0f, 10.0f, 0.0f, 3, 0, 0, 0, 0, 0, 0, 0 };
    ProcessEvent(d, true, -50.0f);                   // past the end: capped at three steps, overshoot trimmed
    if (d.upActions != 3 || d.maxUp != 0 || d.maxDown != 3 || d.moved != -30.0f || d.accum != 0.0f) fail = 3;
    ProcessEvent(d, true, 25.0f);                    // back down two steps
    if (d.downActions != 2 || d.maxDown != 1 || d.maxUp != 2) fail = 4;
    ProcessEvent(d, false, 5.0f);
    if (d.clickable != 3) fail = 5;
    if (fail) { std::printf("00559a90_TEST FAIL %d\n", fail); return 1; }
    std::printf("00559a90_TEST PASS\n");
    return 0;
}
