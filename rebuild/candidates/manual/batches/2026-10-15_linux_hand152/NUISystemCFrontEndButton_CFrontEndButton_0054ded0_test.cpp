#include <cstdio>
static int g_log[32]; static int g_n = 0;
static void Log(int v) { g_log[g_n++] = v; }
struct Freed { int id; Freed(int i) : id(i) {} };
static void Free(void* p) { if (p) Log(((Freed*)p)->id); }

struct Vec { int id; void* p; Vec(int i) : id(i), p(0) {} ~Vec() { Log(id); Free(p); } };
struct List { ~List() { Log(0x1b0); } };
struct Base { ~Base() { Log(-2); } };
struct Button : Base { float t; Vec Actions, OnDelete; List Params; Button() : Actions(0x198), OnDelete(0x1a4) {} };
static int Expect(const int* w, int n) { if (g_n != n) return 0; for (int i = 0; i < n; ++i) if (g_log[i] != w[i]) return 0; return 1; }
int main()
{

    static Freed f(7);
    { Button b; b.OnDelete.p = &f; }
    int w[] = { 0x1b0, 0x1a4, 7, 0x198, -2 };
    if (!Expect(w, 5)) { std::printf("0054ded0_TEST FAIL\n"); return 1; }
    std::printf("0054ded0_TEST PASS\n");
    return 0;
}
