// Behaviour test for 00c34e60: CRowTable::~CRowTable -- removes and deletes the last row until empty, frees the pointer array
#include <cstdio>
static int g_arrayFrees = 0, g_rowDtors = 0;
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
struct CTableRow { int id; ~CTableRow() { ++g_rowDtors; } };
static int g_baseDtors = 0;
struct CItemTable { virtual ~CItemTable() { ++g_baseDtors; } };
struct CRowTable : CItemTable {
    CPtrArray<CTableRow*> m_entries;
    ~CRowTable()
    {
        for (unsigned int count = m_entries.m_count; count != 0; count = m_entries.m_count) {
            CTableRow* row = m_entries.m_data[count - 1];
            m_entries.Remove(row);
            delete row;
        }
    }
};
int main()
{
    {
        CRowTable t;
        t.m_entries.m_data = new CTableRow*[4];
        t.m_entries.m_capacity = 4; t.m_entries.m_count = 3;
        for (int i = 0; i < 3; ++i) { t.m_entries.m_data[i] = new CTableRow; t.m_entries.m_data[i]->id = i; }
    }
    if (g_rowDtors != 3 || g_baseDtors != 1 || g_arrayFrees != 1) { std::printf("00c34e60_TEST FAIL\n"); return 1; }
    std::printf("00c34e60_TEST PASS\n");
    return 0;
}
