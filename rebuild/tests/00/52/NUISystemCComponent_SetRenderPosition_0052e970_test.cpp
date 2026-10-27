// behaviour test for 0x0052E970: the setter copies both coordinates into the member
#include <cstdio>
struct V { float X, Y; };
struct C { V Other; V Member; void Set(V& v) { Member = v; } };
int main()
{
    C c; c.Other.X = 1; c.Other.Y = 2; V v = { 3.5f, -4.0f }; c.Set(v);
    if (c.Member.X != 3.5f || c.Member.Y != -4.0f || c.Other.X != 1) { std::printf("0052e970_TEST FAIL\n"); return 1; }
    std::printf("0052e970_TEST PASS\n");
    return 0;
}