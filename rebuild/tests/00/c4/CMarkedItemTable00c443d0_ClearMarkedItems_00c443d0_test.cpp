#include <cstdio>
#ifdef _MSC_VER
typedef __int64 i64;
#else
typedef long long i64;
#endif
// Behaviour test for CMarkedItemTable_00c443d0::ClearMarkedItems (0x00C443D0): same logic over a copy of the model.
struct CMarkedEntry {
    long m_marks;
    void ReleaseMark() { if (m_marks) --m_marks; }
};
struct CMarkedItemTable_00c443d0 {
    CMarkedEntry** m_ppEntries; i64* m_pRecords; unsigned long m_count;
    long m_markedCount, m_markedBytes; CMarkedEntry* m_pCurrent; long m_first, m_last, m_cursor;
    void ClearMarkedItems()
    {
        for (unsigned long i = 0; i < m_count; ++i) {
            if (m_ppEntries[i] != 0) {
                m_pCurrent = m_ppEntries[i];
                m_ppEntries[i]->ReleaseMark();
                m_ppEntries[i] = 0;
                m_pRecords[i] = 0;
            }
        }
        m_markedCount = 0; m_markedBytes = 0; m_first = 0; m_last = 0; m_cursor = 0; m_pCurrent = 0;
    }
};
int main()
{
    CMarkedEntry a = { 2 }, b = { 0 };
    CMarkedEntry* slots[3] = { &a, 0, &b };
    i64 recs[3] = { 11, 22, 33 };
    CMarkedItemTable_00c443d0 t;
    t.m_ppEntries = slots; t.m_pRecords = recs; t.m_count = 3;
    t.m_markedCount = 5; t.m_markedBytes = 6; t.m_pCurrent = &a; t.m_first = 1; t.m_last = 2; t.m_cursor = 3;
    t.ClearMarkedItems();
    if (a.m_marks != 1 || b.m_marks != 0 || slots[0] || slots[2] || recs[0] != 0 || recs[1] != 22 || recs[2] != 0
        || t.m_markedCount || t.m_markedBytes || t.m_pCurrent || t.m_first || t.m_last || t.m_cursor) { std::printf("00c443d0_TEST FAIL\n"); return 1; }
    std::printf("00c443d0_TEST PASS\n");
    return 0;
}