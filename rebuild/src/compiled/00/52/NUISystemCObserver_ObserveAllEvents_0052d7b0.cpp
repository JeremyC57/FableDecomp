// NUISystem::CObserver::ObserveAllEvents() -- subscribe the observer to every UI event (0..0x21, 0x24, 0x25) through its own virtual ObserveEvent
// Leaf: ObserveAllEvents
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0052D7B0, length 327 bytes (padded to 336); vtable NUISystem::CObserver 0x01246020 slot 6.
// Callees: virtual ObserveEvent(EEvent) (slot 3, 0x0052DA20 in the base) once per event.
// Layout: engine/CObserver.h (0x18): vptr +0, EventsToObserve (set<EEvent>) +4, PreventObservation +0x14. The header models
// the vptr as a plain pointer, so the class is restated here with its real virtual slots (0 scalar deleting dtor,
// 1 pure, 2/4/5 unnamed, 3 ObserveEvent, 6 ObserveAllEvents); the data members keep the header's names and offsets.
// Shape: 36 virtual calls through `this` with constant arguments; events 0x22 and 0x23 are skipped (they are not
// observable). VC7.1 alternates eax/edx for the vtable load because each call is a fresh `[esi]` read.
namespace NUISystem {

enum EEvent { EEVENT_FIRST = 0 };

class CObserver {
public:
    virtual ~CObserver();                    // slot 0
    virtual void Notify(EEvent event) = 0;   // slot 1
    virtual void Slot2();                    // slot 2 (0x0052D900)
    virtual void ObserveEvent(EEvent event); // slot 3 (0x0052DA20)
    virtual void Slot4();                    // slot 4 (0x0052D940)
    virtual void Slot5();                    // slot 5 (0x0052D9A0)
    virtual void ObserveAllEvents();         // slot 6

    unsigned char EventsToObserve[0x10];     // +0x04 set<NUISystem::EEvent>
    bool          PreventObservation;        // +0x14
};

void CObserver::ObserveAllEvents()
{
    ObserveEvent((EEvent)0x00);
    ObserveEvent((EEvent)0x01);
    ObserveEvent((EEvent)0x02);
    ObserveEvent((EEvent)0x03);
    ObserveEvent((EEvent)0x04);
    ObserveEvent((EEvent)0x05);
    ObserveEvent((EEvent)0x06);
    ObserveEvent((EEvent)0x07);
    ObserveEvent((EEvent)0x08);
    ObserveEvent((EEvent)0x09);
    ObserveEvent((EEvent)0x0a);
    ObserveEvent((EEvent)0x0b);
    ObserveEvent((EEvent)0x0c);
    ObserveEvent((EEvent)0x0d);
    ObserveEvent((EEvent)0x0e);
    ObserveEvent((EEvent)0x0f);
    ObserveEvent((EEvent)0x10);
    ObserveEvent((EEvent)0x11);
    ObserveEvent((EEvent)0x12);
    ObserveEvent((EEvent)0x13);
    ObserveEvent((EEvent)0x14);
    ObserveEvent((EEvent)0x15);
    ObserveEvent((EEvent)0x16);
    ObserveEvent((EEvent)0x17);
    ObserveEvent((EEvent)0x18);
    ObserveEvent((EEvent)0x19);
    ObserveEvent((EEvent)0x1a);
    ObserveEvent((EEvent)0x1b);
    ObserveEvent((EEvent)0x1c);
    ObserveEvent((EEvent)0x1d);
    ObserveEvent((EEvent)0x1e);
    ObserveEvent((EEvent)0x1f);
    ObserveEvent((EEvent)0x20);
    ObserveEvent((EEvent)0x21);
    ObserveEvent((EEvent)0x24);
    ObserveEvent((EEvent)0x25);
}

} // namespace NUISystem