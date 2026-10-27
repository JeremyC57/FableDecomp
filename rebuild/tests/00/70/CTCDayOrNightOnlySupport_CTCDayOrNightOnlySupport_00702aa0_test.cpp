// behaviour test for 0x00702AA0 CTCDayOrNightOnlySupport::CTCDayOrNightOnlySupport(CThing&) (model copy of the source; CTCBase's ctor is a stub)
#include <cstdio>
#include <cstring>
#include <new>
class CThing { public: int id; };
static CThing* g_baseThing = 0;
class CTCBase {
public:
    CTCBase(CThing& thing) : ParentThing(&thing), ToBeRemoved(false), IsInGlobalUpdateTCs(false) { g_baseThing = &thing; }
    virtual ~CTCBase() { }
    CThing* ParentThing;
    bool ToBeRemoved;
    bool IsInGlobalUpdateTCs;
};
class CTCDayOrNightOnlySupport : public CTCBase {
public:
    CTCDayOrNightOnlySupport(CThing& thing);
    long DayNightOrBoth;                                 // +0x0C EDayNightOrBoth
};
CTCDayOrNightOnlySupport::CTCDayOrNightOnlySupport(CThing& thing)
    : CTCBase(thing), DayNightOrBoth(0)
{
}
int main()
{
    CThing t; t.id = 1;
    static double raw[(sizeof(CTCDayOrNightOnlySupport) + 7) / 8];
    std::memset(raw, 0xCD, sizeof(raw));
    CTCDayOrNightOnlySupport* p = new (raw) CTCDayOrNightOnlySupport(t);
    int ok = g_baseThing == &t && p->ParentThing == &t && p->DayNightOrBoth == 0;
    if (!ok) { std::printf("00702aa0_TEST FAIL\n"); return 1; }
    std::printf("00702aa0_TEST PASS\n");
    return 0;
}