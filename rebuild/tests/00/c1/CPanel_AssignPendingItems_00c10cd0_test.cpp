// behaviour test for 0x00C10CD0 CPanel::AssignPendingItems: give pending entries free slots while slots last, then place entries until one fails
#include <cstdio>

struct CPanel;
static int g_assigned = 0, g_placed = 0;

struct CPendingEntry {
    void* m_pSlot;
    void Assign(CPanel*, void* slot) { m_pSlot = slot; ++g_assigned; }   // stand-in for 0x00C21750
};

struct CPlacedEntry {
    long m_result;
    long Place(CPanel*) { ++g_placed; return m_result; }                  // stand-in for 0x00C219F0
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

struct CPanelItemSet {
    int m_freeSlots;
    int m_slots[4];
    void* GetFreeSlot() { return m_freeSlots > 0 ? &m_slots[--m_freeSlots] : 0; }   // stand-in for 0x00C21E50
};

struct CPanel {
    CPanelItemSet m_itemSet;
    CPendingList m_pending;
    CPlacedList m_placed;
    void AssignPendingItems();
};

void CPanel::AssignPendingItems()
{
    CPendingEntry* pending = m_pending.GetNext();
    while (pending) {
        void* slot = m_itemSet.GetFreeSlot();
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
    CPanel panel;
    panel.m_itemSet.m_freeSlots = 2;
    for (int i = 0; i < 3; ++i) { panel.m_pending.m_entries[i] = &pe[i]; panel.m_placed.m_entries[i] = &pl[i]; }
    panel.m_pending.m_count = 3; panel.m_pending.m_next = 0;
    panel.m_placed.m_count = 3; panel.m_placed.m_next = 0;
    panel.AssignPendingItems();
    if (g_assigned != 2 || pe[0].m_pSlot == 0 || pe[1].m_pSlot == 0 || pe[2].m_pSlot != 0) { std::printf("00c10cd0_TEST FAIL\n"); return 1; }
    if (g_placed != 2) { std::printf("00c10cd0_TEST FAIL\n"); return 1; }             // stops at the failing second entry
    std::printf("00c10cd0_TEST PASS\n");
    return 0;
}