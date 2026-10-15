#include <cstdio>
static int g_log[32]; static int g_n = 0;
static void Log(int v) { g_log[g_n++] = v; }
struct Freed { int id; Freed(int i) : id(i) {} };
static void Free(void* p) { if (p) Log(((Freed*)p)->id); }

struct Vec { int id; Vec(int i) : id(i) {} ~Vec() { Log(id); } };
struct Base { ~Base() { Log(-2); } };
struct DI : Base { Vec verts, tris, groups; float sphere[4]; Vec ut, vt; DI() : verts(4), tris(0x10), groups(0x1c), ut(0x38), vt(0x44) {} };
static int Expect(const int* w, int n) { if (g_n != n) return 0; for (int i = 0; i < n; ++i) if (g_log[i] != w[i]) return 0; return 1; }
int main()
{

    { DI d; }
    int w[] = { 0x44, 0x38, 0x1c, 0x10, 4, -2 };
    if (!Expect(w, 6)) { std::printf("00a98470_TEST FAIL\n"); return 1; }
    std::printf("00a98470_TEST PASS\n");
    return 0;
}
