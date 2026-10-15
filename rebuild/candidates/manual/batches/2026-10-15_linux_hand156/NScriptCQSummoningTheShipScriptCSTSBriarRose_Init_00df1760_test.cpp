// Behaviour test for 0x00DF1760: CSTS_BriarRose::Init: brain state 1 and the ten script-interface calls in order with their flags
#include <cstdio>
#include <cstring>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %%d\n", __LINE__); } } while (0)
struct Rec { int slot; int a, b, c; };
struct GSI {
    Rec log[16]; int n; int hero;
    GSI() : n(0), hero(77) {}
    void Put(int s, int a, int b, int c) { log[n].slot = s; log[n].a = a; log[n].b = b; log[n].c = c; ++n; }
    const int* GetHero() { Put(70, 0, 0, 0); return &hero; }
};
struct Script { GSI* game; int me; long brainState; };
static void Init(Script& s)
{
    GSI* g = s.game;
    s.brainState = 1;
    g->Put(237, 0, 0, 0);
    g->Put(477, 1, 0, 0);
    g->Put(235, 1, 0, 0);
    g->Put(360, 0, 1, 0);
    g->Put(506, 0, 0, 0);
    const int* hero = g->GetHero();
    g->Put(599, *hero, 0, 0);
    g->Put(517, 0, 0, 0);
    g->Put(516, 0, 0, 0);
    g->Put(526, 0, 0, 0);
}
static void Run()
{
    GSI g; Script s; s.game = &g; s.me = 5; s.brainState = 0;
    Init(s);
    static const int order[11] = { 237, 477, 235, 360, 506, 70, 599, 517, 516, 526, 0 };
    CHECK(s.brainState == 1 && g.n == 10);
    for (int i = 0; i < 10; ++i) CHECK(g.log[i].slot == order[i]);
    CHECK(g.log[1].a == 1 && g.log[3].b == 1 && g.log[6].a == 77);
}

int main()
{
    Run();
    if (g_fail) { std::printf("00df1760_TEST FAIL\n"); return 1; }
    std::printf("00df1760_TEST PASS\n");
    return 0;
}
