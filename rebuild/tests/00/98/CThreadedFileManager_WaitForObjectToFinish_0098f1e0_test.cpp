// Behaviour test for CThreadedFileManager::WaitForObjectToFinish (0098f1e0): lock balance and the wait loop.
#include <cstdio>
static int g_lock = 0, g_maxLock = 0, g_events = 0, g_sleeps = 0, g_prio = -1;
static long g_status = 0; static bool g_present = true; static int g_finishAfter = 3;
static void Enter() { ++g_lock; if (g_lock > g_maxLock) g_maxLock = g_lock; }
static void Leave() { --g_lock; }
static void Sleep() { ++g_sleeps; if (g_sleeps >= g_finishAfter) g_status = 4; }
struct Manager {
    bool IsObjectUnfinished(unsigned long) { return g_present && g_status != 4; }
    void SetObjectPriority(unsigned long, long p) { g_prio = (int)p; }
    void WaitForObjectToFinish(unsigned long uid)
    {
        Enter();
        if (IsObjectUnfinished(uid)) {
            SetObjectPriority(uid, 6);
            while (IsObjectUnfinished(uid)) { Leave(); ++g_events; Sleep(); Enter(); }
            Leave();
            return;
        }
        Leave();
    }
};
int main()
{
    Manager m;
    m.WaitForObjectToFinish(5);
    bool ok = g_lock == 0 && g_maxLock == 1 && g_prio == 6 && g_events == 3 && g_sleeps == 3;
    g_prio = -1; g_events = 0; m.WaitForObjectToFinish(5);       // already finished
    ok = ok && g_lock == 0 && g_prio == -1 && g_events == 0;
    g_present = false; g_status = 0; m.WaitForObjectToFinish(5);  // unknown object
    ok = ok && g_lock == 0 && g_prio == -1;
    if (!ok) { std::printf("0098f1e0_TEST FAIL\n"); return 1; }
    std::printf("0098f1e0_TEST PASS\n");
    return 0;
}