// Behaviour test for 0x00809270 CTargetPointFilter_IsStealableStockItem::operator(): stock-item component present, owned by a shop, stealable.
#include <cstdio>
class CTCShop { public: int id; };
class CTCStockItem { public:
    bool ForSale; bool Stealable; CTCShop* Shop;
    CTCShop* GetShopTC() const { return Shop; }
};
struct SEntry { long Type; void* Component; };
class CThing { public:
    unsigned int UsingComponents[9];
    SEntry Entries[4]; int Count;
    bool HasTC(long type) const
    {
        if (UsingComponents[type >> 5] & (1u << (type & 31)))
            return true;
        return false;
    }
    void* GetTC(long type) const                              // lower bound over the sorted entries, end when absent
    {
        const SEntry* it = Entries;
        const SEntry* end = Entries + Count;
        while (it != end && it->Type < type) ++it;
        return (it == end || type < it->Type ? end : it)->Component;
    }
};
struct CTargetPoint { CThing* PThing; };
static bool IsStealableStockItem(const CTargetPoint& point)
{
    const CThing* thing = point.PThing;
    if (thing->HasTC(0x54)) {
        const CTCStockItem* item = (const CTCStockItem*)thing->GetTC(0x54);
        if (item->GetShopTC() != 0 && item->Stealable)
            return true;
    }
    return false;
}
static void Init(CThing& t, CTCStockItem* item)
{
    for (int i = 0; i < 9; ++i) t.UsingComponents[i] = 0;
    t.Count = 0;
    t.Entries[t.Count].Type = 0x10; t.Entries[t.Count].Component = 0; ++t.Count;
    if (item) {
        t.UsingComponents[0x54 >> 5] |= 1u << (0x54 & 31);
        t.Entries[t.Count].Type = 0x54; t.Entries[t.Count].Component = item; ++t.Count;
    }
    t.Entries[t.Count].Type = 0x99; t.Entries[t.Count].Component = 0; ++t.Count;
    t.Entries[t.Count].Type = 0; t.Entries[t.Count].Component = 0;   // end slot
}
int main()
{
    CTCShop shop = { 1 };
    CTCStockItem stealable = { true, true, &shop }, locked = { true, false, &shop }, loose = { false, true, 0 };
    CThing a, b, c, d;
    Init(a, &stealable); Init(b, &locked); Init(c, &loose); Init(d, 0);
    CTargetPoint pa = { &a }, pb = { &b }, pc = { &c }, pd = { &d };
    bool ok = (a.UsingComponents[2] & 0x100000u) != 0;         // TC_STOCK_ITEM is word 2 bit 20
    ok = ok && IsStealableStockItem(pa) && !IsStealableStockItem(pb) && !IsStealableStockItem(pc) && !IsStealableStockItem(pd);
    if (!ok) { std::printf("00809270_TEST FAIL\n"); return 1; }
    std::printf("00809270_TEST PASS\n");
    return 0;
}