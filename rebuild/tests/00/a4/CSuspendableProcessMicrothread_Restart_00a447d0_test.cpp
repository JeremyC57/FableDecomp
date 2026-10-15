// Behaviour test for CSuspendableProcessMicrothread::Restart (0x00A447D0): old microthread deleted, new one created for this,
// StopRunning cleared, a throw when creation fails.
#include <cstdio>
struct Thread { int id; };
static int g_deleted; static Thread g_thread; static bool g_fail; static void* g_param;
static Thread* CreateMicroThread(void (*)(void*), void* param) { g_param = param; return g_fail ? 0 : &g_thread; }
static void DeleteMicroThread(Thread*) { ++g_deleted; }
static void Entry(void*) {}
struct FibreException { FibreException(const char*) {} };
struct Process {
    bool Running, StopRunning; Thread* PMicrothread;
    void Restart()
    {
        if (PMicrothread)
            DeleteMicroThread(PMicrothread);
        PMicrothread = CreateMicroThread(Entry, this);
        StopRunning = false;
        if (PMicrothread == 0)
            throw FibreException("Failed to create fibre");
    }
};
int main()
{
    Process p; p.StopRunning = true; p.PMicrothread = 0;
    p.Restart();
    if (g_deleted != 0 || p.PMicrothread != &g_thread || p.StopRunning || g_param != &p) { std::printf("00a447d0_TEST FAIL\n"); return 1; }
    p.StopRunning = true;
    p.Restart();
    if (g_deleted != 1 || p.StopRunning) { std::printf("00a447d0_TEST FAIL\n"); return 1; }
    g_fail = true; bool threw = false;
    try { p.Restart(); } catch (FibreException&) { threw = true; }
    if (!threw || g_deleted != 2 || p.PMicrothread != 0) { std::printf("00a447d0_TEST FAIL\n"); return 1; }
    std::printf("00a447d0_TEST PASS\n");
    return 0;
}