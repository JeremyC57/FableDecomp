#include <cstdio>
// Name fix (NF382): the row is CTCDPhysicalObstruction::SetThingToIgnoreObstruction(CThing*) (was CTCTrapBase::SetThingAsImmune).
class CThing { public: int id; };
static int g_live = 0;
struct CIntelligentPointer {
    CThing* m_pItem;
    CIntelligentPointer(CThing* t) : m_pItem(t) { ++g_live; }
    CIntelligentPointer(const CIntelligentPointer& o) : m_pItem(o.m_pItem) { ++g_live; }
    ~CIntelligentPointer() { --g_live; }
};
struct CIntelligentPointerVector {
    CThing* items[8]; int count;
    void push_back(const CIntelligentPointer& v) { items[count++] = v.m_pItem; }
};
class CTCDPhysicalObstruction { public:
    CIntelligentPointerVector ThingsToIgnoreObstruction;
    void SetThingToIgnoreObstruction(CThing* thing) { ThingsToIgnoreObstruction.push_back(CIntelligentPointer(thing)); }
};
int main()
{
    CTCDPhysicalObstruction trap; trap.ThingsToIgnoreObstruction.count = 0;
    CThing a, b; a.id = 1; b.id = 2;
    trap.SetThingToIgnoreObstruction(&a);
    trap.SetThingToIgnoreObstruction(&b);
    if (trap.ThingsToIgnoreObstruction.count != 2 || trap.ThingsToIgnoreObstruction.items[0] != &a || trap.ThingsToIgnoreObstruction.items[1] != &b || g_live != 0) {
        std::printf("007898a3_TEST FAIL\n"); return 1;
    }
    std::printf("007898a3_TEST PASS\n");
    return 0;
}