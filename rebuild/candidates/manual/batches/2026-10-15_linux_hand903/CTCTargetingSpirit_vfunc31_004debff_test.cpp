#include <cstdio>
// Behaviour of CTCTargetingSpirit::vfunc_31 (0x004DEBFF): constant 0.
struct CTCTargetingSpirit_Calls { long vfunc_31(long a0, long a1, long a2); };
long CTCTargetingSpirit_Calls::vfunc_31(long a0, long a1, long a2) { return 0; }
int main()
{
    CTCTargetingSpirit_Calls t;
    if (t.vfunc_31(1, 2, 3) != 0) { std::printf("004debff_TEST FAIL\n"); return 1; }
    std::printf("004debff_TEST PASS\n"); return 0;
}
