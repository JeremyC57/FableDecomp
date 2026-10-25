// behaviour test for 0x008BFE20 NEntityEvents::CEventFiredShot::CEventFiredShot(CThing&): the event starts at world frame 0 and its
// PTarget intelligent pointer is built empty and then pointed at the argument (CBaseIntelligentPointer ctor, then SetPItem)
#include <cstdio>
struct CBaseObject { int tag; };
struct CThing : CBaseObject { int extra; };
static int gOrder = 0, gCtorAt = 0, gSetAt = 0;
struct CBaseIntelligentPointer {
    CBaseObject* PData;
    CBaseIntelligentPointer() : PData(0) { gCtorAt = ++gOrder; }
    void SetPItem(CBaseObject* item) { PData = item; gSetAt = ++gOrder; }
};
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    CIntelligentPointer(T* item) { SetPItem((CBaseObject*)item); }
    T* Get() const { return (T*)PData; }
};
struct CEventBase {
    long WorldFrame;
    CEventBase() : WorldFrame(0) {}
};
struct CEvent : CEventBase {
    CIntelligentPointer<CThing > PTarget;
    CEvent(CThing& thing) : PTarget(&thing) {}
};
int main()
{
    int fails = 0;
    CThing thing; thing.tag = 7; thing.extra = 9;
    CEvent e(thing);
    if (e.WorldFrame != 0) fails |= 1;
    if (e.PTarget.Get() != &thing) fails |= 2;
    if (!(gCtorAt == 1 && gSetAt == 2)) fails |= 4;          // built empty first, then pointed at the thing
    if (e.PTarget.Get()->extra != 9) fails |= 8;
    if (fails) { std::printf("008bfe20_TEST FAIL %d\n", fails); return 1; }
    std::printf("008bfe20_TEST PASS\n");
    return 0;
}