// Behaviour test for CTCCarrying::GetThingInCarrySlot (0x006A11F0): sorted slot map, lower bound + key test.
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
class CTCCarrying { public:
    CCarrySlotMap CarrySlots;
    CThing* GetThingInCarrySlot(long slot) const
    {
        const SCarrySlotEntry* it = CarrySlots.Find(slot);
        if (it != CarrySlots.End())
            return it->second.GetPItem();
        return 0;
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
    if (c.GetThingInCarrySlot(1) != &a || c.GetThingInCarrySlot(7) != &b || c.GetThingInCarrySlot(4) != 0
        || c.GetThingInCarrySlot(5) != 0 || c.GetThingInCarrySlot(0) != 0 || c.GetThingInCarrySlot(9) != 0) {
        std::printf("006a11f0_TEST FAIL\n"); return 1;
    }
    std::printf("006a11f0_TEST PASS\n");
    return 0;
}