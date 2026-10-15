// Behaviour test for 0x008198c0: swap of two 24-byte keyed records.
// The reconstructed logic runs over a host model of the retail classes and callees.
#include <cstdio>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d\n", __LINE__); } } while (0)
struct V { unsigned int w[5]; }; struct R { unsigned int key; V v; };
static void Swap(R& a, R& b) { R t = a; a = b; b = t; }
static int run() { R a, b; a.key = 1; b.key = 2; for (int i = 0; i < 5; ++i) { a.v.w[i] = 10 + i; b.v.w[i] = 20 + i; }
  Swap(a, b); CHECK(a.key == 2 && b.key == 1 && a.v.w[0] == 20 && a.v.w[4] == 24 && b.v.w[0] == 10 && b.v.w[4] == 14); return 0; }
int main()
{
    run();
    if (g_fail) { std::printf("008198c0_TEST FAIL\n"); return 1; }
    std::printf("008198c0_TEST PASS\n");
    return 0;
}