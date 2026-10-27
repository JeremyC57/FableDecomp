// Test for 0x00C34FF0 CItemGridTable_012b501c::Remove: an emptied row leaves the row list and is deleted.
#include <cstdio>
static int g_deleted = 0;
template <class T> struct CPtrArray {
    T* m_data; unsigned int m_capacity; unsigned int m_count;
    CPtrArray() : m_data(new T[4]), m_capacity(4), m_count(0) { }
    ~CPtrArray() { delete[] m_data; ++g_deleted; }
    void Add(T v) { m_data[m_count++] = v; }
    unsigned int GetCount() { return m_count; }
    void RemoveAt(unsigned int i) { for (; i + 1 < m_count; ++i) m_data[i] = m_data[i + 1]; --m_count; }
    void Remove(T v) { long idx = -1; for (long i = (long)m_count - 1; i >= 0 && idx < 0; --i) if (m_data[i] == v) idx = i;
        for (unsigned int i = (unsigned int)idx; i < m_count - 1; ++i) m_data[i] = m_data[i + 1]; --m_count; }
};
struct CTableEntry { int id; };
struct CPlaybackEntry : CPtrArray<CTableEntry*> { };
struct CGrid { CPtrArray<CPlaybackEntry*> m_rows;
    void Remove(unsigned int row, unsigned int column) {
        CPlaybackEntry* entry = m_rows.m_data[row];
        entry->RemoveAt(column);
        if (entry->GetCount() == 0) { m_rows.Remove(entry); delete entry; }
    } };
int main()
{
    CTableEntry a = {1}, b = {2}, c = {3};
    CGrid g; CPlaybackEntry* r0 = new CPlaybackEntry; CPlaybackEntry* r1 = new CPlaybackEntry;
    r0->Add(&a); r0->Add(&b); r1->Add(&c); g.m_rows.Add(r0); g.m_rows.Add(r1);
    g.Remove(0, 0);
    bool ok = r0->m_count == 1 && r0->m_data[0] == &b && g.m_rows.m_count == 2 && g_deleted == 0;
    g.Remove(1, 0);
    ok = ok && g.m_rows.m_count == 1 && g.m_rows.m_data[0] == r0 && g_deleted == 1;
    if (!ok) { std::printf("00c34ff0_TEST FAIL\n"); return 1; }
    delete r0;
    std::printf("00c34ff0_TEST PASS\n");
    return 0;
}