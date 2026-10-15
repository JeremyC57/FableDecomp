#include <cstdio>
// Behaviour of CTCHeroStats::GetNumberOfAbilityCasts (0x0057AAA8): the sum of every ability's use count.
struct CLongVector {
    long* m_pFirst; long* m_pLast; long* m_pEnd;
    unsigned long size() const { return (unsigned long)(m_pLast - m_pFirst); }
    const long& operator[](unsigned long index) const { return m_pFirst[index]; }
};
struct CTCHeroStats_Retail {
    CLongVector NumberOfTimesUsedAbility;
    long GetNumberOfAbilityCasts() const
    {
        long casts = 0;
        for (unsigned long ability = 0; ability != NumberOfTimesUsedAbility.size(); ++ability)
            casts += NumberOfTimesUsedAbility[ability];
        return casts;
    }
};
int main()
{
    long uses[4] = { 3, 0, 7, 5 };
    CTCHeroStats_Retail s;
    s.NumberOfTimesUsedAbility.m_pFirst = uses; s.NumberOfTimesUsedAbility.m_pLast = uses + 4; s.NumberOfTimesUsedAbility.m_pEnd = uses + 4;
    if (s.GetNumberOfAbilityCasts() != 15) { std::printf("0057aaa8_TEST FAIL\n"); return 1; }
    s.NumberOfTimesUsedAbility.m_pLast = uses;
    if (s.GetNumberOfAbilityCasts() != 0) { std::printf("0057aaa8_TEST FAIL\n"); return 1; }
    std::printf("0057aaa8_TEST PASS\n");
    return 0;
}
