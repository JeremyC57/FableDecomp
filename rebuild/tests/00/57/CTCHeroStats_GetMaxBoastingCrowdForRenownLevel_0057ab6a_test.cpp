#include <cstdio>
// Behaviour of CTCHeroStats::GetMaxBoastingCrowdForRenownLevel (0x0057AB6A): the table entry for the renown level, the last entry past the
// end of the table, 0 for an empty table; the argument is ignored.
struct CLongVector {
    long* m_pFirst; long* m_pLast; long* m_pEnd;
    unsigned long size() const { return (unsigned long)(m_pLast - m_pFirst); }
    const long& operator[](unsigned long index) const { return m_pFirst[index]; }
};
class CHeroStatsDef { public: CLongVector RenownLevelMaxBoastingCrowd; };
struct CDefinitionHolder { CHeroStatsDef* PHeroStatsDef; };
static CDefinitionHolder* g_holder = 0;
struct CTCHeroStats_Retail {
    long RenownLevel;
    CDefinitionHolder* GetDefinitions() const { return g_holder; }
    long GetMaxBoastingCrowdForRenownLevel(long unused) const
    {
        const CHeroStatsDef* def = GetDefinitions()->PHeroStatsDef;
        if (def->RenownLevelMaxBoastingCrowd.size() > 0) {
            unsigned long level = (unsigned long)RenownLevel < def->RenownLevelMaxBoastingCrowd.size()
                ? RenownLevel : def->RenownLevelMaxBoastingCrowd.size() - 1;
            return def->RenownLevelMaxBoastingCrowd[level];
        }
        return 0;
    }
};
int main()
{
    long table[3] = { 5, 10, 20 };
    CHeroStatsDef def; def.RenownLevelMaxBoastingCrowd.m_pFirst = table; def.RenownLevelMaxBoastingCrowd.m_pLast = table + 3; def.RenownLevelMaxBoastingCrowd.m_pEnd = table + 3;
    CDefinitionHolder holder; holder.PHeroStatsDef = &def; g_holder = &holder;
    CTCHeroStats_Retail s; s.RenownLevel = 1;
    if (s.GetMaxBoastingCrowdForRenownLevel(99) != 10) { std::printf("0057ab6a_TEST FAIL\n"); return 1; }
    s.RenownLevel = 7;
    if (s.GetMaxBoastingCrowdForRenownLevel(0) != 20) { std::printf("0057ab6a_TEST FAIL\n"); return 1; }
    def.RenownLevelMaxBoastingCrowd.m_pLast = table;
    if (s.GetMaxBoastingCrowdForRenownLevel(0) != 0) { std::printf("0057ab6a_TEST FAIL\n"); return 1; }
    std::printf("0057ab6a_TEST PASS\n");
    return 0;
}