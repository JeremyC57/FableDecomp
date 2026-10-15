#include <cstdio>
static int g_log[32]; static int g_n = 0;
static void Log(int v) { g_log[g_n++] = v; }
struct Freed { int id; Freed(int i) : id(i) {} };
static void Free(void* p) { if (p) Log(((Freed*)p)->id); }

struct Vec { void* p; Vec() : p(0) {} ~Vec() { Free(p); } };
struct Out { int id; Out(int i) : id(i) {} ~Out() { Log(id); } };
struct Def {
    Vec a, b, c; Out m4c; Out m58; float f1, f2; Out m6c, m78, m84, m90, m9c, ma8;
    Def() : m4c(0x4c), m58(0x58), m6c(0x6c), m78(0x78), m84(0x84), m90(0x90), m9c(0x9c), ma8(0xa8) {}
    ~Def() { Log(-1); }
};
static int Expect(const int* w, int n) { if (g_n != n) return 0; for (int i = 0; i < n; ++i) if (g_log[i] != w[i]) return 0; return 1; }
int main()
{

    static Freed fa(0x28), fc(0x40);
    { Def d; d.a.p = &fa; d.c.p = &fc; }
    int w[] = { -1, 0xa8, 0x9c, 0x90, 0x84, 0x78, 0x6c, 0x58, 0x4c, 0x40, 0x28 };
    if (!Expect(w, 11)) { std::printf("004ebb1c_TEST FAIL\n"); return 1; }
    std::printf("004ebb1c_TEST PASS\n");
    return 0;
}