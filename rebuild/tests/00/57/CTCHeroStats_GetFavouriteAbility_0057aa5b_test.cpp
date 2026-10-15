#include <cstdio>
// Behaviour of CTCHeroStats::GetFavouriteAbility (0x0057AA5B): index of the highest positive use count, first wins ties, -1 if none.
struct CLongVector {
    long* m_pFirst; long* m_pLast; long* m_pEnd;
    unsigned long size() const { return (unsigned long)(m_pLast - m_pFirst); }
    const long& operator[](unsigned long index) const { return m_pFirst[index]; }
};
struct CTCHeroStats_Retail {
    CLongVector NumberOfTimesUsedAbility;
    long GetFavouriteAbility() const
    {
        long favourite = -1;
        long mostUses = -1;
        for (unsigned long ability = 0; ability != NumberOfTimesUsedAbility.size(); ++ability) {
            long uses = NumberOfTimesUsedAbility[ability];
            if (uses > mostUses && uses > 0) {
                mostUses = uses;
                favourite = ability;
            }
        }
        return favourite;
    }
};
int main()
{
    long uses[5] = { 0, 4, 9, 9, 2 };
    long none[3] = { 0, 0, 0 };
    CTCHeroStats_Retail s;
    s.NumberOfTimesUsedAbility.m_pFirst = uses; s.NumberOfTimesUsedAbility.m_pLast = uses + 5; s.NumberOfTimesUsedAbility.m_pEnd = uses + 5;
    if (s.GetFavouriteAbility() != 2) { std::printf("0057aa5b_TEST FAIL\n"); return 1; }
    s.NumberOfTimesUsedAbility.m_pFirst = none; s.NumberOfTimesUsedAbility.m_pLast = none + 3;
    if (s.GetFavouriteAbility() != -1) { std::printf("0057aa5b_TEST FAIL\n"); return 1; }
    s.NumberOfTimesUsedAbility.m_pLast = none;
    if (s.GetFavouriteAbility() != -1) { std::printf("0057aa5b_TEST FAIL\n"); return 1; }
    std::printf("0057aa5b_TEST PASS\n");
    return 0;
}