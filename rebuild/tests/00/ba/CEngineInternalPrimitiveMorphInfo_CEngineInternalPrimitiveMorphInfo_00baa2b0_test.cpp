#include <cstdio>
static int g_log[32]; static int g_n = 0;
static void Log(int v) { g_log[g_n++] = v; }
struct Freed { int id; Freed(int i) : id(i) {} };
static void Free(void* p) { if (p) Log(((Freed*)p)->id); }

struct DecalVec { ~DecalVec() { Log(0); } };
struct CompVec { void* p; CompVec() : p(0) {} ~CompVec() { Log(0x10); Free(p); } };
struct Set { unsigned int size; void* head; Set() : size(0), head(0) {} ~Set() { if (size) { Log(0x20); size = 0; } Free(head); } };
struct Bytes { void* p; Bytes() : p(0) {} ~Bytes() { Free(p); } };
struct Morph { DecalVec decals; long c1; CompVec comps; long c2; Set names; Bytes hidden; };
static int Expect(const int* w, int n) { if (g_n != n) return 0; for (int i = 0; i < n; ++i) if (g_log[i] != w[i]) return 0; return 1; }
int main()
{

    static Freed a(0x2c), h(0x24);
    { Morph m; m.hidden.p = &a; m.names.size = 2; m.names.head = &h; }
    int w[] = { 0x2c, 0x20, 0x24, 0x10, 0 };
    if (!Expect(w, 5)) { std::printf("00baa2b0_TEST FAIL\n"); return 1; }
    std::printf("00baa2b0_TEST PASS\n");
    return 0;
}