#include <cstdio>
// Behaviour of CTCTargetingSpirit::vfunc_32 (0x004DEC04): constant 0.
struct CTCTargetingSpirit_Calls { long vfunc_32(long a0); };
long CTCTargetingSpirit_Calls::vfunc_32(long a0) { return 0; }
int main()
{
    CTCTargetingSpirit_Calls t;
    if (t.vfunc_32(7) != 0) { std::printf("004dec04_TEST FAIL\n"); return 1; }
    std::printf("004dec04_TEST PASS\n"); return 0;
}
