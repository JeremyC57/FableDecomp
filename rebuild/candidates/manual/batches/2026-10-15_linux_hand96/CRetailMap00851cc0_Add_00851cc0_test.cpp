#include <cstdio>
struct CRetailMap_00851cc0 {
    struct SValue { long first; long second; };
    struct SInsertResult { long index; bool second; };
    SValue m_items[8]; long m_size;
    SInsertResult insert(const SValue& value)
    {
        SInsertResult r;
        for (long i = 0; i < m_size; ++i) if (m_items[i].first == value.first) { r.index = i; r.second = false; return r; }
        m_items[m_size] = value; r.index = m_size++; r.second = true; return r;
    }
    void Add(long key, long value) { SValue entry; entry.first = key; entry.second = value; insert(entry); }
};
int main()
{
    CRetailMap_00851cc0 m; m.m_size = 0;
    m.Add(5, 50); m.Add(7, 70); m.Add(5, 99);
    if (m.m_size != 2 || m.m_items[0].second != 50 || m.m_items[1].first != 7 || m.m_items[1].second != 70) { std::printf("00851cc0_TEST FAIL\n"); return 1; }
    std::printf("00851cc0_TEST PASS\n");
    return 0;
}
