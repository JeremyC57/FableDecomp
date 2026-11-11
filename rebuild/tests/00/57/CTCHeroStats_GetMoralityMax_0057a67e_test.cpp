// Behaviour test for 0057a67e CTCHeroStats::GetMoralityMax: the row body verbatim over a fixture model of the shared-header interfaces it uses
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
    long GetMoralityMax() const;
};
// ---- row body (verbatim) ----
long CTCHeroStats::GetMoralityMax() const
{
    return GetDefinitionManager().PeekHeroStatsDef().MaxMorality;
}
// ---- harness ----
int main()
{
    CHeroStatsDef def; def.NumberOfSecondsAfterChangeHealthStartsRefilling = 2.5f; def.PintsToDrunk = 3.0f; def.PintsToVomit = 6.5f;
    def.MaxMorality = 1000;
    CGameDefinitionManager manager; manager.PHeroStatsDef = &def;
    CTCHeroStats stats; stats.Manager = &manager; stats.Drunkenness = 0.0f; stats.FramesUntilHealthRefills = -1;
    CHECK(stats.GetMoralityMax() == 1000);
    def.MaxMorality = -5; CHECK(stats.GetMoralityMax() == -5 && g_managerCalls == 2);
    if (fails == 0) printf("0057a67e_TEST PASS\n");
    return fails;
}
