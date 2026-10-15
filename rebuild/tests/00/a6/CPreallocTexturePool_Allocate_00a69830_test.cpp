// Behaviour test for 0x00a69830: CPreallocTexturePool::Allocate: reuse the LRU texture once released.
// The reconstructed logic runs over a host model of the retail classes and callees.
#include <cstdio>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d\n", __LINE__); } } while (0)
struct Tex; struct Owner { Tex* t; void Release(); };
struct Tex { bool owned, releasing, pending; bool stillLoading; Owner* owner; int mru; };
void Owner::Release() { t->owned = false; }
static bool g_steal = false;
struct Pool { Tex* head; unsigned long Count;
  Tex* Allocate() { for (unsigned long i = 0; i < Count; ++i) { Tex* t = head; if (t) { ++t->mru;
        if (t->pending) { t->pending = t->stillLoading; if (t->pending && !g_steal) continue; }
        if (!t->releasing) { t->releasing = true; if (t->owned) t->owner->Release(); t->releasing = false; if (!t->owned) return t; } } } return 0; } };
static int run() { Owner o; Tex t = {true, false, false, false, &o, 0}; o.t = &t; Pool p = {&t, 3};
  CHECK(p.Allocate() == &t && !t.owned && t.mru == 1);
  Tex u = {false, false, true, true, &o, 0}; Pool q = {&u, 2}; CHECK(q.Allocate() == 0 && u.mru == 2);
  g_steal = true; u.pending = true; CHECK(q.Allocate() == &u);
  Pool e = {&t, 0}; CHECK(e.Allocate() == 0); return 0; }
int main()
{
    run();
    if (g_fail) { std::printf("00a69830_TEST FAIL\n"); return 1; }
    std::printf("00a69830_TEST PASS\n");
    return 0;
}