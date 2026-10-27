#include <cstdio>
#include <cstring>
// Behaviour test: a copy of the source's logic over a mock model (calls are recorded, not executed).
static char g_log[2048];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
static const char* g_str = "";
struct Script {
    float CurrentPopulationScale[4];
    bool HighDetailSimulationActive;
    int CurrentPopulationRegion;
    int CreaturesAlive[3];
    unsigned int BanditCreatureGeneratorIndex;
    unsigned int VillagerCreatureGeneratorIndex;
    unsigned int GuardCreatureGeneratorIndex;
    int Tail;

    void Init()
    {
        for (int i = 0; i < 4; ++i)
            CurrentPopulationScale[i] = 0.0f;
        for (int j = 0; j < 3; ++j)
            CreaturesAlive[j] = 0;
        HighDetailSimulationActive = false; CurrentPopulationRegion = 1;
        BanditCreatureGeneratorIndex = 0; VillagerCreatureGeneratorIndex = 0; GuardCreatureGeneratorIndex = 0;
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
    ok = (s.CurrentPopulationScale[0] == 0.0f && s.CurrentPopulationScale[3] == 0.0f && s.CreaturesAlive[0] == 0 && s.CreaturesAlive[2] == 0 && !s.HighDetailSimulationActive && s.CurrentPopulationRegion == 1 && s.BanditCreatureGeneratorIndex == 0 && s.VillagerCreatureGeneratorIndex == 0 && s.GuardCreatureGeneratorIndex == 0) && s.Tail == 77;

    if (!ok) { std::printf("00ec1810_TEST FAIL\n"); return 1; }
    std::printf("00ec1810_TEST PASS\n");
    return 0;
}