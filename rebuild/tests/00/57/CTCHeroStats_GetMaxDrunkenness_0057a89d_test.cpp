// Behaviour test for 0057a89d CTCHeroStats::GetMaxDrunkenness: the row body verbatim over a fixture model of the shared-header interfaces it uses
// (CTCBase::GetDefinitionManager returning the game definition manager, CGameDefinitionManager::PeekHeroStatsDef, CHeroStatsDef fields).
#include <stdio.h>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
class CHeroStatsDef { public:
    float NumberOfSecondsAfterChangeHealthStartsRefilling; float PintsToDrunk; float PintsToVomit; long MaxMorality;
};
class CGameDefinitionManager { public:
    const CHeroStatsDef* PHeroStatsDef;
    const CHeroStatsDef& PeekHeroStatsDef() const { return *PHeroStatsDef; }
};
class CMainGameComponent { public: static long ConstantFPS; static long GetConstantFPS() { return ConstantFPS; } };
long CMainGameComponent::ConstantFPS = 30;
static int g_managerCalls = 0;
class CTCHeroStats { public:
    float Drunkenness; long FramesUntilHealthRefills; const CGameDefinitionManager* Manager;
    const CGameDefinitionManager& GetDefinitionManager() const { ++g_managerCalls; return *Manager; }
    float GetMaxDrunkenness() const;
};
// ---- row body (verbatim) ----
float CTCHeroStats::GetMaxDrunkenness() const
{
    return GetDefinitionManager().PeekHeroStatsDef().PintsToVomit;
}
// ---- harness ----
int main()
{
    CHeroStatsDef def; def.NumberOfSecondsAfterChangeHealthStartsRefilling = 2.5f; def.PintsToDrunk = 3.0f; def.PintsToVomit = 6.5f;
    def.MaxMorality = 1000;
    CGameDefinitionManager manager; manager.PHeroStatsDef = &def;
    CTCHeroStats stats; stats.Manager = &manager; stats.Drunkenness = 0.0f; stats.FramesUntilHealthRefills = -1;
    CHECK(stats.GetMaxDrunkenness() == 6.5f && g_managerCalls == 1);
    def.PintsToVomit = 1.0f; CHECK(stats.GetMaxDrunkenness() == 1.0f && g_managerCalls == 2);
    if (fails == 0) printf("0057a89d_TEST PASS\n");
    return fails;
}
