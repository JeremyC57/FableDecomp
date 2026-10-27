// behaviour test for 0x00C11120 CUnprovenPKInterface_012B2748::SetMember7C0 (model; the guard counts enter/leave)
#include <cstdio>
static int g_depth = 0, g_seen = -1;
struct Guard { Guard() { ++g_depth; } ~Guard() { --g_depth; } };
struct Obj {
    float Member;
    void Set(float value) { Guard lock; Member = value; g_seen = g_depth; }
};
int main()
{
    Obj o; o.Member = 0;
    o.Set((float)7);
    int ok = o.Member == (float)7 && g_seen == 1 && g_depth == 0;
    if (!ok) { std::printf("00c11120_TEST FAIL\n"); return 1; }
    std::printf("00c11120_TEST PASS\n");
    return 0;
}