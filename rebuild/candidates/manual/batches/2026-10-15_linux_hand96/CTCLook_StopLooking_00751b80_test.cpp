#include <cstdio>
struct CTCLook {
    long HeadTurnState; long BodyTurnState; long TimeToStopLooking; bool OneFrameDelay;
    void ChangeHeadTurnState(long s) { if (HeadTurnState != s) { HeadTurnState = s; OneFrameDelay = true; } }
    void SetBodyTurnState(long s) { if (BodyTurnState != s) BodyTurnState = s; }
    void StopLooking(long t)
    {
        SetBodyTurnState(1);
        if (t == 0) { ChangeHeadTurnState(1); TimeToStopLooking = -1; } else TimeToStopLooking = t;
    }
};
int main()
{
    CTCLook l; l.HeadTurnState = 3; l.BodyTurnState = 4; l.TimeToStopLooking = 9; l.OneFrameDelay = false;
    l.StopLooking(77);
    if (l.BodyTurnState != 1 || l.TimeToStopLooking != 77 || l.HeadTurnState != 3 || l.OneFrameDelay) { std::printf("00751b80_TEST FAIL\n"); return 1; }
    l.StopLooking(0);
    if (l.HeadTurnState != 1 || !l.OneFrameDelay || l.TimeToStopLooking != -1) { std::printf("00751b80_TEST FAIL\n"); return 1; }
    l.OneFrameDelay = false; l.StopLooking(0);
    if (l.OneFrameDelay) { std::printf("00751b80_TEST FAIL\n"); return 1; }
    std::printf("00751b80_TEST PASS\n");
    return 0;
}
