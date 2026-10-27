// behaviour test for 0x00C15F90 CUnprovenPKInterface_012B2A74::SetRecord14 (model; the guard counts enter/leave)
#include <cstdio>
static int g_depth = 0, g_seen = -1;
struct Guard { Guard() { ++g_depth; } ~Guard() { --g_depth; } };
struct Rec { int w[6]; };
struct Obj {
    Rec* Record;
    void Set(int value) { Guard lock; Record->w[20 / 4] = value; g_seen = g_depth; }
};
int main()
{
    Rec r = { { 0, 0, 0, 0, 0, 0 } }; Obj o; o.Record = &r;
    o.Set(9);
    int ok = r.w[20 / 4] == 9 && g_seen == 1 && g_depth == 0;
    for (int i = 0; i < 6; ++i) if (i != 20 / 4 && r.w[i] != 0) ok = 0;
    if (!ok) { std::printf("00c15f90_TEST FAIL\n"); return 1; }
    std::printf("00c15f90_TEST PASS\n");
    return 0;
}