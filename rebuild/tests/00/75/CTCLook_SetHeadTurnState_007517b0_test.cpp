#include <cstdio>
class CTCLook { public:
    long HeadTurnState; long BodyTurnState; long TimeToStopLooking; bool OneFrameDelay;
    void SetHeadTurnState(long state)
    {
        TimeToStopLooking = -1;
        if (HeadTurnState != state) { HeadTurnState = state; OneFrameDelay = true; }
    }
};
int main()
{
    CTCLook l; l.HeadTurnState = 2; l.BodyTurnState = 0; l.TimeToStopLooking = 50; l.OneFrameDelay = false;
    l.SetHeadTurnState(2);
    if (l.TimeToStopLooking != -1 || l.OneFrameDelay || l.HeadTurnState != 2) { std::printf("007517b0_TEST FAIL\n"); return 1; }
    l.SetHeadTurnState(5);
    if (l.HeadTurnState != 5 || !l.OneFrameDelay) { std::printf("007517b0_TEST FAIL\n"); return 1; }
    std::printf("007517b0_TEST PASS\n");
    return 0;
}