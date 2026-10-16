// Behaviour test for 0x00caf500: CSyncQueue_00caf500 ctor: section initialised, two empty lists (cache 10), manual-reset event, seven cleared words
// The reconstructed logic runs over a host model of the retail classes and callees.
#include <cstdio>
#include <cstring>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d\n", __LINE__); } } while (0)

static int g_sections = 0; static int g_manual = -1;
struct CritSec { int inited; CritSec() : inited(++g_sections) {} };
struct List { void* first; void* last; long count; long cache; long used; void* head; List(const unsigned short*) : first(0), last(0), count(0), cache(10), used(0), head(0) {} };
struct Event { int manual; Event(int m) : manual(m) { g_manual = m; } };
struct Q { long a, b; CritSec lock; List l1, l2; Event ev; long w[7]; Q() : l1(0), l2(0), ev(1) { for (int i = 0; i < 7; ++i) w[i] = 0; } };
static void run() { Q q; CHECK(q.lock.inited == 1 && q.l1.cache == 10 && q.l2.count == 0 && !q.l2.head && q.ev.manual == 1 && g_manual == 1); for (int i = 0; i < 7; ++i) CHECK(q.w[i] == 0); }

int main()
{
    run();
    if (g_fail) { std::printf("00caf500_TEST FAIL\n"); return 1; }
    std::printf("00caf500_TEST PASS\n");
    return 0;
}