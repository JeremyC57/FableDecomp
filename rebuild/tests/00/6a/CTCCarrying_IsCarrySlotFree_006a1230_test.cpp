// Behaviour test for CTCCarrying::IsCarrySlotFree (0x006A1230): present-and-empty slot test.
#include <cstdio>
class CThing { public: int id; };

struct CBaseIntelligentPointer {
    CThing* m_pItem;
    CThing* GetPItem() const { return m_pItem; }
};
struct SCarrySlotEntry { long first; CBaseIntelligentPointer second; };
struct CCarrySlotMap {
    SCarrySlotEntry* m_pFirst;
    SCarrySlotEntry* m_pLast;
    SCarrySlotEntry* m_pEnd;
    const SCarrySlotEntry* LowerBound(const long& key) const
    {
        const SCarrySlotEntry* it = m_pFirst;
        while (it != m_pLast && it->first < key) ++it;
        return it;
    }
    const SCarrySlotEntry* End() const { return m_pLast; }
    const SCarrySlotEntry* Find(const long& key) const
    {
        const SCarrySlotEntry* it = LowerBound(key);
        if (it == End() || key < it->first)
            it = End();
        return it;
    }
};
struct CTCCarrying {
    CCarrySlotMap CarrySlots;
    bool IsCarrySlotFree(long slot) const
    {
        const SCarrySlotEntry* it = CarrySlots.Find(slot);
        if (it == CarrySlots.End())
            return false;
        return it->second.GetPItem() == 0;
    }
};

int main()
{
    CThing a, b; a.id = 1; b.id = 2;
    SCarrySlotEntry e[3];
    e[0].first = 1; e[0].second.m_pItem = &a;
    e[1].first = 4; e[1].second.m_pItem = 0;
    e[2].first = 7; e[2].second.m_pItem = &b;
    CTCCarrying c; c.CarrySlots.m_pFirst = e; c.CarrySlots.m_pLast = e + 3; c.CarrySlots.m_pEnd = e + 3;
    if (c.IsCarrySlotFree(1) || c.IsCarrySlotFree(7) || !c.IsCarrySlotFree(4)
        || c.IsCarrySlotFree(5) || c.IsCarrySlotFree(0) || c.IsCarrySlotFree(9)) {
        std::printf("006a1230_TEST FAIL\n"); return 1;
    }
    std::printf("006a1230_TEST PASS\n");
    return 0;
}