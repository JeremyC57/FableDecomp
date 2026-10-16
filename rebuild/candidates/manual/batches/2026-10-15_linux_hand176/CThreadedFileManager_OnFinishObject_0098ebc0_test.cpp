// Behaviour test for 0x0098ebc0: CThreadedFileManager::OnFinishObject: callback told outside the lock, status 4, current cleared, non-queued objects erased and freed
// The reconstructed logic runs over a host model of the retail classes and callees.
#include <cstdio>
#include <cstring>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d\n", __LINE__); } } while (0)
static int g_lock = 0, g_enter = 0, g_leave = 0, g_set = 0, g_sleep = 0, g_close = 0, g_del = 0, g_free = 0, g_deleted = 0;
static void Enter() { ++g_lock; ++g_enter; } static void Leave() { --g_lock; ++g_leave; }
struct Obj { void* cb; bool queue; long prio; unsigned long uid; long status; int overlapped; };
static Obj* g_map[4]; static int g_pending_removed = 0;
static int Find(unsigned long uid) { for (int i = 0; i < 4; ++i) if (g_map[i] && g_map[i]->uid == uid) return i; return -1; }

static int g_cb = 0; static int g_cb_lock = -1; static Obj* g_current = 0;
static void OnFinishObject(Obj* o) { if (o->cb) { Leave(); ++g_cb; g_cb_lock = g_lock; Enter(); o->cb = 0; } o->status = 4; g_current = 0; if (!o->queue) { int it = Find(o->uid); if (it != -1) g_map[it] = 0; if (o->overlapped) ++g_free; delete o; ++g_deleted; } }
static void run() { int dummy; Obj* a = new Obj; a->cb = &dummy; a->queue = true; a->uid = 7; a->overlapped = 0; g_map[2] = a; g_current = a; Enter(); OnFinishObject(a); Leave();
  CHECK(g_cb == 1 && g_cb_lock == 0 && a->cb == 0 && a->status == 4 && g_current == 0 && g_map[2] == a);
  Obj* b = new Obj; b->cb = 0; b->queue = false; b->uid = 8; b->overlapped = 1; g_map[3] = b; OnFinishObject(b); CHECK(g_map[3] == 0 && g_free == 1 && g_deleted == 1); delete a; }

int main()
{
    run();
    if (g_fail) { std::printf("0098ebc0_TEST FAIL\n"); return 1; }
    std::printf("0098ebc0_TEST PASS\n");
    return 0;
}
