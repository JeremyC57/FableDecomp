#include <cstdio>
// Behaviour of CTCTargetingSpirit::vfunc_8 (0x004DEC19): constant true.
struct CTCTargetingSpirit_Calls { bool vfunc_8(); };
bool CTCTargetingSpirit_Calls::vfunc_8() { return true; }
int main()
{
    CTCTargetingSpirit_Calls t;
    if (!t.vfunc_8()) { std::printf("004dec19_TEST FAIL\n"); return 1; }
    std::printf("004dec19_TEST PASS\n"); return 0;
}