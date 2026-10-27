// behaviour test for 0x0087E770: the getter returns a copy of the member vector
#include <cstdio>
struct V { float X, Y, Z; };
struct C { V Other; V Member; V Get() const { return Member; } };
int main()
{
    C c; c.Member.X = 1; c.Member.Y = 2; c.Member.Z = 3; c.Other.X = 9;
    V r = c.Get(); r.X = 7;
    if (r.Y != 2 || r.Z != 3 || c.Member.X != 1) { std::printf("0087e770_TEST FAIL\n"); return 1; }
    std::printf("0087e770_TEST PASS\n");
    return 0;
}