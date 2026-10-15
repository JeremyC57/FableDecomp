#include <cstdio>
static int g_log[32]; static int g_n = 0;
static void Log(int v) { g_log[g_n++] = v; }
struct Freed { int id; Freed(int i) : id(i) {} };
static void Free(void* p) { if (p) Log(((Freed*)p)->id); }

struct List { int id; void* node; List(int i) : id(i), node(0) {} void clear() { Log(id); } ~List() { clear(); Free(node); } };
struct B0 { ~B0() { Log(-2); } };
struct B1 { void* o; long x; ~B1() { Log(-1); } };
struct Mgr : B0, B1 { List full, always, paused; Mgr() : full(0x10), always(0x14), paused(0x18) {} };
static int Expect(const int* w, int n) { if (g_n != n) return 0; for (int i = 0; i < n; ++i) if (g_log[i] != w[i]) return 0; return 1; }
int main()
{

    static Freed n(77);
    { Mgr m; m.always.node = &n; }
    int w[] = { 0x18, 0x14, 77, 0x10, -1, -2 };
    if (!Expect(w, 6)) { std::printf("00b81cf0_TEST FAIL\n"); return 1; }
    std::printf("00b81cf0_TEST PASS\n");
    return 0;
}
