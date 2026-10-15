// Behaviour test for 0x0088b910 FindLastRecordBelow
#include <cstdio>
struct SKeyedRecord { long m_key; long m_a; long m_b; };
template <class T> struct CArray { T* m_pBegin; T* m_pEnd; T* m_pCapacity;
    unsigned long Size() const { return m_pEnd - m_pBegin; } const T& operator[](unsigned long i) const { return m_pBegin[i]; } };
struct COwner {
    CArray<SKeyedRecord> m_records;
    const SKeyedRecord* FindLastRecordBelow(long key) const
    {
        const SKeyedRecord* found = 0;
        for (unsigned long i = 0; i < m_records.Size(); ++i)
            if (m_records[i].m_key < key) found = &m_records[i];
        return found;
    }
};
int main()
{
    SKeyedRecord r[4] = { { -5, 0, 0 }, { 10, 0, 0 }, { 3, 0, 0 }, { 20, 0, 0 } };
    COwner o; o.m_records.m_pBegin = r; o.m_records.m_pEnd = o.m_records.m_pCapacity = r + 4;
    bool ok = o.FindLastRecordBelow(11) == &r[2] && o.FindLastRecordBelow(-5) == 0 && o.FindLastRecordBelow(100) == &r[3] && o.FindLastRecordBelow(0) == &r[0];
    o.m_records.m_pEnd = r; ok = ok && o.FindLastRecordBelow(100) == 0;
    if (!ok) { std::printf("0088b910_TEST FAIL\n"); return 1; }
    std::printf("0088b910_TEST PASS\n");
    return 0;
}