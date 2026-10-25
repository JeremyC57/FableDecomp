// Behaviour test for 00c1ec70: CVisitedItem::MatchesKey -- while the claim source finds another (first, second) match for the key, hand the pair back to it; return the count
#include <cstdio>

struct CClaimSource {
    long m_pairs[4][3]; int m_next; int m_n;
    long m_got[4][2]; int m_processed; int m_sourceCalls;
    virtual void Process(long first, long second) { m_got[m_processed][0] = first; m_got[m_processed][1] = second; ++m_processed; }
    virtual long FindNext(long key, long* first, long* second)
    {
        while (m_next < m_n) {
            long* p = m_pairs[m_next++];
            if (p[0] == key) { *first = p[1]; *second = p[2]; return 1; }
        }
        return 0;
    }
};

struct CVisitedItem {
    CClaimSource* m_pSource;
    virtual CClaimSource* GetClaimSource() { ++m_pSource->m_sourceCalls; return m_pSource; }
    long MatchesKey(long key)
    {
        long count = 0;
        long first, second;
        while (GetClaimSource()->FindNext(key, &first, &second)) {
            GetClaimSource()->Process(first, second);
            ++count;
        }
        return count;
    }
};

int main()
{
    CClaimSource src; src.m_next = 0; src.m_n = 4; src.m_processed = 0; src.m_sourceCalls = 0;
    long pairs[4][3] = { { 5, 10, 11 }, { 6, 20, 21 }, { 5, 30, 31 }, { 5, 40, 41 } };
    for (int i = 0; i < 4; ++i) for (int j = 0; j < 3; ++j) src.m_pairs[i][j] = pairs[i][j];
    CVisitedItem item; item.m_pSource = &src;
    if (item.MatchesKey(5) != 3 || src.m_processed != 3) { std::printf("00c1ec70_TEST FAIL\n"); return 1; }
    if (src.m_got[0][0] != 10 || src.m_got[0][1] != 11 || src.m_got[2][0] != 40 || src.m_got[2][1] != 41) { std::printf("00c1ec70_TEST FAIL\n"); return 1; }
    if (src.m_sourceCalls != 7) { std::printf("00c1ec70_TEST FAIL\n"); return 1; }   // fetched afresh for every call
    src.m_next = 0;
    if (item.MatchesKey(9) != 0) { std::printf("00c1ec70_TEST FAIL\n"); return 1; }
    std::printf("00c1ec70_TEST PASS\n");
    return 0;
}