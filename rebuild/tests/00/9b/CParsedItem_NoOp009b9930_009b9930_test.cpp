#include <cstdio>
// Behaviour test for the empty one-argument member at 0x009B9930: it does nothing.
struct CParsedItem { long Type; void NoOp_009b9930(long) {} };
int main()
{
    CParsedItem p; p.Type = 7; p.NoOp_009b9930(99); int ok = (p.Type == 7);
    std::printf(ok ? "009b9930_TEST PASS\n" : "009b9930_TEST FAIL\n"); return ok ? 0 : 1;
}