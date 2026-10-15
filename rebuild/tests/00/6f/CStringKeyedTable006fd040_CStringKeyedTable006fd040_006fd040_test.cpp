#include <cstdio>
static int g_log[32]; static int g_n = 0;
static void Log(int v) { g_log[g_n++] = v; }
struct Freed { int id; Freed(int i) : id(i) {} };
static void Free(void* p) { if (p) Log(((Freed*)p)->id); }

struct Tree { unsigned int size; void* head; Tree() : size(0), head(0) {} ~Tree() { if (size) { Log(0x99); size = 0; } Free(head); } };
struct Vec { void* p; Vec() : p(0) {} ~Vec() { Log(0x88); Free(p); } };
struct Base { ~Base() { Log(-2); } };
struct Table : Base { Vec entries; long cmp; Tree index; };
static int Expect(const int* w, int n) { if (g_n != n) return 0; for (int i = 0; i < n; ++i) if (g_log[i] != w[i]) return 0; return 1; }
int main()
{

    static Freed h(1), v(2);
    { Table t; t.index.size = 3; t.index.head = &h; t.entries.p = &v; }
    int w[] = { 0x99, 1, 0x88, 2, -2 };
    if (!Expect(w, 5)) { std::printf("006fd040_TEST FAIL\n"); return 1; }
    std::printf("006fd040_TEST PASS\n");
    return 0;
}