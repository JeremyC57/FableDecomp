// behaviour test for 0x00C21EB0 CPanelItemSet::FindEntry: the first entry when the key is "no limit" or the entry's key is below it
#include <cstdio>

struct CPanelEntry { void* m_pItem; unsigned int m_key; };

struct CPanelEntryQueue {
    CPanelEntry* m_first;
    virtual CPanelEntry* GetFirst() { return m_first; }       // slot 11 in retail
    virtual ~CPanelEntryQueue() {}
};

struct CPanelItemSet {
    CPanelEntryQueue m_entries;
    CPanelEntry* FindEntry(unsigned int key);
};

CPanelEntry* CPanelItemSet::FindEntry(unsigned int key)
{
    CPanelEntry* entry = m_entries.GetFirst();
    if (entry && (key >= 0xFFFFFFFFu || entry->m_key < key))
        return entry;
    return 0;
}

int main()
{
    CPanelEntry e; e.m_pItem = 0; e.m_key = 50;
    CPanelItemSet set; set.m_entries.m_first = 0;
    if (set.FindEntry(0xFFFFFFFFu) != 0 || set.FindEntry(51) != 0) { std::printf("00c21eb0_TEST FAIL\n"); return 1; }
    set.m_entries.m_first = &e;
    if (set.FindEntry(0xFFFFFFFFu) != &e || set.FindEntry(51) != &e || set.FindEntry(50) != 0 || set.FindEntry(3) != 0) { std::printf("00c21eb0_TEST FAIL\n"); return 1; }
    e.m_key = 0xFFFFFFFFu;                                    // "no limit" accepts any entry
    if (set.FindEntry(0xFFFFFFFFu) != &e || set.FindEntry(0xFFFFFFFEu) != 0) { std::printf("00c21eb0_TEST FAIL\n"); return 1; }
    std::printf("00c21eb0_TEST PASS\n");
    return 0;
}