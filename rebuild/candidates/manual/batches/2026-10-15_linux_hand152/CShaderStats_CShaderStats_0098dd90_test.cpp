#include <cstdio>
static int g_log[32]; static int g_n = 0;
static void Log(int v) { g_log[g_n++] = v; }
struct Freed { int id; Freed(int i) : id(i) {} };
static void Free(void* p) { if (p) Log(((Freed*)p)->id); }

struct Str { int id; Str(int i) : id(i) {} ~Str() { Log(id); } };
struct Map { int id; unsigned int size; void* head; Map(int i) : id(i), size(1), head(0) {} ~Map() { if (size) Log(id); Free(head); } };
struct Base { ~Base() { Log(-2); } };
struct Stats : Base { Str px; Map pxc; Str vx; Map vxc; Stats() : px(4), pxc(8), vx(0x14), vxc(0x18) {} };
static int Expect(const int* w, int n) { if (g_n != n) return 0; for (int i = 0; i < n; ++i) if (g_log[i] != w[i]) return 0; return 1; }
int main()
{

    { Stats s; }
    int w[] = { 0x18, 0x14, 8, 4, -2 };
    if (!Expect(w, 5)) { std::printf("0098dd90_TEST FAIL\n"); return 1; }
    std::printf("0098dd90_TEST PASS\n");
    return 0;
}
