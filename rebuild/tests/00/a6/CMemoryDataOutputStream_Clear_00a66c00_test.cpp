// Behaviour test for 0x00A66C00 CMouse::ClearAllMaintainedEvents: the maintained-event list is emptied and the stored-event count
// zeroed; nothing else in the device is touched.
#include <cstdio>
#include <list>
struct CMaintainedEvent { long Event; long Time; };
class CMouse { public:
    long                        Guard;
    long                        NoStoredEvents;
    std::list<CMaintainedEvent> MaintainedEventsList;
    void ClearAllMaintainedEvents();
};
void CMouse::ClearAllMaintainedEvents()
{
    MaintainedEventsList.clear();
    NoStoredEvents = 0;
}
int main()
{
    static CMouse d;
    CMaintainedEvent e = { 7, 9 };
    d.MaintainedEventsList.push_back(e); d.MaintainedEventsList.push_back(e);
    d.NoStoredEvents = 12345; d.Guard = 0x55;
    d.ClearAllMaintainedEvents();
    bool ok = d.MaintainedEventsList.empty() && d.NoStoredEvents == 0 && d.Guard == 0x55;
    d.ClearAllMaintainedEvents();   // clearing an empty list is harmless
    ok = ok && d.MaintainedEventsList.empty() && d.NoStoredEvents == 0;
    if (ok) { std::printf("CLEAR_OK\n"); return 0; }
    std::printf("CLEAR_FAIL\n");
    return 1;
}
