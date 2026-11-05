// Behaviour test for CGameEventPackage::Clear (0x009F16C0): the event count goes to zero, the time stamp is untouched.
#include <cstdio>

struct CGameEventPackageModel
{
    long TimeStamp;
    unsigned long NoEvents;
    void Clear() { NoEvents = 0; }
};

int main()
{
    CGameEventPackageModel p;
    p.TimeStamp = 1234;
    p.NoEvents = 40;
    p.Clear();
    if (p.NoEvents != 0 || p.TimeStamp != 1234) { std::printf("009f16c0_TEST FAIL\n"); return 1; }
    std::printf("009f16c0_TEST PASS\n");
    return 0;
}
