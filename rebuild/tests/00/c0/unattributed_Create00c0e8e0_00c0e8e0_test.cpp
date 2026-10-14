#include <cstdio>
static void* g_o = 0; static void* g_a = 0; struct F; struct O { unsigned char b[0x20]; O(F* o, void* a) { g_o = o; g_a = a; } }; struct F { O* C(void* a) { return new O(this, a); } };
static bool Check() { F f; int i; O* o = f.C(&i); bool ok = o && g_o == &f && g_a == &i; delete o; return ok; }

int main()
{
    if (!Check()) { std::printf("00c0e8e0_TEST FAIL\n"); return 1; }
    std::printf("00c0e8e0_TEST PASS\n");
    return 0;
}