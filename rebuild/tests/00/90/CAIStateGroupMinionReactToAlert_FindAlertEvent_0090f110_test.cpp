// behaviour test for 0x0090F110 CAIStateGroup_MinionReactToAlert::FindAlertEvent (alert event from a live allied originator)
#include <cstdio>
class CThing { public: enum { ThingTypeMask = 0 }; unsigned int ThingTypeFlags; unsigned char m_flags91; int faction; };
template <class T> inline bool IsThingOfType(const CThing* const& thing)
{ unsigned long mask = T::ThingTypeMask; return (thing->ThingTypeFlags & mask) == mask; }
struct Event { CThing* POriginator; };
static Event* Find(Event* event, CThing* me)
{
    if (event) {
        CThing* originator = 0;
        if (event->POriginator != 0 && IsThingOfType<CThing>(event->POriginator))
            originator = event->POriginator;
        if (originator != 0 && !(originator->m_flags91 & 1)) {
            if (originator->faction == me->faction)
                return event;
        }
    }
    return 0;
}
int main()
{
    CThing me = { 4, 0, 1 }, ally = { 8, 0, 1 }, foe = { 8, 0, 2 }, deadAlly = { 8, 1, 1 };
    Event e1 = { &ally }, e2 = { &foe }, e3 = { &deadAlly }, e4 = { 0 };
    bool ok = Find(&e1, &me) == &e1 && Find(&e2, &me) == 0 && Find(&e3, &me) == 0 && Find(&e4, &me) == 0 && Find(0, &me) == 0;
    std::printf(ok ? "0090f110_TEST PASS\n" : "0090f110_TEST FAIL\n");
    return ok ? 0 : 1;
}