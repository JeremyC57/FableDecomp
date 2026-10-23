#include <cstdio>
static void* g_o = 0; static void* g_a = 0; static void* g_b = 0; struct F; struct O { unsigned char b[0x24]; O(F* o, void* b, void* a) { g_o = o; g_b = b; g_a = a; } }; struct F { O* C(void* a, void* b) { return new O(this, b, a); } };
static bool Check() { F f; int i, j; O* o = f.C(&i, &j); bool ok = o && g_o == &f && g_a == &i && g_b == &j; delete o; return ok; }

int main()
{
    if (!Check()) { std::printf("00c0e700_TEST FAIL\n"); return 1; }
    std::printf("01210ff2_TEST PASS\n");
    return 0;
}