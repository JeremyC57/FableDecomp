// Behaviour test for 0x00c90cb0: InitializeDrawing: state reset, backend hook result, info block fill, busy error.
// The reconstructed logic runs over a host model of the retail classes and callees.
#include <cstdio>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d\n", __LINE__); } } while (0)
struct P; typedef long (*Hook)(P*);
struct P { unsigned int state[4]; int resets; unsigned int w, h; unsigned int frame[5]; bool busy; };
static long g_hookResult = 0; static long HookImpl(P*) { return g_hookResult; } static Hook g_hook = HookImpl;
struct Info { unsigned int w, h, flags, frame[5]; };
static long InitDraw(P* p, Info* info) { for (int i = 0; i < 4; ++i) p->state[i] = 0; ++p->resets; long r = g_hook(p); if (r != 0) return r;
  if (info) { if (p->busy) return -131; info->w = p->w; info->h = p->h; info->flags = 0; for (int i = 0; i < 5; ++i) info->frame[i] = p->frame[i]; } return 0; }
static int run() { P p; p.state[0] = 9; p.resets = 0; p.w = 640; p.h = 480; for (int i = 0; i < 5; ++i) p.frame[i] = i + 1; p.busy = false; Info inf;
  CHECK(InitDraw(&p, &inf) == 0 && inf.w == 640 && inf.h == 480 && inf.flags == 0 && inf.frame[4] == 5 && p.state[0] == 0);
  p.busy = true; CHECK(InitDraw(&p, &inf) == -131); CHECK(InitDraw(&p, 0) == 0);
  g_hookResult = 7; CHECK(InitDraw(&p, &inf) == 7 && p.resets == 4); return 0; }
int main()
{
    run();
    if (g_fail) { std::printf("00c90cb0_TEST FAIL\n"); return 1; }
    std::printf("00c90cb0_TEST PASS\n");
    return 0;
}
