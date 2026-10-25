// Behaviour test for 00c1ecf0: CVisitedItem::ContainsValue -- 1 when any entry of any group of the claim source equals the value, else 0
#include <cstdio>

struct CClaimSource {
    long m_values[3][4]; unsigned int m_counts[3]; unsigned int m_groups; int m_reads;
    virtual unsigned int GetCount(unsigned int group) { return m_counts[group]; }
    virtual unsigned int GetGroupCount() { return m_groups; }
    virtual long GetAt(unsigned int group, unsigned int index) { ++m_reads; return m_values[group][index]; }
};

struct CVisitedItem {
    CClaimSource* m_pSource;
    virtual CClaimSource* GetClaimSource() { return m_pSource; }
    long ContainsValue(long value)
    {
        for (unsigned int group = 0; group < GetClaimSource()->GetGroupCount(); ++group)
            for (unsigned int index = 0; index < GetClaimSource()->GetCount(group); ++index)
                if (value == GetClaimSource()->GetAt(group, index))
                    return 1;
        return 0;
    }
};

int main()
{
    CClaimSource src; src.m_groups = 3; src.m_counts[0] = 2; src.m_counts[1] = 0; src.m_counts[2] = 3;
    long v[3][4] = { { 1, 2, 0, 0 }, { 7, 7, 7, 7 }, { 3, 4, 5, 0 } };
    for (int g = 0; g < 3; ++g) for (int i = 0; i < 4; ++i) src.m_values[g][i] = v[g][i];
    CVisitedItem item; item.m_pSource = &src;
    src.m_reads = 0;
    if (item.ContainsValue(4) != 1 || src.m_reads != 4) { std::printf("00c1ecf0_TEST FAIL\n"); return 1; }   // stops at the hit
    src.m_reads = 0;
    if (item.ContainsValue(7) != 0 || src.m_reads != 5) { std::printf("00c1ecf0_TEST FAIL\n"); return 1; }   // group 1 is empty
    if (item.ContainsValue(1) != 1) { std::printf("00c1ecf0_TEST FAIL\n"); return 1; }
    src.m_groups = 0;
    if (item.ContainsValue(1) != 0) { std::printf("00c1ecf0_TEST FAIL\n"); return 1; }
    std::printf("00c1ecf0_TEST PASS\n");
    return 0;
}