#include <cstdio>
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
class CTCTrapBase { public:
    CIntelligentPointerVector ImmuneThings;
    void SetThingAsImmune(CThing& thing) { ImmuneThings.push_back(CIntelligentPointer(&thing)); }
};
int main()
{
    CTCTrapBase trap; trap.ImmuneThings.count = 0;
    CThing a, b; a.id = 1; b.id = 2;
    trap.SetThingAsImmune(a);
    trap.SetThingAsImmune(b);
    if (trap.ImmuneThings.count != 2 || trap.ImmuneThings.items[0] != &a || trap.ImmuneThings.items[1] != &b || g_live != 0) {
        std::printf("0078839a_TEST FAIL\n"); return 1;
    }
    std::printf("0078839a_TEST PASS\n");
    return 0;
}