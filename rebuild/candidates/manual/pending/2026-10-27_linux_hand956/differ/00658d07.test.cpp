// behaviour test for 0x00658D07: the range check uses the def's start and end and releases both references
#include <cstdio>
struct Def { long refs; long start; long end; };
struct Ptr { Def* o; Ptr(Def* d) : o(d) { ++o->refs; } ~Ptr() { --o->refs; } };
static bool InRange(long t, long s, long e) { return s <= e ? (t >= s && t < e) : (t >= s || t < e); }
static bool IsTime(Def* d, long t) { return InRange(t, Ptr(d).o->start, Ptr(d).o->end); }
int main()
{
    Def d = { 1, 2200, 600 };
    if (!IsTime(&d, 2300) || !IsTime(&d, 100) || IsTime(&d, 1200) || d.refs != 1) { std::printf("00658d07_TEST FAIL\n"); return 1; }
    std::printf("00658d07_TEST PASS\n");
    return 0;
}
