#include <cstdio>
static int g_baseCalls;
struct CBase { virtual void v00() { } CBase() { ++g_baseCalls; } };
struct CASuspendableProcess : CBase {
    unsigned char Running, StopRunning, TerminateUsingExceptions; long TimeProcessSliceStarted; long TimesliceLength;
    virtual void v00() { }
    CASuspendableProcess(bool t, long unused, long ts) : CBase(), Running(0), StopRunning(0), TerminateUsingExceptions(t), TimesliceLength(ts) { }
};
int main()
{
    CASuspendableProcess p(true, 99, 0x3f800000);
    if (g_baseCalls != 1 || p.Running != 0 || p.StopRunning != 0 || p.TerminateUsingExceptions != 1 || p.TimesliceLength != 0x3f800000
        || sizeof(p) != 0x10 || (char*)&p.TimesliceLength - (char*)&p != 0xc || (char*)&p.TerminateUsingExceptions - (char*)&p != 6) {
        std::printf("00a445d0_TEST FAIL\n"); return 1; }
    std::printf("00a445d0_TEST PASS\n");
    return 0;
}