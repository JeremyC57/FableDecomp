#include <cstdio>
#include <new>
static int g_l = 0; struct G {}; static G g_g; struct L { void* t; long s; L(void* x) : t(x), s(0) { ++g_l; } ~L() { --g_l; } };
static int g_fin = 0, g_sd = 0; static unsigned long g_tick = 0; static unsigned long Tick() { return g_tick; } struct Tg { void Finish() { ++g_fin; } };
struct X { unsigned char p[0x60]; Tg* tg; unsigned char q[0xc]; float rate; long armed; unsigned long dl; void Shutdown() { ++g_sd; } void Close() { L l(&g_g); if (tg) tg->Finish(); if (!armed) return; unsigned long now = Tick(); if (now > dl) { Shutdown(); dl = now - (long)(-1000.0f / rate); } } };
static bool Check() { Tg t; X* x = new X; x->tg = 0; x->armed = 0; x->rate = 10.0f; x->dl = 100; g_tick = 500; x->Close(); if (g_fin || g_sd) return false; x->tg = &t; x->Close(); if (g_fin != 1 || g_sd) return false; x->armed = 1; g_tick = 50; x->Close(); if (g_sd) return false; g_tick = 500; x->Close(); bool ok = g_fin == 3 && g_sd == 1 && x->dl == 600 && g_l == 0; delete x; return ok; }

int main()
{
    if (!Check()) { std::printf("00c092d0_TEST FAIL\n"); return 1; }
    std::printf("00c092d0_TEST PASS\n");
    return 0;
}
