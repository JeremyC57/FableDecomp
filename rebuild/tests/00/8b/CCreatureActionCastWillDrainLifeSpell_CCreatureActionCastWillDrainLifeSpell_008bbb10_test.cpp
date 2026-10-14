#include <cstdio>

struct Base { long type; Base(long t) : type(t) {} };
struct Mid : Base { long Level, DrainDefIndex; Mid(long t) : Base(t), Level(0), DrainDefIndex(0) {} };
struct Spell : Mid { Spell() : Mid(150) {} };
int main()
{
    Spell s; if (s.type != 150 || s.Level != 0 || s.DrainDefIndex != 0) { std::printf("008bbb10_TEST FAIL\n"); return 1; }
    std::printf("008bbb10_TEST PASS\n");
    return 0;
}