// Behaviour test for CAIStateGroup_LightHomeCandle::Update (0x00946850): model copy with scripted stubs.
#include <cstdio>

static int g_inUse, g_freePointOk, g_needs, g_moveR[8], g_lookR[8], g_mi, g_li, g_used, g_freed, g_term, g_freedIndex;
static bool g_candle;
struct Group {
    bool candleSet;
    bool Needs() { return g_needs > 0; }
    int Move() { int r = g_moveR[g_mi]; if (g_mi < 7) ++g_mi; return r; }
    int Look() { int r = g_lookR[g_li]; if (g_li < 7) ++g_li; return r; }
    void Free(int index) { ++g_freed; g_freedIndex = index; }
    void Terminate() { ++g_term; }
    void Update()
    {
        if (!candleSet) { Terminate(); return; }
        if (g_inUse != 0 || !Needs()) { Terminate(); return; }
        int index = 7;
        if (!g_freePointOk) { Terminate(); return; }
        int result;
        do {
            if (!candleSet || !Needs()) { Free(index); Terminate(); return; }
            result = Move();
        } while (result == 4);
        if (result != 2 || !candleSet || !Needs()) { Free(index); Terminate(); return; }
        do {
            if (!candleSet || !Needs()) { Free(index); Terminate(); return; }
            result = Look();
        } while (result == 4);
        if (result != 2 || !candleSet || !Needs()) { Free(index); return; }
        ++g_used;
        Free(index);
        candleSet = false;
        Terminate();
    }
};
static int fail(int n) { std::printf("00946850_TEST FAIL %d\n", n); return 1; }
static void reset(Group& g)
{
    g.candleSet = true; g_inUse = 0; g_freePointOk = 1; g_needs = 1; g_mi = g_li = g_used = g_freed = g_term = 0;
    for (int i = 0; i < 8; ++i) { g_moveR[i] = 2; g_lookR[i] = 2; }
}
int main()
{
    Group g;
    reset(g); g.Update();
    if (g_used != 1 || g_freed != 1 || g_freedIndex != 7 || g_term != 1 || g.candleSet) return fail(1);
    reset(g); g_moveR[0] = 4; g_moveR[1] = 4; g_lookR[0] = 4; g.Update();
    if (g_mi != 3 || g_li != 2 || g_used != 1) return fail(2);
    reset(g); g_inUse = 1; g.Update();
    if (g_freed != 0 || g_term != 1 || g_mi != 0) return fail(3);
    reset(g); g_moveR[0] = 3; g.Update();
    if (g_freed != 1 || g_term != 1 || g_used != 0) return fail(4);
    reset(g); g_lookR[0] = 3; g.Update();
    if (g_freed != 1 || g_term != 0 || g_used != 0 || !g.candleSet) return fail(5);
    reset(g); g_freePointOk = 0; g.Update();
    if (g_freed != 0 || g_term != 1) return fail(6);
    std::printf("00946850_TEST PASS\n");
    return 0;
}