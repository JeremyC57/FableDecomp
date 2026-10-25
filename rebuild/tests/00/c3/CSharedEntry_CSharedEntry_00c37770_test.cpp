// Behaviour test for CSharedEntry::CSharedEntry (0x00C37770): fields initialised (owner, -1, key, 0, 0) before the
// entry registers itself with the owner's keyed list.
#include <cstdio>

class CSharedEntry;
static CSharedEntry* g_added = 0; static long g_indexAtAdd = 0; static long g_refAtAdd = 99;

struct CSharedEntryList {
    virtual ~CSharedEntryList() {}
    virtual void Add(CSharedEntry* entry);
};

class CSharedEntryTable {
public:
    unsigned char    _pad_0x00[0x38];
    CSharedEntryList m_entries;
};

class CSharedEntry {
public:
    CSharedEntry(CSharedEntryTable* table, void* key);
    CSharedEntryTable* m_pTable;
    long               m_index;
    void*              m_key;
    long               m_0c;
    long               m_refCount;
};

void CSharedEntryList::Add(CSharedEntry* entry) { g_added = entry; g_indexAtAdd = entry->m_index; g_refAtAdd = entry->m_refCount; }

CSharedEntry::CSharedEntry(CSharedEntryTable* table, void* key)
    : m_pTable(table), m_index(-1), m_key(key), m_0c(0), m_refCount(0)
{
    table->m_entries.Add(this);
}

int main()
{
    CSharedEntryTable table;
    int keyObject = 5;
    CSharedEntry e(&table, &keyObject);
    if (e.m_pTable != &table || e.m_index != -1 || e.m_key != &keyObject || e.m_0c != 0 || e.m_refCount != 0
        || g_added != &e || g_indexAtAdd != -1 || g_refAtAdd != 0)
        { std::printf("00c37770_TEST FAIL\n"); return 1; }
    std::printf("00c37770_TEST PASS\n");
    return 0;
}