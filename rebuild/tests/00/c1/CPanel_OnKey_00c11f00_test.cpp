// behaviour test for 0x00C11F00 CPanel::OnKey: when the item set is idle, update the items and, still idle, activate the item bound to the key
#include <cstdio>

struct CPanel;
static int g_updates = 0, g_busyQueries = 0;

struct CPanelItem {
    unsigned long m_key;
    int m_activations;
    void Activate(CPanel*) { ++m_activations; }                  // stand-in for 0x00C21620
};

struct CPanelItemSet {
    long m_busyAt;                                                // query number from which the set reports busy (0 = never)
    CPanelItem* m_items;
    int m_count;
    long IsBusy() { ++g_busyQueries; return m_busyAt != 0 && g_busyQueries >= m_busyAt; }   // stand-in for 0x00C21DF0
    CPanelItem* FindItem(unsigned long key)                      // stand-in for 0x00C21E60
    {
        for (int i = 0; i < m_count; ++i) if (m_items[i].m_key == key) return &m_items[i];
        return 0;
    }
};

struct CPanel {
    CPanelItemSet m_itemSet;
    void UpdateItems() { ++g_updates; }                          // stand-in for 0x00C11CB0
    void OnKey(unsigned long key);
};

void CPanel::OnKey(unsigned long key)
{
    if (!m_itemSet.IsBusy()) {
        UpdateItems();
        if (!m_itemSet.IsBusy()) {
            CPanelItem* item = m_itemSet.FindItem(key);
            if (item)
                item->Activate(this);
            m_itemSet.IsBusy();
        }
    }
}

int main()
{
    CPanelItem items[2] = { { 7, 0 }, { 9, 0 } };
    CPanel panel; panel.m_itemSet.m_items = items; panel.m_itemSet.m_count = 2; panel.m_itemSet.m_busyAt = 0;
    panel.OnKey(9);
    if (items[1].m_activations != 1 || items[0].m_activations != 0 || g_updates != 1 || g_busyQueries != 3) { std::printf("00c11f00_TEST FAIL\n"); return 1; }
    panel.OnKey(5);                                               // unbound key: update only
    if (g_updates != 2 || items[0].m_activations + items[1].m_activations != 1) { std::printf("00c11f00_TEST FAIL\n"); return 1; }
    g_busyQueries = 0; panel.m_itemSet.m_busyAt = 2;              // busy after the update
    panel.OnKey(7);
    if (g_updates != 3 || items[0].m_activations != 0 || g_busyQueries != 2) { std::printf("00c11f00_TEST FAIL\n"); return 1; }
    g_busyQueries = 0; panel.m_itemSet.m_busyAt = 1;              // busy from the start
    panel.OnKey(7);
    if (g_updates != 3 || g_busyQueries != 1) { std::printf("00c11f00_TEST FAIL\n"); return 1; }
    std::printf("00c11f00_TEST PASS\n");
    return 0;
}