#include <cstdio>
// Behaviour of CTCHeroStats::GetPercentageThroughRenownLevel (0x0057AAD9): points / level cost inside the table, else 0.
struct CLongVector {
    long* m_pFirst; long* m_pLast; long* m_pEnd;
    unsigned long size() const { return (unsigned long)(m_pLast - m_pFirst); }
    const long& operator[](unsigned long index) const { return m_pFirst[index]; }
};
struct CHeroStatsDef { CLongVector RenownLevelCosts; };
struct CDefinitionHolder { CHeroStatsDef* PHeroStatsDef; };
static CDefinitionHolder* g_holder = 0;
struct CTCHeroStats_Retail {
    long RenownLevel;
    long RenownMaxLevel;
    long RenownPointsInLevel;
    CDefinitionHolder* GetDefinitions() const { return g_holder; }
    float GetPercentageThroughRenownLevel() const
    {
        const CHeroStatsDef* def = GetDefinitions()->PHeroStatsDef;
        if (RenownLevel >= 0 && (unsigned long)RenownLevel < def->RenownLevelCosts.size())
            return (float)RenownPointsInLevel / def->RenownLevelCosts[RenownLevel];
        return 0.0f;
    }
};
int main()
{
    long costs[3] = { 100, 200, 400 };
    CHeroStatsDef def; def.RenownLevelCosts.m_pFirst = costs; def.RenownLevelCosts.m_pLast = costs + 3; def.RenownLevelCosts.m_pEnd = costs + 3;
    CDefinitionHolder holder; holder.PHeroStatsDef = &def; g_holder = &holder;
    CTCHeroStats_Retail s; s.RenownMaxLevel = 3; s.RenownLevel = 1; s.RenownPointsInLevel = 50;
    if (s.GetPercentageThroughRenownLevel() != 0.25f) { std::printf("0057aad9_TEST FAIL\n"); return 1; }
    s.RenownLevel = 3;
    if (s.GetPercentageThroughRenownLevel() != 0.0f) { std::printf("0057aad9_TEST FAIL\n"); return 1; }
    s.RenownLevel = -1;
    if (s.GetPercentageThroughRenownLevel() != 0.0f) { std::printf("0057aad9_TEST FAIL\n"); return 1; }
    std::printf("0057aad9_TEST PASS\n");
    return 0;
}
