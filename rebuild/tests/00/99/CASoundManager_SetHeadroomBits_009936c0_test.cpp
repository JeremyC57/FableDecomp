// Behaviour test for CASoundManager::SetHeadroomBits (0x009936C0): the base body ignores the value (the LH override
// 0x00A3E330 does the work). Host model: the call leaves the manager unchanged.
#include <cstdio>
struct Manager { int state; void SetHeadroomBits(unsigned int) {} };
int main()
{
    Manager m; m.state = 3;
    m.SetHeadroomBits(12u); bool ok = m.state == 3;
    if (!ok) { std::printf("009936c0_TEST FAIL\n"); return 1; }
    std::printf("009936c0_TEST PASS\n");
    return 0;
}
