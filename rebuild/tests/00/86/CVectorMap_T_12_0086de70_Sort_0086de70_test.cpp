#include <cstdio>
static int g_sorted = 0;
static void __fastcall SortRange(void* b, void* e, bool c) { ++g_sorted; }
struct CVectorMap_T_12_0086de70 { void* first; void* last; char pad[4]; bool compare; unsigned char dirty; void Sort() { SortRange(first, last, compare); dirty = 0; } };
int main() { CVectorMap_T_12_0086de70 m; m.dirty = 1; m.Sort(); bool ok = g_sorted == 1 && m.dirty == 0; std::printf(ok ? "0086de70_TEST PASS\n" : "0086de70_TEST FAIL\n"); return ok ? 0 : 1; }
