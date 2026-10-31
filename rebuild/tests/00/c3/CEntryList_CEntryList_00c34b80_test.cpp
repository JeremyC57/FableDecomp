// Behaviour test for 00c34b80: CEntryList::~CEntryList -- removes the last entry until empty (entries not owned), frees the pointer array
#include <cstdio>
static int g_arrayFrees = 0;
template <class T>
struct CPtrArray {
    T* m_data; unsigned int m_capacity; unsigned int m_count;
    ~CPtrArray() { if (m_data) { delete[] m_data; ++g_arrayFrees; m_data = 0; } }
    long Find(T value)
    {
        long index = -1;
        for (long i = (long)m_count - 1; i >= 0 && index < 0; --i)
            if (m_data[i] == value) index = i;
        return index;
    }
    void Remove(T value)
    {
        for (unsigned int i = (unsigned int)Find(value); i < m_count - 1; ++i) m_data[i] = m_data[i + 1];
        --m_count;
    }
};
struct CListEntry { int id; };
static int g_baseDtors = 0;
struct CItemTable { virtual ~CItemTable() { ++g_baseDtors; } };
struct CEntryList : CItemTable {
    CPtrArray<CListEntry*> m_entries;
    int m_seenCounts[8]; int m_n;
    ~CEntryList()
    {
        m_n = 0;
        for (unsigned int count = m_entries.m_count; count != 0; count = m_entries.m_count) {
            m_seenCounts[m_n++] = (int)count;
            m_entries.Remove(m_entries.m_data[count - 1]);
        }
        if (m_n != 3 || m_seenCounts[0] != 3 || m_seenCounts[1] != 2 || m_seenCounts[2] != 1 || m_entries.m_count != 0) g_baseDtors = -100;
    }
};
int main()
{
    CListEntry e[3] = { { 1 }, { 2 }, { 3 } };
    {
        CEntryList list;
        list.m_entries.m_data = new CListEntry*[4];
        list.m_entries.m_capacity = 4; list.m_entries.m_count = 3;
        list.m_entries.m_data[0] = &e[0]; list.m_entries.m_data[1] = &e[1]; list.m_entries.m_data[2] = &e[2];
    }
    if (g_baseDtors != 1 || g_arrayFrees != 1 || e[0].id != 1 || e[2].id != 3) { std::printf("00c34b80_TEST FAIL\n"); return 1; }
    std::printf("00c34b80_TEST PASS\n");
    return 0;
}