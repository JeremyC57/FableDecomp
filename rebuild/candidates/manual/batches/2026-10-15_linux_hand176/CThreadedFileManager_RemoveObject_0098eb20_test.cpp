// Behaviour test for 0x0098eb20: CThreadedFileManager::RemoveObject: pending objects leave their queue, reading objects are kept (false), others freed and erased (true)
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

static bool RemoveObject(unsigned long uid) { Enter(); int it = Find(uid); if (it != -1) { Obj* o = g_map[it]; if (o->status == 0) { ++g_pending_removed; o->status = 4; } if (o->status == 1 || o->status == 2 || o->status == 3) { Leave(); return false; } if (o->overlapped) ++g_free; delete o; ++g_deleted; g_map[it] = 0; } Leave(); return true; }
static void run() { Obj* a = new Obj; a->uid = 1; a->status = 0; a->overlapped = 1; Obj* b = new Obj; b->uid = 2; b->status = 2; b->overlapped = 0; g_map[0] = a; g_map[1] = b;
  CHECK(RemoveObject(1) && g_pending_removed == 1 && g_free == 1 && g_map[0] == 0); CHECK(!RemoveObject(2) && g_map[1] == b); CHECK(RemoveObject(99)); CHECK(g_lock == 0); delete b; }

int main()
{
    run();
    if (g_fail) { std::printf("0098eb20_TEST FAIL\n"); return 1; }
    std::printf("0098eb20_TEST PASS\n");
    return 0;
}
