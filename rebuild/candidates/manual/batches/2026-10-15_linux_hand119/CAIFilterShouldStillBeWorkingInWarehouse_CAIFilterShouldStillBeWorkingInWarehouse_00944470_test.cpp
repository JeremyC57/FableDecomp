// Behaviour test for 0x00944470: the filter stores the village and copies both intelligent pointers;
// the by-value arguments are destroyed (released) at the end.
#include <cstdio>
struct CBaseObject { int refs; };
static int g_live = 0;
struct CBaseIntelligentPointer {
    CBaseObject* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) {}
    ~CBaseIntelligentPointer() { SetPItem(0); }
    CBaseObject* GetPItem() const { return m_pItem; }
    void SetPItem(CBaseObject* item) { if (m_pItem) { m_pItem->refs--; g_live--; } m_pItem = item; if (item) { item->refs++; g_live++; } }
};
struct CIP : CBaseIntelligentPointer {
    CIP() {}
    CIP(const CIP& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
};
struct CTCVillage { int id; };
struct CFilter {
    CTCVillage* m_pVillage; CIP m_pCrate; CIP m_pWarehouse;
    CFilter(CIP crate, CIP warehouse, CTCVillage* village) : m_pVillage(village), m_pCrate(crate), m_pWarehouse(warehouse) {}
};
int main()
{
    CBaseObject crate = { 0 }, house = { 0 };
    CTCVillage village = { 7 };
    CIP a; a.SetPItem(&crate);
    CIP b; b.SetPItem(&house);
    {
        CFilter f(a, b, &village);
        if (f.m_pVillage != &village || f.m_pCrate.GetPItem() != &crate || f.m_pWarehouse.GetPItem() != &house
            || crate.refs != 2 || house.refs != 2) { std::printf("00944470_TEST FAIL\n"); return 1; }
    }
    if (crate.refs != 1 || house.refs != 1) { std::printf("00944470_TEST FAIL\n"); return 1; }
    std::printf("00944470_TEST PASS\n");
    return 0;
}
