#include <cstdio>
#include <set>
enum EEvent { EVENT_35 = 35, EVENT_36 = 36, EVENT_37 = 37 };
static float g_hide = -1.0f;
struct CObserver {
    std::set<int> EventsToObserve;
    virtual ~CObserver() {}
    virtual void IgnoreEvent(EEvent e) { EventsToObserve.erase((int)e); }
};
struct CComponentRoot { virtual ~CComponentRoot() {} int pad; };
struct CScrollableList : CComponentRoot, CObserver {
    void HideList(float t) { g_hide = t; }
    void OnDeactivate() { HideList(0.2f); IgnoreEvent(EVENT_37); IgnoreEvent(EVENT_36); }
};
int main()
{
    CScrollableList l;
    l.EventsToObserve.insert(35); l.EventsToObserve.insert(36); l.EventsToObserve.insert(37);
    l.OnDeactivate();
    if (g_hide != 0.2f || l.EventsToObserve.size() != 1 || l.EventsToObserve.count(35) != 1) { std::printf("005562a0_TEST FAIL\n"); return 1; }
    std::printf("005562a0_TEST PASS\n");
    return 0;
}