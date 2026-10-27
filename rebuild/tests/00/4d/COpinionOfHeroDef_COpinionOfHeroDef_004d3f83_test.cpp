// behaviour test for 0x004D3F83: the constructor sets the CDefIndex member to its default and leaves the scalars alone
#include <cstdio>
struct D { int base; int member; int scalar; D() : base(0), member(12345), scalar(7) { } void Construct() { base = 1; member = 0; } };
int main()
{
    D d; d.Construct();
    if (d.base != 1 || d.member != 0 || d.scalar != 7) { std::printf("004d3f83_TEST FAIL\n"); return 1; }
    std::printf("004d3f83_TEST PASS\n");
    return 0;
}