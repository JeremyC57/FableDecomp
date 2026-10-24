#include <cstdio>
static int g_sorted = 0;
static void __fastcall SortRange(void* b, void* e, bool c) { ++g_sorted; }
struct CVectorMap_T_0047119a { void* first; void* last; char pad[4]; bool compare; unsigned char dirty; void Sort() { SortRange(first, last, compare); dirty = 0; } };
int main() { CVectorMap_T_0047119a m; m.dirty = 1; m.Sort(); bool ok = g_sorted == 1 && m.dirty == 0; std::printf(ok ? "0047119a_TEST PASS\n" : "0047119a_TEST FAIL\n"); return ok ? 0 : 1; }
