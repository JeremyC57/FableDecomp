#include <cstdio>
#include <set>
#include <algorithm>
enum EEvent { EVENT_3 = 3, EVENT_HOVER = 25, EVENT_30 = 30 };
struct CObserver {
    std::set<EEvent> EventsToObserve; bool PreventObservation;
    bool IsObservingEvent(EEvent event)
    {
        if (PreventObservation) return false;
        return std::find(EventsToObserve.begin(), EventsToObserve.end(), event) != EventsToObserve.end();
    }
};
int main()
{
    CObserver o; o.PreventObservation = false; o.EventsToObserve.insert(EVENT_3); o.EventsToObserve.insert(EVENT_30);
    int bad = 0;
    if (!o.IsObservingEvent(EVENT_3) || !o.IsObservingEvent(EVENT_30) || o.IsObservingEvent(EVENT_HOVER)) bad = 1;
    o.PreventObservation = true; if (o.IsObservingEvent(EVENT_3)) bad = 1;
    if (bad) { std::printf("0052d900_TEST FAIL\n"); return 1; }
    std::printf("0052d900_TEST PASS\n");
    return 0;
}