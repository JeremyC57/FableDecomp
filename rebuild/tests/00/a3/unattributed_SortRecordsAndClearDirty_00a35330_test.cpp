// Behaviour test for the 0x00A35330 sorted-container Sort(): the whole [data, data + count) range is sorted with the comparator, then Dirty is cleared.
#include <cstdio>
struct CRecord38 { long m_key; unsigned char _rest[0x34]; };
struct CRecordCompare { bool operator()(const CRecord38& a, const CRecord38& b) const { return a.m_key < b.m_key; } };
static void SortRecords(CRecord38* first, CRecord38* last, CRecordCompare compare)
{
    for (CRecord38* i = first; i != last; ++i)
        for (CRecord38* j = i + 1; j != last; ++j)
            if (compare(*j, *i)) { CRecord38 t = *i; *i = *j; *j = t; }
}
struct CSortedRecords {
    CRecordCompare Compare;
    CRecord38* m_pData;
    long m_count;
    bool Dirty;
    void SortRecordsAndClearDirty()
    {
        SortRecords(m_pData, m_pData + m_count, Compare);
        Dirty = false;
    }
};
int main()
{
    CRecord38 r[5];
    long keys[5] = { 5, 1, 4, 2, 3 };
    for (int i = 0; i < 5; ++i) r[i].m_key = keys[i];
    CSortedRecords s;
    s.m_pData = r; s.m_count = 4; s.Dirty = true;   // the fifth record lies outside the range
    s.SortRecordsAndClearDirty();
    if (s.Dirty || r[0].m_key != 1 || r[1].m_key != 2 || r[2].m_key != 4 || r[3].m_key != 5 || r[4].m_key != 3) { std::printf("00a35330_TEST FAIL\n"); return 1; }
    std::printf("00a35330_TEST PASS\n");
    return 0;
}