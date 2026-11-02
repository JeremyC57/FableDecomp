// Behaviour test for CCreatureAction_PlayAnimation::FrameUpdate (0x008426E0): a stay-on-last-frame animation holds its frame
// counter one short of the total time, then the base update runs.
#include <cstdio>
class CCreatureActionBase {
public:
    long TotalTime;
    long FramesRunning;
    int baseCalls;
    CCreatureActionBase() : TotalTime(0), FramesRunning(0), baseCalls(0) { }
    virtual ~CCreatureActionBase() { }
protected:
    virtual void FrameUpdate() { ++baseCalls; ++FramesRunning; }   // the base advances the frame counter
};
class CCreatureAction_PlayAnimation : public CCreatureActionBase {
public:
    bool StayOnLastFrame;
    CCreatureAction_PlayAnimation() : StayOnLastFrame(false) { }
    virtual void FrameUpdate();
};
void CCreatureAction_PlayAnimation::FrameUpdate()
{
    if (StayOnLastFrame) {
        if (FramesRunning + 1 >= TotalTime)
            FramesRunning--;
    }
    CCreatureActionBase::FrameUpdate();
}
int main()
{
    CCreatureAction_PlayAnimation a;
    a.TotalTime = 5;
    a.StayOnLastFrame = true;
    for (int i = 0; i < 20; ++i) a.FrameUpdate();
    if (a.FramesRunning != 4 || a.baseCalls != 20) { std::printf("008426e0_TEST FAIL\n"); return 1; }
    CCreatureAction_PlayAnimation b;
    b.TotalTime = 5;
    for (int i = 0; i < 20; ++i) b.FrameUpdate();
    if (b.FramesRunning != 20) { std::printf("008426e0_TEST FAIL\n"); return 1; }
    std::printf("008426e0_TEST PASS\n");
    return 0;
}