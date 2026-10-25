// behaviour test for 0x00C10CD0 CSoundEventManager::AssignPendingItems: give pending entries free slots while slots last, then place entries until one fails
#include <cstdio>

struct CSoundEventManager;
static int g_assigned = 0, g_placed = 0;

struct CPendingEntry {
    void* m_pSlot;
    void Assign(CSoundEventManager*, void* slot) { m_pSlot = slot; ++g_assigned; }   // stand-in for 0x00C21750
};

struct CPlacedEntry {
    long m_result;
    long Place(CSoundEventManager*) { ++g_placed; return m_result; }                  // stand-in for 0x00C219F0
};

struct CPendingList {                           // GetNext hands out the entries in turn, then 0
    CPendingEntry* m_entries[4];
    int m_count, m_next;
    virtual CPendingEntry* GetNext() { return m_next < m_count ? m_entries[m_next++] : 0; }
};

struct CPlacedList {
    CPlacedEntry* m_entries[4];
    int m_count, m_next;
    virtual CPlacedEntry* GetNext() { return m_next < m_count ? m_entries[m_next++] : 0; }
};

static inline bool IsEntry(const CPlacedEntry* entry)
{
    if (entry == 0)
        return false;
    return true;
}

struct CSoundEventSet {
    int m_freeSlots;
    int m_slots[4];
    void* GetFreeSlot() { return m_freeSlots > 0 ? &m_slots[--m_freeSlots] : 0; }   // stand-in for 0x00C21E50
};

struct CSoundEventManager {
    CSoundEventSet m_eventSet;
    CPendingList m_pending;
    CPlacedList m_placed;
    void AssignPendingItems();
};

void CSoundEventManager::AssignPendingItems()
{
    CPendingEntry* pending = m_pending.GetNext();
    while (pending) {
        void* slot = m_eventSet.GetFreeSlot();
        if (!slot)
            break;
        pending->Assign(this, slot);
        pending = m_pending.GetNext();
    }
    CPlacedEntry* placed = m_placed.GetNext();
    while (IsEntry(placed)) {
        if (placed->Place(this) < 0)
            break;
        placed = m_placed.GetNext();
    }
}

int main()
{
    CPendingEntry pe[3] = { { 0 }, { 0 }, { 0 } };
    CPlacedEntry pl[3] = { { 0 }, { -1 }, { 0 } };
    CSoundEventManager manager;
    manager.m_eventSet.m_freeSlots = 2;
    for (int i = 0; i < 3; ++i) { manager.m_pending.m_entries[i] = &pe[i]; manager.m_placed.m_entries[i] = &pl[i]; }
    manager.m_pending.m_count = 3; manager.m_pending.m_next = 0;
    manager.m_placed.m_count = 3; manager.m_placed.m_next = 0;
    manager.AssignPendingItems();
    if (g_assigned != 2 || pe[0].m_pSlot == 0 || pe[1].m_pSlot == 0 || pe[2].m_pSlot != 0) { std::printf("00c10cd0_TEST FAIL\n"); return 1; }
    if (g_placed != 2) { std::printf("00c10cd0_TEST FAIL\n"); return 1; }             // stops at the failing second entry
    std::printf("00c10cd0_TEST PASS\n");
    return 0;
}