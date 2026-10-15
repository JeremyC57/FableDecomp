// Behaviour test for 006a1060 (CTCCarrying::IsCarryingThing): true when some carry slot's intelligent pointer resolves to the thing.
#include <cstdio>

struct CThing { long m_id; };

struct CBaseIntelligentPointer {
    CThing* m_pItem;
    CThing* GetPItem() const { return m_pItem; }
};

struct SCarrySlotEntry {
    long                    first;
    CBaseIntelligentPointer second;
};

struct CCarrySlotMap {
    SCarrySlotEntry* m_pFirst;
    SCarrySlotEntry* m_pLast;
    SCarrySlotEntry* m_pEnd;
    SCarrySlotEntry* begin() { return m_pFirst; }
    SCarrySlotEntry* end() { return m_pLast; }
};

struct CTCCarrying {
    CCarrySlotMap CarrySlots;
    SCarrySlotEntry* FindThing(CThing* thing)
    {
        for (SCarrySlotEntry* it = CarrySlots.begin(); it != CarrySlots.end(); ++it)
            if (it->second.GetPItem() == thing)
                return it;
        return CarrySlots.end();
    }
    bool IsCarryingThing(CThing* thing) { return FindThing(thing) != CarrySlots.end(); }
};

int main()
{
    CThing a = { 1 }, b = { 2 }, c = { 3 };
    SCarrySlotEntry slots[2];
    slots[0].first = 4; slots[0].second.m_pItem = &a;
    slots[1].first = 9; slots[1].second.m_pItem = &b;
    CTCCarrying carrying;
    carrying.CarrySlots.m_pFirst = slots; carrying.CarrySlots.m_pLast = slots + 2; carrying.CarrySlots.m_pEnd = slots + 2;
    bool ok = carrying.IsCarryingThing(&a) && carrying.IsCarryingThing(&b) && !carrying.IsCarryingThing(&c);
    carrying.CarrySlots.m_pLast = slots;
    ok = ok && !carrying.IsCarryingThing(&a);
    if (!ok) { std::printf("006a1060_TEST FAIL\n"); return 1; }
    std::printf("006a1060_TEST PASS\n");
    return 0;
}