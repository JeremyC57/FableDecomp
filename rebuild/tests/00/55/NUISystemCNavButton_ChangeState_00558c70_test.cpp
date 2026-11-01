// Test for NUISystem::CNavButton::ChangeState (0x00558C70): base state change first; states 1/5 add the observer and observe
// event 0x19 unless it is the accept button; states 0/6 remove the observer; 2/3/4 and others do nothing more.
#include <cstdio>
struct Manager { int added, removed; void* last; void AddObserver(void* o) { ++added; last = o; } void RemoveObserver(void* o) { ++removed; last = o; } };
static Manager g_manager = { 0, 0, 0 };
struct Observer { int observed; long lastEvent; void ObserveEvent(long e) { ++observed; lastEvent = e; } };
struct Button : Observer {
    int baseCalls; unsigned long baseState; bool accept;
    void BaseChangeState(unsigned long s) { ++baseCalls; baseState = s; }
    bool IsAcceptButton() const { return accept; }
    void ChangeState(unsigned long state)
    {
        BaseChangeState(state);
        switch (state) {
        case 1: case 5:
            g_manager.AddObserver((Observer*)this);
            if (!IsAcceptButton()) ObserveEvent(0x19);
            break;
        case 0: case 6:
            g_manager.RemoveObserver((Observer*)this);
            break;
        }
    }
};
int main()
{
    Button b; b.observed = 0; b.lastEvent = 0; b.baseCalls = 0; b.baseState = 99; b.accept = false;
    b.ChangeState(1);
    if (g_manager.added != 1 || b.observed != 1 || b.lastEvent != 0x19 || b.baseState != 1) { std::printf("00558c70_TEST FAIL\n"); return 1; }
    b.accept = true; b.ChangeState(5);
    if (g_manager.added != 2 || b.observed != 1) { std::printf("00558c70_TEST FAIL\n"); return 1; }
    b.ChangeState(6); b.ChangeState(0);
    if (g_manager.removed != 2 || g_manager.last != (Observer*)&b) { std::printf("00558c70_TEST FAIL\n"); return 1; }
    b.ChangeState(3); b.ChangeState(9);
    if (g_manager.added != 2 || g_manager.removed != 2 || b.baseCalls != 6 || b.baseState != 9) { std::printf("00558c70_TEST FAIL\n"); return 1; }
    std::printf("00558c70_TEST PASS\n");
    return 0;
}