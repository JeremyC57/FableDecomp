// Behaviour test for 0x00B59B60: defaults and fade table of the local-detail generator.
#include <cstdio>
struct Entry { float Min, Factor; };
struct Gen { void* a; void* b; void* c; bool zs; int stipple, batch; float maxUpdate; bool enabled, dynamic; int block; Entry t[8];
  Gen(bool d) : a(0), b(0), c(0), dynamic(d) { zs = true; stipple = 4; batch = 0x20; maxUpdate = 0.002f; enabled = true; block = 0x8000;
    for (int i = 0; i < 8; ++i) { t[i].Min = 0.0f; t[i].Factor = 1.0f; } } };
int main()
{
    Gen g(true), h(false);
    bool ok = g.dynamic && !h.dynamic && g.zs && g.stipple == 4 && g.batch == 32 && g.block == 0x8000 && g.t[7].Factor == 1.0f && g.t[0].Min == 0.0f && !g.a;
    if (!ok) { std::printf("00b59b60_TEST FAIL\n"); return 1; }
    std::printf("00b59b60_TEST PASS\n");
    return 0;
}