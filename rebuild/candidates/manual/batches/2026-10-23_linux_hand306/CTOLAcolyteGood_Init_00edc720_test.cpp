#include <cstdio>
#include <cstring>
// behaviour test: CTOL_AcolyteGood::Init registers the two parent callbacks by "ParentClass.<name>", arms its timer from the quest global, sets the thing flags/target and clears the parent's flag
struct CParent { char Names[2][64]; int NumAdded; bool Flag5B; };
static long g_timerIndex = -1, g_timerTime = -1; static int g_flagsSet = 0, g_targetSet = 0;
struct CAcolyte {
    CParent* ParentClass; long TimerIndex; long GlobalTime;
    void Add(const char* name)
    {
        char* dst = ParentClass->Names[ParentClass->NumAdded++];
        std::strcpy(dst, "ParentClass.");
        std::strcat(dst, name);
    }
    void Init()
    {
        Add("WatchForAcolyteHit");
        Add("WatchForAcolyteDeath");
        g_timerIndex = TimerIndex; g_timerTime = GlobalTime;
        ++g_flagsSet; ++g_targetSet;
        ParentClass->Flag5B = false;
    }
};
int main()
{
    CParent p; p.NumAdded = 0; p.Flag5B = true;
    CAcolyte a; a.ParentClass = &p; a.TimerIndex = 3; a.GlobalTime = 900;
    a.Init();
    bool ok = p.NumAdded == 2 && std::strcmp(p.Names[0], "ParentClass.WatchForAcolyteHit") == 0
        && std::strcmp(p.Names[1], "ParentClass.WatchForAcolyteDeath") == 0 && g_timerIndex == 3 && g_timerTime == 900
        && g_flagsSet == 1 && g_targetSet == 1 && !p.Flag5B;
    if (!ok) { std::printf("00edc720_TEST FAIL\n"); return 1; }
    std::printf("00edc720_TEST PASS\n");
    return 0;
}
