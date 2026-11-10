// Behaviour test for CASoundManager::SetHeadRelativePosition (0x009936B0): the base body ignores the position (the LH override
// 0x00A41800 does the work). Host model: the call leaves the manager unchanged.
#include <cstdio>
struct Vec { float X, Y, Z; };
struct Manager { int state; void SetHeadRelativePosition(const Vec&) {} };
int main()
{
    Manager m; m.state = 7; Vec v = { 1.0f, 2.0f, 3.0f };
    m.SetHeadRelativePosition(v); bool ok = m.state == 7;
    if (!ok) { std::printf("009936b0_TEST FAIL\n"); return 1; }
    std::printf("009936b0_TEST PASS\n");
    return 0;
}
