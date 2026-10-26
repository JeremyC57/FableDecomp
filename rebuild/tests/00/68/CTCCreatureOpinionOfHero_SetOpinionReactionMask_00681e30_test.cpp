// Test for 00681e30 CTCCreatureOpinionOfHero::SetOpinionReactionMask(const CCharString&): name -> def index; apply only a non-zero index.
#include <cstdio>
#include <string.h>
class CCharString { public: const char* Text; };
class CGameDefinitionManager { public:
    long GetDefGlobalIndexFromName(const CCharString& name) const
    {
        if (strcmp(name.Text, "OPINION_MASK_A") == 0) return 17;
        if (strcmp(name.Text, "OPINION_MASK_B") == 0) return 42;
        return 0;
    }
};
class CWorld { public: CGameDefinitionManager* DefinitionManager; };
class CTCCreatureOpinionOfHero { public:
    CWorld* World; long Applied; int ApplyCalls;
    CWorld* GetWorld() const { return World; }
    void SetOpinionReactionMask(long defIndex) { Applied = defIndex; ++ApplyCalls; }
    void SetOpinionReactionMask(const CCharString& defName)
    {
        CGameDefinitionManager* definitions = GetWorld()->DefinitionManager;
        long defIndex = definitions->GetDefGlobalIndexFromName(defName);
        if (defIndex)
            SetOpinionReactionMask(defIndex);
    }
};
int main()
{
    CGameDefinitionManager m; CWorld w = { &m };
    CTCCreatureOpinionOfHero o = { &w, -1, 0 };
    CCharString a = { "OPINION_MASK_A" }, b = { "OPINION_MASK_B" }, none = { "UNKNOWN" };
    o.SetOpinionReactionMask(none);
    if (o.ApplyCalls != 0 || o.Applied != -1) { std::printf("00681e30_TEST FAIL\n"); return 1; }
    o.SetOpinionReactionMask(a);
    if (o.ApplyCalls != 1 || o.Applied != 17) { std::printf("00681e30_TEST FAIL\n"); return 1; }
    o.SetOpinionReactionMask(b);
    if (o.ApplyCalls != 2 || o.Applied != 42) { std::printf("00681e30_TEST FAIL\n"); return 1; }
    std::printf("00681e30_TEST PASS\n");
    return 0;
}