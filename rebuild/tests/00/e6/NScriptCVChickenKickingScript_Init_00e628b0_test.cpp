#include <cstdio>
#include <cstring>
// Behaviour test: a copy of the source's logic over a mock model (calls are recorded, not executed).
static char g_log[2048];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
static const char* g_str = "";
struct Script {
    bool ChickenLanded;
    int FinalSector;
    int DistanceBand;
    bool KnowGhostHasGone;
    bool SpectatorsCreated;
    bool RanOff;
    bool TalkedTo;
    bool SpectatorsUnderAttack;
    unsigned int PrizesWon;
    bool InfoDisplayed;
    int Tail;

    void Init()
    {
        ChickenLanded = false; FinalSector = 0; DistanceBand = 0; KnowGhostHasGone = false; SpectatorsCreated = false;
        RanOff = false; TalkedTo = false; SpectatorsUnderAttack = false; PrizesWon = 0; InfoDisplayed = false;
    }
};

int main()
{
    g_log[0] = 0;
    bool ok = true;
    Script s;
    std::memset(&s, 0x5a, sizeof(s));
    s.Tail = 77;
    s.Init();
    ok = (!s.ChickenLanded && s.FinalSector == 0 && s.DistanceBand == 0 && !s.KnowGhostHasGone && !s.SpectatorsCreated && !s.RanOff && !s.TalkedTo && !s.SpectatorsUnderAttack && s.PrizesWon == 0 && !s.InfoDisplayed) && s.Tail == 77;

    if (!ok) { std::printf("00e628b0_TEST FAIL\n"); return 1; }
    std::printf("00e628b0_TEST PASS\n");
    return 0;
}