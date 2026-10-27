// behaviour test for 0x00C14860 CUnprovenPKInterface_012B29F8::SetMember9C (model; the guard counts enter/leave)
#include <cstdio>
static int g_depth = 0, g_seen = -1;
struct Guard { Guard() { ++g_depth; } ~Guard() { --g_depth; } };
struct Obj {
    long Member;
    void Set(long value) { Guard lock; Member = value; g_seen = g_depth; }
};
int main()
{
    Obj o; o.Member = 0;
    o.Set((long)7);
    int ok = o.Member == (long)7 && g_seen == 1 && g_depth == 0;
    if (!ok) { std::printf("00c14860_TEST FAIL\n"); return 1; }
    std::printf("00c14860_TEST PASS\n");
    return 0;
}