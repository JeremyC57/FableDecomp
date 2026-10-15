#include <cstdio>
struct CAnimationEntryPair { unsigned long Key; long ChainID; long Stage; };
struct CAnimationSet { const CAnimationEntryPair* First; const CAnimationEntryPair* Last; };
struct CGetMaxComboStage {
    long MaxStage; long ChainID;
    CGetMaxComboStage(long chainID) : MaxStage(-777), ChainID(chainID) { }
    void operator()(const CAnimationEntryPair& p) { if (p.ChainID == ChainID && p.Stage > MaxStage) MaxStage = p.Stage; }
};
static CGetMaxComboStage ForEachAnimation(const CAnimationEntryPair* first, const CAnimationEntryPair* last, CGetMaxComboStage f)
{
    for (; first != last; ++first) f(*first);
    return f;
}
struct CThingCreatureBase {
    CAnimationSet m_set;
    const CAnimationSet* GetCombatAnimationSet() { return &m_set; }
};
struct CTCCombat {
    long ComboChainID; CThingCreatureBase* ParentCreature;
    long GetComboMaxStage()
    {
        if (ComboChainID != -999) {
            const CAnimationSet* set = ParentCreature->GetCombatAnimationSet();
            long stage = ForEachAnimation(set->First, set->Last, CGetMaxComboStage(ComboChainID)).MaxStage;
            if (stage != -777)
                return stage;
        }
        return 0;
    }
};
int main()
{
    CAnimationEntryPair anims[4] = { { 1, 7, 1 }, { 2, 7, 3 }, { 3, 8, 9 }, { 4, 7, 2 } };
    CThingCreatureBase c; c.m_set.First = anims; c.m_set.Last = anims + 4;
    CTCCombat t; t.ParentCreature = &c;
    t.ComboChainID = 7;
    if (t.GetComboMaxStage() != 3) { std::printf("00733910_TEST FAIL\n"); return 1; }
    t.ComboChainID = 8;
    if (t.GetComboMaxStage() != 9) { std::printf("00733910_TEST FAIL\n"); return 1; }
    t.ComboChainID = 5;                                    /* no animation in the chain: sentinel -> 0 */
    if (t.GetComboMaxStage() != 0) { std::printf("00733910_TEST FAIL\n"); return 1; }
    t.ComboChainID = -999; c.m_set.First = 0; c.m_set.Last = 0;
    if (t.GetComboMaxStage() != 0) { std::printf("00733910_TEST FAIL\n"); return 1; }
    std::printf("00733910_TEST PASS\n");
    return 0;
}