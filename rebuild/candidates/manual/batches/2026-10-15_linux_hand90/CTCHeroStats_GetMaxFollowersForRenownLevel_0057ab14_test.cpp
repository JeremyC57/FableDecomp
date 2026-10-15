#include <cstdio>
// Behaviour of CTCHeroStats::GetMaxFollowersForRenownLevel (0x0057AB14): the table entry for the renown level, the last entry past the
// end of the table, 0 for an empty table; the argument is ignored.
struct CLongVector {
    long* m_pFirst; long* m_pLast; long* m_pEnd;
    unsigned long size() const { return (unsigned long)(m_pLast - m_pFirst); }
    const long& operator[](unsigned long index) const { return m_pFirst[index]; }
};
struct CHeroStatsDef { CLongVector RenownLevelMaxFollowers; };
struct CDefinitionHolder { CHeroStatsDef* PHeroStatsDef; };
static CDefinitionHolder* g_holder = 0;
struct CTCHeroStats_Retail {
    long RenownLevel;
    CDefinitionHolder* GetDefinitions() const { return g_holder; }
    long GetMaxFollowersForRenownLevel(long unused) const
    {
        const CHeroStatsDef* def = GetDefinitions()->PHeroStatsDef;
        if (def->RenownLevelMaxFollowers.size() > 0) {
            unsigned long level = (unsigned long)RenownLevel < def->RenownLevelMaxFollowers.size()
                ? RenownLevel : def->RenownLevelMaxFollowers.size() - 1;
            return def->RenownLevelMaxFollowers[level];
        }
        return 0;
    }
};
int main()
{
    long table[3] = { 5, 10, 20 };
    CHeroStatsDef def; def.RenownLevelMaxFollowers.m_pFirst = table; def.RenownLevelMaxFollowers.m_pLast = table + 3; def.RenownLevelMaxFollowers.m_pEnd = table + 3;
    CDefinitionHolder holder; holder.PHeroStatsDef = &def; g_holder = &holder;
    CTCHeroStats_Retail s; s.RenownLevel = 1;
    if (s.GetMaxFollowersForRenownLevel(99) != 10) { std::printf("0057ab14_TEST FAIL\n"); return 1; }
    s.RenownLevel = 7;
    if (s.GetMaxFollowersForRenownLevel(0) != 20) { std::printf("0057ab14_TEST FAIL\n"); return 1; }
    def.RenownLevelMaxFollowers.m_pLast = table;
    if (s.GetMaxFollowersForRenownLevel(0) != 0) { std::printf("0057ab14_TEST FAIL\n"); return 1; }
    std::printf("0057ab14_TEST PASS\n");
    return 0;
}
