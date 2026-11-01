// Behaviour test for CTCKraken::SetMode (0x007FC730): the mode is stored, then modes 1/2 create the tentacles when there are
// none and raise them and end the head effects, mode 3 kills the beam and the tentacles, modes 4/5 start the beam effect.
#include <cstdio>
#include <cstring>
static char g_log[64];
static void Log(char c) { size_t n = std::strlen(g_log); g_log[n] = c; g_log[n + 1] = 0; }
class CTCKraken { public:
    int Mode;
    int TentacleCount;
    void CreateTentacles() { Log('C'); TentacleCount = 3; }
    void RaiseSunkenTentacles() { Log('R'); }
    void EndHeadEffects() { Log('E'); }
    void KillBeamEffect() { Log('K'); }
    void KillTentacles() { Log('T'); TentacleCount = 0; }
    void StartBeamEffect() { Log('B'); }
    void InitTentacles() { if (TentacleCount == 0) CreateTentacles(); RaiseSunkenTentacles(); EndHeadEffects(); }
    void SetMode(int mode)
    {
        Mode = mode;
        switch (mode) {
        case 1: InitTentacles(); break;
        case 2: InitTentacles(); break;
        case 3: KillBeamEffect(); KillTentacles(); break;
        case 4:
        case 5: StartBeamEffect(); break;
        }
    }
};
static bool Check(CTCKraken& k, int mode, const char* want)
{
    g_log[0] = 0;
    k.SetMode(mode);
    return k.Mode == mode && std::strcmp(g_log, want) == 0;
}
int main()
{
    CTCKraken k;
    k.Mode = 0; k.TentacleCount = 0;
    if (!Check(k, 1, "CRE") || !Check(k, 2, "RE") || !Check(k, 4, "B") || !Check(k, 5, "B") || !Check(k, 3, "KT") ||
        !Check(k, 2, "CRE") || !Check(k, 0, "") || !Check(k, 6, "")) { std::printf("007fc730_TEST FAIL\n"); return 1; }
    std::printf("007fc730_TEST PASS\n");
    return 0;
}