// Behaviour test for 00c14ad0: ReleaseTarget -- repeatedly finds an entry registered for the target and removes it from its own list, until the registry finds none
#include <cstdio>

struct CEntry;
struct CEntryList {
    int m_removed;
    void Remove(CEntry* entry);
};
struct CEntry {
    void* m_target;
    CEntryList* m_pList;
    bool m_linked;
};
void CEntryList::Remove(CEntry* entry) { entry->m_linked = false; ++m_removed; }

class CRegistry {
public:
    CEntry* m_entries;
    int m_count;
    int m_queries;
    virtual CEntry* Find(void* const* key)
    {
        ++m_queries;
        for (int i = 0; i < m_count; ++i)
            if (m_entries[i].m_linked && m_entries[i].m_target == *key) return &m_entries[i];
        return 0;
    }
    CEntry* FindEntry(void* key) { return Find(&key); }
};

struct CEntryOwner {
    CRegistry m_registry;
    void ReleaseTarget(void* target)
    {
        for (CEntry* entry = m_registry.FindEntry(target); entry; entry = m_registry.FindEntry(target))
            entry->m_pList->Remove(entry);
    }
};

int main()
{
    int t1, t2;
    CEntryList la = { 0 }, lb = { 0 };
    CEntry entries[5] = { { &t1, &la, true }, { &t2, &la, true }, { &t1, &lb, true }, { &t1, &la, true }, { &t2, &lb, true } };
    CEntryOwner owner;
    owner.m_registry.m_entries = entries; owner.m_registry.m_count = 5; owner.m_registry.m_queries = 0;
    owner.ReleaseTarget(&t1);
    if (la.m_removed != 2 || lb.m_removed != 1 || owner.m_registry.m_queries != 4) { std::printf("00c14ad0_TEST FAIL\n"); return 1; }
    if (entries[0].m_linked || !entries[1].m_linked || entries[2].m_linked || entries[3].m_linked || !entries[4].m_linked) { std::printf("00c14ad0_TEST FAIL\n"); return 1; }
    owner.ReleaseTarget(&t1);              // nothing left: one query, no removals
    if (la.m_removed != 2 || owner.m_registry.m_queries != 5) { std::printf("00c14ad0_TEST FAIL\n"); return 1; }
    std::printf("00c14ad0_TEST PASS\n");
    return 0;
}