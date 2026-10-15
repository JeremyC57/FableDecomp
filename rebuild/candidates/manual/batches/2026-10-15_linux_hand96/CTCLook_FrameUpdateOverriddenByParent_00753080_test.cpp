#include <cstdio>
struct CTCLook {
    long HeadTurnState; long BodyTurnState; long TimeToStopLooking; bool OneFrameDelay;
    int centred; int eyes; float ex, ey;
    void ChangeHeadTurnState(long s) { if (HeadTurnState != s) { HeadTurnState = s; OneFrameDelay = true; } }
    void SetBodyTurnState(long s) { if (BodyTurnState != s) BodyTurnState = s; }
    void CentreHead() { ++centred; }
    void ChangeEyeAngles(float a, float b) { ++eyes; ex = a; ey = b; }
    void FrameUpdateOverriddenByParent()
    {
        long state = HeadTurnState;
        if (state & 1) { if (!(state & 8)) CentreHead(); }
        else { ChangeHeadTurnState(1); TimeToStopLooking = -1; CentreHead(); ChangeEyeAngles(0.0f, 0.0f); }
        SetBodyTurnState(1);
    }
};
static void Init(CTCLook& l, long h) { l.HeadTurnState = h; l.BodyTurnState = 0; l.TimeToStopLooking = 5; l.OneFrameDelay = false; l.centred = 0; l.eyes = 0; l.ex = l.ey = 1.0f; }
int main()
{
    CTCLook l;
    Init(l, 3); l.FrameUpdateOverriddenByParent();
    if (l.centred != 1 || l.eyes != 0 || l.BodyTurnState != 1 || l.TimeToStopLooking != 5) { std::printf("00753080_TEST FAIL\n"); return 1; }
    Init(l, 9); l.FrameUpdateOverriddenByParent();
    if (l.centred != 0 || l.eyes != 0 || l.HeadTurnState != 9) { std::printf("00753080_TEST FAIL\n"); return 1; }
    Init(l, 2); l.FrameUpdateOverriddenByParent();
    if (l.HeadTurnState != 1 || !l.OneFrameDelay || l.TimeToStopLooking != -1 || l.centred != 1 || l.eyes != 1 || l.ex != 0.0f || l.ey != 0.0f) { std::printf("00753080_TEST FAIL\n"); return 1; }
    std::printf("00753080_TEST PASS\n");
    return 0;
}
