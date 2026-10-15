#include <cstdio>
// Behaviour test for the empty member at 0x00F36D20: calling it changes nothing.
struct CDynamicObjectHeapBase { long a; void NoOp_00f36d20() {} };
int main()
{
    CDynamicObjectHeapBase h; h.a = 42; h.NoOp_00f36d20();
    std::printf(h.a == 42 ? "00f36d20_TEST PASS\n" : "00f36d20_TEST FAIL\n"); return h.a == 42 ? 0 : 1;
}
