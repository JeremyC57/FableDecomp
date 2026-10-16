// Behaviour test for 0x0098e0d0: CThreadedFileManager::Uninitialise: signals the thread, waits for ThreadFinished, closes the event, deletes the lock
// The reconstructed logic runs over a host model of the retail classes and callees.
#include <cstdio>
#include <cstring>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d\n", __LINE__); } } while (0)
static int g_lock = 0, g_enter = 0, g_leave = 0, g_set = 0, g_sleep = 0, g_close = 0, g_del = 0, g_free = 0, g_deleted = 0;
static void Enter() { ++g_lock; ++g_enter; } static void Leave() { --g_lock; ++g_leave; }

struct M { bool Initialised, ExitThread, ThreadFinished; int Event;
  void SleepStub() { ++g_sleep; if (g_sleep == 3 && ExitThread) ThreadFinished = true; }
  void Uninitialise() { if (Initialised) { Initialised = false; Enter(); ExitThread = true; ++g_set; while (!ThreadFinished) { Leave(); SleepStub(); Enter(); } if (Event) { ++g_close; Event = 0; } Leave(); ++g_del; } } };
static void run() { M m; m.Initialised = true; m.ExitThread = false; m.ThreadFinished = false; m.Event = 5; m.Uninitialise(); CHECK(!m.Initialised && m.ExitThread && m.ThreadFinished && m.Event == 0 && g_sleep == 3 && g_lock == 0 && g_close == 1 && g_del == 1 && g_set == 1);
  M n; n.Initialised = false; n.Event = 4; n.Uninitialise(); CHECK(n.Event == 4 && g_del == 1); }

int main()
{
    run();
    if (g_fail) { std::printf("0098e0d0_TEST FAIL\n"); return 1; }
    std::printf("0098e0d0_TEST PASS\n");
    return 0;
}