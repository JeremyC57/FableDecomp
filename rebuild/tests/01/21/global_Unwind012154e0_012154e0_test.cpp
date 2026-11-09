// Behaviour test for CAmbientCube::Dump (0x00C45990): nothing is logged while every apex's minimum distance is zero; otherwise
// a header line and one "   Apex (x,y,z) = d" line per corner (corner i prints bits 2, 1, 0 of i) go to the log sink.
#include <cstdio>
#include <cstring>
#include <string>
static std::string g_log[16]; static int g_lines = 0;
static void Sink(const char* text) { g_log[g_lines++] = text; }
struct Apex { float offset[3], position[3], minDistance; };
struct Cube {
    Apex apexes[8];
    void Dump()
    {
        int any = 0;
        for (int i = 0; i < 8 && !any; ++i)
            any = apexes[i].minDistance != 0.0f;
        if (!any) return;
        Sink("Ambient cube:-\n");
        for (int i = 0; i < 8; ++i) {
            char buf[64];
            std::sprintf(buf, "   Apex (%d,%d,%d) = %g\n", (i >> 2) & 1, (i >> 1) & 1, i & 1, (double)apexes[i].minDistance);
            Sink(buf);
        }
    }
};
int main()
{
    Cube c; std::memset(&c, 0, sizeof c);
    c.Dump();
    bool ok = g_lines == 0;
    c.apexes[6].minDistance = 2.5f;
    c.Dump();
    ok = ok && g_lines == 9 && g_log[0] == "Ambient cube:-\n" && g_log[7] == "   Apex (1,1,0) = 2.5\n" && g_log[2] == "   Apex (0,0,1) = 0\n";
    if (!ok) { std::printf("00c45990_TEST FAIL\n"); return 1; }
    std::printf("012154e0_TEST PASS\n");
    return 0;
}