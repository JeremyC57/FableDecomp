#include <cstdio>
struct V { float x, y, z; V() : x(0), y(0), z(0) {} };
static long g_type = 0, g_dist = 0, g_c = 1; static float g_fa = 1, g_fb = 1; static void* g_s = (void*)1; static void* g_e = (void*)1; static V g_pos;
struct Mgr { void* AddMessage(long t, unsigned long d, V* p, void* s, void* e, float a, float b, long c) { g_type = t; g_dist = d; g_pos = *p; g_s = s; g_e = e; g_fa = a; g_fb = b; g_c = c; return 0; } };
static Mgr g_mgr; struct World { unsigned char pad[0x60]; Mgr* msgs; }; static World g_world;
static int g_fin = 0; static bool g_open = false;
struct Base { virtual void FinishAction() { ++g_fin; } World* GetWorld() { return &g_world; } };
struct Act : Base { bool IsChestOpen() { return g_open; } void FinishAction() { if (!IsChestOpen()) { V o; GetWorld()->msgs->AddMessage(72, 50, &o, 0, 0, 0.0f, 0.0f, 0); } Base::FinishAction(); } };
int main()
{
    g_world.msgs = &g_mgr; Act a; a.FinishAction(); long t1 = g_type; g_type = 0; g_open = true; a.FinishAction();
    if (t1 != 72 || g_dist != 50 || g_pos.x != 0 || g_pos.z != 0 || g_s || g_e || g_fa != 0 || g_fb != 0 || g_c || g_type != 0 || g_fin != 2) { std::printf("00842b60_TEST FAIL\n"); return 1; }
    std::printf("00842b60_TEST PASS\n");
    return 0;
}
