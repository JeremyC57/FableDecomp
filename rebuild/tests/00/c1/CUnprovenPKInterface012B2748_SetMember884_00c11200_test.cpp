// behaviour test for 0x00C11200 CUnprovenPKInterface_012B2748::SetMember884 (model; the guard counts enter/leave)
#include <cstdio>
static int g_depth = 0, g_seen = -1;
struct Guard { Guard() { ++g_depth; } ~Guard() { --g_depth; } };
struct Obj {
    unsigned char Member;
    void Set(unsigned char value) { Guard lock; Member = value; g_seen = g_depth; }
};
int main()
{
    Obj o; o.Member = 0;
    o.Set((unsigned char)7);
    int ok = o.Member == (unsigned char)7 && g_seen == 1 && g_depth == 0;
    if (!ok) { std::printf("00c11200_TEST FAIL\n"); return 1; }
    std::printf("00c11200_TEST PASS\n");
    return 0;
}