// behaviour test for 0x00C15F30 CUnprovenPKInterface_012B2A74::SetRecord0C (model; the guard counts enter/leave)
#include <cstdio>
static int g_depth = 0, g_seen = -1;
struct Guard { Guard() { ++g_depth; } ~Guard() { --g_depth; } };
struct Rec { int w[6]; };
struct Obj {
    Rec* Record;
    void Set(int value) { Guard lock; Record->w[12 / 4] = value; g_seen = g_depth; }
};
int main()
{
    Rec r = { { 0, 0, 0, 0, 0, 0 } }; Obj o; o.Record = &r;
    o.Set(9);
    int ok = r.w[12 / 4] == 9 && g_seen == 1 && g_depth == 0;
    for (int i = 0; i < 6; ++i) if (i != 12 / 4 && r.w[i] != 0) ok = 0;
    if (!ok) { std::printf("00c15f30_TEST FAIL\n"); return 1; }
    std::printf("00c15f30_TEST PASS\n");
    return 0;
}