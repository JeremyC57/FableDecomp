// Behaviour test for 0x009C31E0 CFileInstaller::Pause: count bump, unlock + signal + wait only on the first pause of a running installer.
#include <cstdio>
static int g_locks = 0, g_unlocks = 0, g_sets = 0, g_waits = 0;
static bool g_installerNoPauseWait = false;
static void* g_lastSet = 0; static void* g_lastWait = 0;
static int SetEvent_(void* e) { ++g_sets; g_lastSet = e; return 1; }
static unsigned int WaitForSingleObjectEx_(void* h, unsigned int, int) { ++g_waits; g_lastWait = h; return 0; }
struct Lock {
    bool locked;
    Lock() : locked(true) { ++g_locks; }
    ~Lock() { if (locked) Unlock(); }
    void Unlock() { if (locked) { locked = false; ++g_unlocks; } }
};
struct CFileInstaller {
    bool Running; unsigned int Paused; void* Event; void* PauseStartEvent;
    void Pause()
    {
        Lock lock;
        ++Paused;
        if (Running && !(Paused > 1) && !g_installerNoPauseWait) {
            lock.Unlock();
            SetEvent_(Event);
            WaitForSingleObjectEx_(PauseStartEvent, 0xFFFFFFFF, 1);
        }
    }
};
static int Fail() { std::printf("009c31e0_TEST FAIL\n"); return 1; }
int main()
{
    int ev = 0, ps = 0;
    CFileInstaller f; f.Running = true; f.Paused = 0; f.Event = &ev; f.PauseStartEvent = &ps;
    f.Pause();
    if (f.Paused != 1 || g_sets != 1 || g_waits != 1 || g_lastSet != &ev || g_lastWait != &ps || g_unlocks != 1) return Fail();
    f.Pause();
    if (f.Paused != 2 || g_sets != 1 || g_waits != 1 || g_unlocks != 2) return Fail();
    f.Paused = 0; f.Running = false; f.Pause();
    if (f.Paused != 1 || g_sets != 1) return Fail();
    f.Paused = 0; f.Running = true; g_installerNoPauseWait = true; f.Pause();
    if (f.Paused != 1 || g_sets != 1 || g_locks != 4 || g_unlocks != 4) return Fail();
    std::printf("009c31e0_TEST PASS\n");
    return 0;
}
