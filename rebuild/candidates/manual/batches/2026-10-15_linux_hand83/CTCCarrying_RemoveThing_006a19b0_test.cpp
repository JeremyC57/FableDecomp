// Behaviour test for 006a19b0 (CTCCarrying::RemoveThing): the slot holding the thing is emptied with the dropped flag false; an uncarried thing changes nothing.
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

static int  g_calls = 0;
static long g_slot = -1;
static bool g_dropped = true;

struct CTCCarrying {
    CCarrySlotMap CarrySlots;
    SCarrySlotEntry* FindThing(CThing* thing)
    {
        for (SCarrySlotEntry* it = CarrySlots.begin(); it != CarrySlots.end(); ++it)
            if (it->second.GetPItem() == thing)
                return it;
        return CarrySlots.end();
    }
    void RemoveThingInCarrySlot(long slot, bool dropped) { ++g_calls; g_slot = slot; g_dropped = dropped; }
    void RemoveThing(CThing* thing)
    {
        SCarrySlotEntry* it = FindThing(thing);
        if (it != CarrySlots.end())
            RemoveThingInCarrySlot(it->first, false);
    }
};

int main()
{
    CThing a = { 1 }, b = { 2 }, c = { 3 };
    SCarrySlotEntry slots[2];
    slots[0].first = 4; slots[0].second.m_pItem = &a;
    slots[1].first = 9; slots[1].second.m_pItem = &b;
    CTCCarrying carrying;
    carrying.CarrySlots.m_pFirst = slots; carrying.CarrySlots.m_pLast = slots + 2; carrying.CarrySlots.m_pEnd = slots + 2;
    carrying.RemoveThing(&c);
    if (g_calls != 0) { std::printf("006a19b0_TEST FAIL\n"); return 1; }
    carrying.RemoveThing(&b);
    if (g_calls != 1 || g_slot != 9 || g_dropped) { std::printf("006a19b0_TEST FAIL\n"); return 1; }
    std::printf("006a19b0_TEST PASS\n");
    return 0;
}
