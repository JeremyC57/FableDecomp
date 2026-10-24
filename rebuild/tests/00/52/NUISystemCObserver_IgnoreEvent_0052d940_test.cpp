#include <cstdio>
#include <set>
#include <algorithm>
enum EEvent { EVENT_3 = 3, EVENT_HOVER = 25, EVENT_30 = 30 };
struct CObserver {
    std::set<EEvent> EventsToObserve;
    void IgnoreEvent(EEvent event)
    {
        std::set<EEvent>::iterator it = std::find(EventsToObserve.begin(), EventsToObserve.end(), event);
        if (it != EventsToObserve.end()) EventsToObserve.erase(it);
    }
};
int main()
{
    CObserver o; o.EventsToObserve.insert(EVENT_3); o.EventsToObserve.insert(EVENT_30);
    o.IgnoreEvent(EVENT_3); o.IgnoreEvent(EVENT_HOVER);
    if (o.EventsToObserve.size() != 1 || o.EventsToObserve.count(EVENT_30) != 1) { std::printf("0052d940_TEST FAIL\n"); return 1; }
    std::printf("0052d940_TEST PASS\n");
    return 0;
}