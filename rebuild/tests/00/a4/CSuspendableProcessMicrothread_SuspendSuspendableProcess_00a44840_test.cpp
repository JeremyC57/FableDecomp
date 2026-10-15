#include <cstdio>
static int g_yields = 0;
static bool* g_stopOnYield = 0;
static void YieldMicrothread() { ++g_yields; if (g_stopOnYield) *g_stopOnYield = true; }
class CASuspendableProcess {
public:
    bool Running, StopRunning;
    bool CheckTermination() const { return StopRunning != 0; }
};
class CSuspendableProcessMicrothread : public CASuspendableProcess {
public:
    bool SuspendSuspendableProcess()
    {
        if (CheckTermination())
            return false;
        YieldMicrothread();
        if (CheckTermination())
            return false;
        return true;
    }
};
int main()
{
    CSuspendableProcessMicrothread p; p.Running = true; p.StopRunning = false;
    if (!p.SuspendSuspendableProcess() || g_yields != 1) { std::printf("00a44840_TEST FAIL\n"); return 1; }
    g_stopOnYield = &p.StopRunning;
    if (p.SuspendSuspendableProcess() || g_yields != 2) { std::printf("00a44840_TEST FAIL\n"); return 1; }
    if (p.SuspendSuspendableProcess() || g_yields != 2) { std::printf("00a44840_TEST FAIL\n"); return 1; }
    std::printf("00a44840_TEST PASS\n");
    return 0;
}