// Behaviour test for 0x00C3C3E0: descriptor copy, allocate-on-null, -5 on failed allocation, state bits 0/1/7.
#include <cstdio>
static int g_allocs; static bool g_fail; static char g_mem[64];
void* AllocBuffer(unsigned int) { ++g_allocs; return g_fail ? 0 : g_mem; }
struct Desc { unsigned int format; int a, b; unsigned int size; int c, d; void* buf; };
struct Obj { Desc desc; unsigned int state;
  int SetDesc(const Desc& d) { desc = d; if (!desc.buf) { desc.buf = AllocBuffer(desc.size); if (!desc.buf) return -5; state |= 2; }
    if (desc.format == 0x80000000u) state |= 0x80; state |= 1; return 0; } };
int main()
{
    Obj o; o.state = 0; Desc d = {0x80000000u, 1, 2, 16, 0, 0, 0};
    bool ok = o.SetDesc(d) == 0 && o.state == 0x83 && o.desc.buf == g_mem && g_allocs == 1;
    char own[4]; Desc e = {5, 0, 0, 4, 0, 0, own}; o.state = 0;
    ok = ok && o.SetDesc(e) == 0 && o.state == 1 && o.desc.buf == own && g_allocs == 1;
    g_fail = true; o.state = 0; ok = ok && o.SetDesc(d) == -5 && o.state == 0;
    if (!ok) { std::printf("00c3c3e0_TEST FAIL\n"); return 1; }
    std::printf("00c3c3e0_TEST PASS\n");
    return 0;
}