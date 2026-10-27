// behaviour test for 0x00658B6B: both def references are released after the query
#include <cstdio>
struct Def { long refs; long start; long dur; };
struct Ptr { Def* o; Ptr(Def* d) : o(d) { if (o) ++o->refs; } ~Ptr() { if (o) --o->refs; } };
static bool Between(long start, long dur, long t) { return t >= start && t < start + dur; }
static bool IsTime(Def* d, long t) { return Between(Ptr(d).o->start, Ptr(d).o->dur, t); }
int main()
{
    Def d = { 1, 100, 50 };
    if (!IsTime(&d, 120) || IsTime(&d, 160) || d.refs != 1) { std::printf("00658b6b_TEST FAIL\n"); return 1; }
    std::printf("00658b6b_TEST PASS\n");
    return 0;
}