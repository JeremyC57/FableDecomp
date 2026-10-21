// Behaviour test for NUISystem::CObserver::ObserveAllEvents @ 0x0052D7B0: every event except 0x22/0x23 is observed, in order.
#include <cstdio>

enum EEvent { EEVENT_FIRST = 0 };

class CObserver {
public:
    int seen[64];
    int count;
    CObserver() : count(0) {}
    virtual ~CObserver() {}
    virtual void ObserveEvent(EEvent event) { seen[count++] = (int)event; }
    void ObserveAllEvents();
};

void CObserver::ObserveAllEvents()
{
    ObserveEvent((EEvent)0x00); ObserveEvent((EEvent)0x01); ObserveEvent((EEvent)0x02); ObserveEvent((EEvent)0x03);
    ObserveEvent((EEvent)0x04); ObserveEvent((EEvent)0x05); ObserveEvent((EEvent)0x06); ObserveEvent((EEvent)0x07);
    ObserveEvent((EEvent)0x08); ObserveEvent((EEvent)0x09); ObserveEvent((EEvent)0x0a); ObserveEvent((EEvent)0x0b);
    ObserveEvent((EEvent)0x0c); ObserveEvent((EEvent)0x0d); ObserveEvent((EEvent)0x0e); ObserveEvent((EEvent)0x0f);
    ObserveEvent((EEvent)0x10); ObserveEvent((EEvent)0x11); ObserveEvent((EEvent)0x12); ObserveEvent((EEvent)0x13);
    ObserveEvent((EEvent)0x14); ObserveEvent((EEvent)0x15); ObserveEvent((EEvent)0x16); ObserveEvent((EEvent)0x17);
    ObserveEvent((EEvent)0x18); ObserveEvent((EEvent)0x19); ObserveEvent((EEvent)0x1a); ObserveEvent((EEvent)0x1b);
    ObserveEvent((EEvent)0x1c); ObserveEvent((EEvent)0x1d); ObserveEvent((EEvent)0x1e); ObserveEvent((EEvent)0x1f);
    ObserveEvent((EEvent)0x20); ObserveEvent((EEvent)0x21); ObserveEvent((EEvent)0x24); ObserveEvent((EEvent)0x25);
}

int main()
{
    CObserver o;
    o.ObserveAllEvents();
    int ok = o.count == 36;
    for (int i = 0; ok && i < 34; ++i)
        if (o.seen[i] != i) ok = 0;
    if (ok && (o.seen[34] != 0x24 || o.seen[35] != 0x25)) ok = 0;
    if (!ok) { std::printf("0052d7b0_TEST FAIL\n"); return 1; }
    std::printf("0052d7b0_TEST PASS\n");
    return 0;
}
