// behaviour test for 0x00C11CB0 CPanel::UpdateItems: activate every active item the panel has not handled yet
#include <cstdio>

struct CPanel;

struct CPanelItem {
    long m_state;
    bool m_handled;
    int m_activations;
    long IsActive() const { return m_state != 0; }               // stand-in for 0x00C20840
    bool IsHandled(CPanel*) { return m_handled; }                // stand-in for 0x00C20C10
    void Activate(CPanel*) { ++m_activations; }                  // stand-in for 0x00C21620
};

struct CPanelItemArray {
    unsigned long m_count;
    CPanelItem* m_pItems;
    CPanelItem& operator[](unsigned long index) { return m_pItems[index]; }   // 0x00C13620
};

struct CPanelItemSet {
    CPanelItemArray m_items;
};

struct CPanel {
    CPanelItemSet m_itemSet;
    void UpdateItems();
};

void CPanel::UpdateItems()
{
    for (unsigned long i = 0; i < m_itemSet.m_items.m_count; ++i) {
        CPanelItem& item = m_itemSet.m_items[i];
        if (item.IsActive()) {
            if (!item.IsHandled(this))
                item.Activate(this);
        }
    }
}

int main()
{
    CPanelItem items[4] = { { 1, false, 0 }, { 0, false, 0 }, { 2, true, 0 }, { 3, false, 0 } };
    CPanel panel; panel.m_itemSet.m_items.m_count = 4; panel.m_itemSet.m_items.m_pItems = items;
    panel.UpdateItems();
    if (items[0].m_activations != 1 || items[1].m_activations != 0 || items[2].m_activations != 0 || items[3].m_activations != 1) { std::printf("00c11cb0_TEST FAIL\n"); return 1; }
    panel.m_itemSet.m_items.m_count = 0;
    panel.UpdateItems();
    if (items[0].m_activations != 1) { std::printf("00c11cb0_TEST FAIL\n"); return 1; }
    std::printf("00c11cb0_TEST PASS\n");
    return 0;
}