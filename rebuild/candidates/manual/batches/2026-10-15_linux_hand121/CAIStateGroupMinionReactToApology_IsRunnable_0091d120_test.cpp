#include <cstdio>
// CAIStateGroup_MinionReactToApology::IsRunnable: entity-events component holds a hit-by event
struct Events { void* hitBy; bool GetEvent(void** out) { *out = hitBy; return hitBy != 0; } };
struct Group {
    Events* events; bool hasEvents;
    bool IsRunnable()
    {
        if (hasEvents) { void* ev = 0; events->GetEvent(&ev); if (ev) return true; }
        return false;
    }
};
int main()
{
    int x; Events e; e.hitBy = &x; Group g; g.events = &e; g.hasEvents = true;
    if (!g.IsRunnable()) { std::printf("0091d120_TEST FAIL\n"); return 1; }
    e.hitBy = 0; if (g.IsRunnable()) { std::printf("0091d120_TEST FAIL\n"); return 1; }
    e.hitBy = &x; g.hasEvents = false; if (g.IsRunnable()) { std::printf("0091d120_TEST FAIL\n"); return 1; }
    std::printf("0091d120_TEST PASS\n"); return 0;
}
