#include <cstdio>
#include <cmath>
// Behaviour test: CTCOracleMinigame::ProcessInput -- accepts an input only while waiting for input (stage 4) with fewer than Level symbols fired+pending, queuing it and pushing its symbol to the HUD.
#define CHECK(c) if (!(c)) { std::printf("007e5f70_TEST FAIL\n"); return 1; }
static const char* g_names[4] = { "UP", "DOWN", "LEFT", "RIGHT" };
static const char* g_lastSymbol = 0;
static int g_pushes = 0;
static void OracleSymbolPushBack(const char* s) { g_lastSymbol = s; ++g_pushes; }
class CTCOracleMinigame { public:
    long Level; int Stage; long NumOutputsFired;
    int Pending[16]; unsigned int PendingCount;
    bool ProcessInput(int input)
    {
        if (NumOutputsFired + PendingCount < (unsigned int)Level && Stage == 4) {
            Pending[PendingCount++] = input;
            OracleSymbolPushBack(g_names[input]);
            return true;
        }
        return false;
    }
};
int main()
{
    CTCOracleMinigame g; g.Level = 3; g.Stage = 4; g.NumOutputsFired = 1; g.PendingCount = 0;
    CHECK(g.ProcessInput(2) && g.PendingCount == 1 && g.Pending[0] == 2 && g_lastSymbol == g_names[2])
    CHECK(g.ProcessInput(0) && g.PendingCount == 2 && g_lastSymbol == g_names[0])
    CHECK(!g.ProcessInput(1) && g.PendingCount == 2 && g_pushes == 2)
    g.PendingCount = 0; g.Stage = 3;
    CHECK(!g.ProcessInput(1) && g.PendingCount == 0)
    std::printf("007e5f70_TEST PASS\n");
    return 0;
}