// behaviour test for 0x007CBDA0 CTCGold::CTCGold(CThing&) (model copy of the source; CTCBase's ctor is a stub)
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
class CTCGold : public CTCBase {
public:
    CTCGold(CThing& thing);
    long Gold;                                           // +0x0C
};
CTCGold::CTCGold(CThing& thing)
    : CTCBase(thing), Gold(0)
{
}
int main()
{
    CThing t; t.id = 1;
    static double raw[(sizeof(CTCGold) + 7) / 8];
    std::memset(raw, 0xCD, sizeof(raw));
    CTCGold* p = new (raw) CTCGold(t);
    int ok = g_baseThing == &t && p->ParentThing == &t && p->Gold == 0;
    if (!ok) { std::printf("007cbda0_TEST FAIL\n"); return 1; }
    std::printf("007cbda0_TEST PASS\n");
    return 0;
}