#include <cstdio>
static int g_log[32]; static int g_n = 0;
static void Log(int v) { g_log[g_n++] = v; }
struct Freed { int id; Freed(int i) : id(i) {} };
static void Free(void* p) { if (p) Log(((Freed*)p)->id); }

struct Str { int id; Str() : id(0) {} ~Str() { Log(id); } };
struct Gen { char pad[0x34]; Str name; };
struct Pool { void Free(long h) { Log(1000 + (int)h); } };
inline void DestroyGenerators(Gen* f, Gen* l) { for (; f != l; ++f) f->~Gen(); }
struct Stats {
    Pool* pool; long handle; Str name; Gen* first; Gen* last;
    ~Stats() { if (handle != -1) pool->Free(handle); DestroyGenerators(first, last); Log(-3); }
};
static int Expect(const int* w, int n) { if (g_n != n) return 0; for (int i = 0; i < n; ++i) if (g_log[i] != w[i]) return 0; return 1; }
int main()
{

    Pool pool; static Gen g[2]; g[0].name.id = 50; g[1].name.id = 51;
    { Stats s; s.pool = &pool; s.handle = 5; s.name.id = 12; s.first = g; s.last = g + 2; }
    { Stats s; s.pool = &pool; s.handle = -1; s.name.id = 13; s.first = g; s.last = g; }
    int w[] = { 1005, 50, 51, -3, 12, -3, 13 };
    if (!Expect(w, 7)) { std::printf("00a2f190_TEST FAIL\n"); return 1; }
    std::printf("00a2f190_TEST PASS\n");
    return 0;
}
