// Behaviour test for NUISystem::CClickable::ProcessEvent (0x0055AD60): click/unclick latching, held actions with
// the input delay, time stamping and the chained CHoverable handler.
#include <cstdio>

enum EEvent { EVENT_LEFT_CLICKED = 26, EVENT_RIGHT_CLICKED = 27, EVENT_LEFT_UNCLICKED = 28, EVENT_RIGHT_UNCLICKED = 29,
              EVENT_30 = 30, EVENT_LEFT_HELD = 31, EVENT_RIGHT_HELD = 32 };
struct CActionList { int id; };
struct CClickable {
    float Time;
    bool WasHovered, WasLeftClicked, WasRightClicked;
    CActionList* PActionsOnLeftHeld; CActionList* PActionsOnRightClicked;
    float InputDelay, LastInputTime;
    int leftClicked, rightClicked, leftUnclicked, rightUnclicked, baseCalls;
    CActionList* lastExecuted; int executed;
    virtual ~CClickable() {}
    virtual void OnLeftClicked() { ++leftClicked; }
    virtual void OnLeftUnclicked() { ++leftUnclicked; }
    virtual void OnRightClicked() { ++rightClicked; }
    virtual void OnRightUnclicked() { ++rightUnclicked; }
    virtual void ExecuteActions(CActionList* a) { lastExecuted = a; ++executed; }
    void HoverableProcessEvent(EEvent) { ++baseCalls; }
    void ProcessEvent(EEvent event);
};

void CClickable::ProcessEvent(EEvent event)
{
    switch (event) {
    case EVENT_LEFT_CLICKED:
        if (WasHovered) { OnLeftClicked(); WasLeftClicked = true; }
        LastInputTime = Time;
        break;
    case EVENT_LEFT_HELD:
        if (Time - LastInputTime >= InputDelay) {
            if (WasLeftClicked)
                ExecuteActions(PActionsOnLeftHeld);
            LastInputTime = Time;
        }
        break;
    case EVENT_LEFT_UNCLICKED:
        if (WasLeftClicked) { OnLeftUnclicked(); WasLeftClicked = false; }
        LastInputTime = Time;
        break;
    case EVENT_RIGHT_CLICKED:
        if (WasHovered) { OnRightClicked(); WasRightClicked = true; }
        LastInputTime = Time;
        break;
    case EVENT_RIGHT_HELD:
        if (WasRightClicked)
            ExecuteActions(PActionsOnRightClicked);
        LastInputTime = Time;
        break;
    case EVENT_RIGHT_UNCLICKED:
        if (WasRightClicked) { OnRightUnclicked(); WasRightClicked = false; }
        LastInputTime = Time;
        break;
    }
    HoverableProcessEvent(event);
}

int main()
{
    CActionList held = { 1 }, right = { 2 };
    CClickable c;
    c.Time = 1.0f; c.WasHovered = true; c.WasLeftClicked = c.WasRightClicked = false;
    c.PActionsOnLeftHeld = &held; c.PActionsOnRightClicked = &right; c.InputDelay = 0.5f; c.LastInputTime = 0.0f;
    c.leftClicked = c.rightClicked = c.leftUnclicked = c.rightUnclicked = c.baseCalls = c.executed = 0; c.lastExecuted = 0;
    c.ProcessEvent(EVENT_LEFT_CLICKED);
    bool ok = c.leftClicked == 1 && c.WasLeftClicked && c.LastInputTime == 1.0f && c.baseCalls == 1;
    c.Time = 1.25f; c.ProcessEvent(EVENT_LEFT_HELD);              // within the delay: nothing, no stamp
    ok = ok && c.executed == 0 && c.LastInputTime == 1.0f && c.baseCalls == 2;
    c.Time = 1.75f; c.ProcessEvent(EVENT_LEFT_HELD);              // past the delay
    ok = ok && c.executed == 1 && c.lastExecuted == &held && c.LastInputTime == 1.75f;
    c.ProcessEvent(EVENT_LEFT_UNCLICKED);
    ok = ok && c.leftUnclicked == 1 && !c.WasLeftClicked;
    c.WasHovered = false; c.Time = 2.0f; c.ProcessEvent(EVENT_RIGHT_CLICKED);
    ok = ok && c.rightClicked == 0 && !c.WasRightClicked && c.LastInputTime == 2.0f;
    c.WasHovered = true; c.ProcessEvent(EVENT_RIGHT_CLICKED); c.ProcessEvent(EVENT_RIGHT_HELD);
    ok = ok && c.rightClicked == 1 && c.executed == 2 && c.lastExecuted == &right;
    c.ProcessEvent(EVENT_RIGHT_UNCLICKED); c.Time = 9.0f; c.ProcessEvent(EVENT_30);
    ok = ok && c.rightUnclicked == 1 && !c.WasRightClicked && c.LastInputTime == 2.0f && c.baseCalls == 9;
    if (!ok) { std::printf("0055ad60_TEST FAIL\n"); return 1; }
    std::printf("0055ad60_TEST PASS\n");
    return 0;
}