// Behaviour test for CVsyncCallbackTimer::GetInfo @ 0x009F7050: snapshot CPU globals + the current GPU record under its busy counter.
#include <cstdio>
struct CVsyncCallbackTimerInfo {
    double CPUTimeStamp, CPUPrevTimeStamp; long CPUFrameCount;
    double GPUTimeStamp, GPUPrevTimeStamp; long GPUFrameCount;
    CVsyncCallbackTimerInfo() : CPUTimeStamp(0.0), CPUPrevTimeStamp(0.0), CPUFrameCount(0),
        GPUTimeStamp(0.0), GPUPrevTimeStamp(0.0), GPUFrameCount(0) {}
};
struct SVsyncGPUTimeRecord { double TimeStamp, PrevTimeStamp; long FrameCount; long Busy; };
static SVsyncGPUTimeRecord g_VsyncGPUTimeRecords[3];
static long g_VsyncGPUTimeRecordIndex;
static double g_VsyncCPUTimeStamp, g_VsyncCPUPrevTimeStamp;
static long g_VsyncCPUFrameCount;
static long g_maxBusy;
static long Inc(long* p) { ++*p; if (*p > g_maxBusy) g_maxBusy = *p; return *p; }
static long Dec(long* p) { return --*p; }
class CVsyncCallbackTimer { public:
    void* Device; void* FreeQueoryObjects; void* PendingQueories; long PendingFrame, FinishedFrame;
    CVsyncCallbackTimerInfo GetInfo()
    {
        SVsyncGPUTimeRecord* pRecord = &g_VsyncGPUTimeRecords[g_VsyncGPUTimeRecordIndex];
        Inc(&pRecord->Busy);
        CVsyncCallbackTimerInfo info;
        info.CPUFrameCount = g_VsyncCPUFrameCount;
        info.GPUFrameCount = pRecord->FrameCount;
        info.CPUTimeStamp = g_VsyncCPUTimeStamp;
        info.CPUPrevTimeStamp = g_VsyncCPUPrevTimeStamp;
        info.GPUTimeStamp = pRecord->TimeStamp;
        info.GPUPrevTimeStamp = pRecord->PrevTimeStamp;
        Dec(&pRecord->Busy);
        return info;
    }
};
int main()
{
    for (int i = 0; i < 3; ++i) {
        g_VsyncGPUTimeRecords[i].TimeStamp = 10.0 * i + 1.5;
        g_VsyncGPUTimeRecords[i].PrevTimeStamp = 10.0 * i + 0.5;
        g_VsyncGPUTimeRecords[i].FrameCount = 100 + i;
        g_VsyncGPUTimeRecords[i].Busy = 0;
    }
    g_VsyncGPUTimeRecordIndex = 2;
    g_VsyncCPUTimeStamp = 7.25; g_VsyncCPUPrevTimeStamp = 6.75; g_VsyncCPUFrameCount = 42;
    CVsyncCallbackTimer t;
    CVsyncCallbackTimerInfo info = t.GetInfo();
    if (info.CPUTimeStamp != 7.25 || info.CPUPrevTimeStamp != 6.75 || info.CPUFrameCount != 42 ||
        info.GPUTimeStamp != 21.5 || info.GPUPrevTimeStamp != 20.5 || info.GPUFrameCount != 102 ||
        g_VsyncGPUTimeRecords[2].Busy != 0 || g_maxBusy != 1 || g_VsyncGPUTimeRecords[0].Busy != 0) {
        std::printf("009f7050_TEST FAIL\n"); return 1;
    }
    std::printf("009f7050_TEST PASS\n");
    return 0;
}