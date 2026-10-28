// Test: CAIStateGroup_MinionReactToCrimeFromSleep::IsRunnable -- never runnable itself; when the creature is ready to sleep and the base group is runnable it wakes it (IsReadyToSleep = false)
#include <cstdio>
class CTCSleep { public: bool IsReadyToSleep; bool FastTrack; };
static bool IsRunnable(CTCSleep* tc, bool baseRunnable)
{
    CTCSleep* sleep = tc;
    bool readyToSleep = false;
    if (sleep) readyToSleep = sleep->IsReadyToSleep;
    if (!readyToSleep) return false;
    if (baseRunnable) sleep->IsReadyToSleep = false;
    return false;
}
int main()
{
    CTCSleep a = { true, false }, b = { true, false }, c = { false, false };
    if (IsRunnable(&a, true) || a.IsReadyToSleep) { std::printf("00958f00_TEST FAIL\n"); return 1; }
    if (IsRunnable(&b, false) || !b.IsReadyToSleep) { std::printf("00958f00_TEST FAIL\n"); return 1; }
    if (IsRunnable(&c, true) || c.IsReadyToSleep || IsRunnable(0, true)) { std::printf("00958f00_TEST FAIL\n"); return 1; }
    std::printf("00958f00_TEST PASS\n");
    return 0;
}