// behaviour test for 0x0052F540: the getter returns a copy of the member's four channels
#include <cstdio>
struct Col { unsigned char B, G, R, A; };
struct C { Col Other; Col Member; Col Get() const { return Member; } };
int main()
{
    C c; c.Member.B = 1; c.Member.G = 2; c.Member.R = 3; c.Member.A = 4; c.Other.B = 9;
    Col r = c.Get(); r.B = 7;
    if (r.G != 2 || r.R != 3 || r.A != 4 || c.Member.B != 1) { std::printf("0052f540_TEST FAIL\n"); return 1; }
    std::printf("0052f540_TEST PASS\n");
    return 0;
}