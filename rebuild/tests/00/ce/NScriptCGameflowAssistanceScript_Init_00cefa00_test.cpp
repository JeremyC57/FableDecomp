#include <cstdio>
#include <cstring>
// Behaviour test: a copy of the source's logic over a mock model.
struct Script {
    int FlourishesOffered, FlourishesPerformed, CombatMultHighestLevel, CombatMultFramesSinceChange;
    int HealthWarningsGiven, WillEnergyWarningsGiven, SavedRenownLevel, SaveWarningsGiven;
    int After;
    void Init()
    {
        FlourishesOffered = 0;
        FlourishesPerformed = 0;
        CombatMultHighestLevel = 1;
        CombatMultFramesSinceChange = 0;
        HealthWarningsGiven = 0;
        WillEnergyWarningsGiven = 0;
        SaveWarningsGiven = 0;
        SavedRenownLevel = 0;
    }
};
int main()
{
    int ok = 1;
    Script s; std::memset(&s, 0x55, sizeof s);
    s.Init();
    ok = s.FlourishesOffered == 0 && s.FlourishesPerformed == 0 && s.CombatMultHighestLevel == 1 && s.CombatMultFramesSinceChange == 0
      && s.HealthWarningsGiven == 0 && s.WillEnergyWarningsGiven == 0 && s.SavedRenownLevel == 0 && s.SaveWarningsGiven == 0 && s.After == 0x55555555;
    if (!ok) { std::printf("00cefa00_TEST FAIL\n"); return 1; }
    std::printf("00cefa00_TEST PASS\n");
    return 0;
}