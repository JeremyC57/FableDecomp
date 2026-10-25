// Behaviour test for CSharedEntryTable::Acquire @ 0x00C2A3D0 (find or create the key's shared entry, add one reference).
#include <cstdio>

class CSharedEntryTable;

class CSharedEntry {
public:
    CSharedEntry(CSharedEntryTable* table, void* key);
    CSharedEntryTable* m_pTable;
    long               m_index;
    void*              m_key;
    long               m_0c;
    long               m_refCount;
};

struct CSharedEntryList {
    CSharedEntry* m_items[8];
    int           m_count;
    CSharedEntry* Find(const unsigned long& key)
    {
        for (int i = 0; i < m_count; ++i)
            if ((unsigned long)m_items[i]->m_key == key)
                return m_items[i];
        return 0;
    }
    void Add(CSharedEntry* entry) { m_items[m_count++] = entry; }
};

class CSharedEntryTable {
public:
    CSharedEntryList m_entries;
    CSharedEntry* Acquire(void* key)
    {
        CSharedEntry* entry = m_entries.Find((unsigned long)key);
        if (!entry)
            entry = new CSharedEntry(this, key);
        ++entry->m_refCount;
        return entry;
    }
};

CSharedEntry::CSharedEntry(CSharedEntryTable* table, void* key)
    : m_pTable(table), m_index(-1), m_key(key), m_0c(0), m_refCount(0)
{
    table->m_entries.Add(this);
}

int main()
{
    CSharedEntryTable table;
    table.m_entries.m_count = 0;
    int k1, k2;
    CSharedEntry* a = table.Acquire(&k1);
    CSharedEntry* b = table.Acquire(&k1);
    CSharedEntry* c = table.Acquire(&k2);
    bool ok = a == b && a != c && a->m_refCount == 2 && c->m_refCount == 1 && table.m_entries.m_count == 2;
    ok = ok && a->m_pTable == &table && a->m_index == -1 && a->m_key == &k1 && c->m_key == &k2;
    delete a;
    delete c;
    if (!ok) { std::printf("00c2a3d0_TEST FAIL\n"); return 1; }
    std::printf("00c2a3d0_TEST PASS\n");
    return 0;
}