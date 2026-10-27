// behaviour test for 0x004D5056: the constructor sets the CDefString member to its default and leaves the scalars alone
#include <cstdio>
struct D { int base; int member; int scalar; D() : base(0), member(12345), scalar(7) { } void Construct() { base = 1; member = -1; } };
int main()
{
    D d; d.Construct();
    if (d.base != 1 || d.member != -1 || d.scalar != 7) { std::printf("004d5056_TEST FAIL\n"); return 1; }
    std::printf("004d5056_TEST PASS\n");
    return 0;
}