// behaviour test for 0x00C21E60 CPanelItemSet::FindItem: the first pending item when its key is below the argument
#include <cstdio>

struct CPanelItem { unsigned long m_key; };

struct CPanelItemQueue {
    CPanelItem* m_first;
    virtual CPanelItem* GetFirst() { return m_first; }        // slot 11 in retail
    virtual ~CPanelItemQueue() {}
};

struct CPanelItemSet {
    CPanelItemQueue m_pending;
    CPanelItem* FindItem(unsigned long key);
};

CPanelItem* CPanelItemSet::FindItem(unsigned long key)
{
    CPanelItem* item = m_pending.GetFirst();
    if (item && item->m_key < key)
        return item;
    return 0;
}

int main()
{
    CPanelItem item; item.m_key = 10;
    CPanelItemSet set; set.m_pending.m_first = 0;
    if (set.FindItem(100) != 0) { std::printf("00c21e60_TEST FAIL\n"); return 1; }
    set.m_pending.m_first = &item;
    if (set.FindItem(11) != &item || set.FindItem(10) != 0 || set.FindItem(0) != 0 || set.FindItem(0xFFFFFFFFu) != &item) { std::printf("00c21e60_TEST FAIL\n"); return 1; }
    std::printf("00c21e60_TEST PASS\n");
    return 0;
}