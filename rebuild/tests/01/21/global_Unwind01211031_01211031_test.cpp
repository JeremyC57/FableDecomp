#include <cstdio>
static void* g_a = 0; static void* g_b = 0; struct O { unsigned char b[0x1c]; O(void* a, void* b) { g_a = a; g_b = b; } }; struct F { O* C(void* a, void* b) { return new O(a, b); } };
static bool Check() { F f; int i, j; O* o = f.C(&i, &j); bool ok = o && g_a == &i && g_b == &j; delete o; return ok; }

int main()
{
    if (!Check()) { std::printf("00c0e870_TEST FAIL\n"); return 1; }
    std::printf("01211031_TEST PASS\n");
    return 0;
}