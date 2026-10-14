#include <cstdio>
struct V { float x, y, z; V() : x(0), y(0), z(0) {} };
static long g_type = 0, g_dist = 0, g_c = 1; static float g_fa = 1, g_fb = 1; static void* g_s = (void*)1; static void* g_e = (void*)1; static V g_pos;
struct Mgr { void* AddMessage(long t, unsigned long d, V* p, void* s, void* e, float a, float b, long c) { g_type = t; g_dist = d; g_pos = *p; g_s = s; g_e = e; g_fa = a; g_fb = b; g_c = c; return 0; } };
static Mgr g_mgr; struct World { unsigned char pad[0x60]; Mgr* msgs; }; static World g_world;
struct Base { virtual void v00() {} World* GetWorld() { return &g_world; } };
struct Act : Base { void OnFollowUpActionFailedToAdd() { V o; GetWorld()->msgs->AddMessage(72, 30, &o, 0, 0, 0.0f, 0.0f, 0); } };
int main()
{
    g_world.msgs = &g_mgr; Act a; a.OnFollowUpActionFailedToAdd();
    if (g_type != 72 || g_dist != 30 || g_pos.y != 0 || g_s || g_e || g_fa != 0 || g_fb != 0 || g_c) { std::printf("00842bc0_TEST FAIL\n"); return 1; }
    std::printf("00842bc0_TEST PASS\n");
    return 0;
}
