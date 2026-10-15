// Test for 006822e0 CTCCreatureOpinionOfHero::SetOpinionDeedMask(const CCharString&): name -> def index; apply only a non-zero index.
#include <cstdio>
#include <string.h>
struct CCharString { const char* Text; };
struct CGameDefinitionManager {
    long GetDefGlobalIndexFromName(const CCharString& name) const
    {
        if (strcmp(name.Text, "OPINION_MASK_A") == 0) return 17;
        if (strcmp(name.Text, "OPINION_MASK_B") == 0) return 42;
        return 0;
    }
};
struct CWorld { CGameDefinitionManager* DefinitionManager; };
struct CTCCreatureOpinionOfHero {
    CWorld* World; long Applied; int ApplyCalls;
    CWorld* GetWorld() const { return World; }
    void SetOpinionDeedMask(long defIndex) { Applied = defIndex; ++ApplyCalls; }
    void SetOpinionDeedMask(const CCharString& defName)
    {
        CGameDefinitionManager* definitions = GetWorld()->DefinitionManager;
        long defIndex = definitions->GetDefGlobalIndexFromName(defName);
        if (defIndex)
            SetOpinionDeedMask(defIndex);
    }
};
int main()
{
    CGameDefinitionManager m; CWorld w = { &m };
    CTCCreatureOpinionOfHero o = { &w, -1, 0 };
    CCharString a = { "OPINION_MASK_A" }, b = { "OPINION_MASK_B" }, none = { "UNKNOWN" };
    o.SetOpinionDeedMask(none);
    if (o.ApplyCalls != 0 || o.Applied != -1) { std::printf("006822e0_TEST FAIL\n"); return 1; }
    o.SetOpinionDeedMask(a);
    if (o.ApplyCalls != 1 || o.Applied != 17) { std::printf("006822e0_TEST FAIL\n"); return 1; }
    o.SetOpinionDeedMask(b);
    if (o.ApplyCalls != 2 || o.Applied != 42) { std::printf("006822e0_TEST FAIL\n"); return 1; }
    std::printf("006822e0_TEST PASS\n");
    return 0;
}