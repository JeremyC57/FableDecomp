// Behaviour test for CCombatActionBase::GetPActiveStrikeEvent (0x00856E50): the functor is seeded with FramesRunning,
// a null event and a 1000000 finish, runs over the whole list, and the kept event is returned.
#include <cstdio>
#include <list>
#include <algorithm>

struct CStrikeEvent {
    long Start, End;
};

class CGetActiveStrikeEventThatIsFirstToFinish {
public:
    CGetActiveStrikeEventThatIsFirstToFinish(long frame) : Frame(frame), PStrikeEvent(0), FirstFinishFrame(1000000) { }
    void operator()(CStrikeEvent* e)
    {
        if (e->Start <= Frame && Frame < e->End && e->End < FirstFinishFrame) {
            PStrikeEvent = e;
            FirstFinishFrame = e->End;
        }
    }
    long          Frame;
    CStrikeEvent* PStrikeEvent;
    long          FirstFinishFrame;
};

class CCombatActionBase { public:
    long FramesRunning;
    std::list<CStrikeEvent*> StrikeEvents;
    CStrikeEvent* GetPActiveStrikeEvent()
    {
        return std::for_each(StrikeEvents.begin(), StrikeEvents.end(),
                             CGetActiveStrikeEventThatIsFirstToFinish(FramesRunning)).PStrikeEvent;
    }
};

int main()
{
    CStrikeEvent a = { 0, 50 }, b = { 5, 20 }, c = { 30, 40 };
    CCombatActionBase action;
    action.FramesRunning = 10;
    if (action.GetPActiveStrikeEvent() != 0) { std::printf("00856e50_TEST FAIL\n"); return 1; }   // empty list
    action.StrikeEvents.push_back(&a);
    action.StrikeEvents.push_back(&b);
    action.StrikeEvents.push_back(&c);
    if (action.GetPActiveStrikeEvent() != &b) { std::printf("00856e50_TEST FAIL\n"); return 1; }  // b finishes first
    action.FramesRunning = 35;
    if (action.GetPActiveStrikeEvent() != &c) { std::printf("00856e50_TEST FAIL\n"); return 1; }
    action.FramesRunning = 60;
    if (action.GetPActiveStrikeEvent() != 0) { std::printf("00856e50_TEST FAIL\n"); return 1; }
    std::printf("00856e50_TEST PASS\n");
    return 0;
}