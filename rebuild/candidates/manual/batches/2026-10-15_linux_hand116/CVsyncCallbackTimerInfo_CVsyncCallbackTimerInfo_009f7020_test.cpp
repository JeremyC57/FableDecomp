#include <cstdio>
#include <cstring>
#include <new>
struct CVsyncCallbackTimerInfo {
    double CPUTimeStamp; double CPUPrevTimeStamp; unsigned long CPUFrameCount;
    double GPUTimeStamp; double GPUPrevTimeStamp; unsigned long GPUFrameCount;
    CVsyncCallbackTimerInfo() : CPUTimeStamp(0.0), CPUPrevTimeStamp(0.0), CPUFrameCount(0), GPUTimeStamp(0.0), GPUPrevTimeStamp(0.0), GPUFrameCount(0) {}
};
int main()
{
    int ok = 1;
    unsigned char raw[sizeof(CVsyncCallbackTimerInfo)];
    std::memset(raw, 0x5a, sizeof raw);
    CVsyncCallbackTimerInfo* p = new (raw) CVsyncCallbackTimerInfo;
    if (p->CPUTimeStamp != 0.0 || p->CPUPrevTimeStamp != 0.0 || p->CPUFrameCount != 0) ok = 0;
    if (p->GPUTimeStamp != 0.0 || p->GPUPrevTimeStamp != 0.0 || p->GPUFrameCount != 0) ok = 0;
    if (!ok) { std::printf("009f7020_TEST FAIL\n"); return 1; }
    std::printf("009f7020_TEST PASS\n");
    return 0;
}
