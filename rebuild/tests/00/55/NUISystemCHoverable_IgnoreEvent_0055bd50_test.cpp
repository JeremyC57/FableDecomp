#include <cstdio>
#include <set>
#include <algorithm>
enum EEvent { EVENT_3 = 3, EVENT_HOVER = 25, EVENT_30 = 30 };
static int g_unhovered = 0, g_removed = 0;
struct CHoverable {
    std::set<EEvent> EventsToObserve; bool WasHovered;
    void OnUnhovered() { ++g_unhovered; }
    void SetWasHovered(bool b) { WasHovered = b; }
    void RemoveFromHoveredList() { ++g_removed; }
    void StopHovering() { OnUnhovered(); SetWasHovered(false); RemoveFromHoveredList(); }
    void IgnoreEvent(EEvent event)
    {
        std::set<EEvent>::iterator it = std::find(EventsToObserve.begin(), EventsToObserve.end(), event);
        if (it != EventsToObserve.end()) {
            EventsToObserve.erase(it);
            if (event == EVENT_HOVER && WasHovered) StopHovering();
        }
    }
};
int main()
{
    CHoverable h; h.WasHovered = true; h.EventsToObserve.insert(EVENT_3); h.EventsToObserve.insert(EVENT_HOVER);
    int bad = 0;
    h.IgnoreEvent(EVENT_3); if (g_unhovered != 0 || !h.WasHovered || h.EventsToObserve.size() != 1) bad = 1;
    h.IgnoreEvent(EVENT_HOVER); if (g_unhovered != 1 || g_removed != 1 || h.WasHovered || h.EventsToObserve.size() != 0) bad = 1;
    h.WasHovered = true; h.IgnoreEvent(EVENT_HOVER); if (g_unhovered != 1 || !h.WasHovered) bad = 1;
    h.EventsToObserve.insert(EVENT_HOVER); h.WasHovered = false; h.IgnoreEvent(EVENT_HOVER); if (g_unhovered != 1 || h.EventsToObserve.size() != 0) bad = 1;
    if (bad) { std::printf("0055bd50_TEST FAIL\n"); return 1; }
    std::printf("0055bd50_TEST PASS\n");
    return 0;
}