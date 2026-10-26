#include <cstdio>
#include <cstring>
// Behaviour test for CDisplayManager::ChangePresentationInterval (0x009BFE70): an interval equal in
// PresentImmediate and VSyncCount does nothing; otherwise the device is reset with the new interval.
struct CPresentationIntervalDesc {
    bool PresentImmediate; bool PresentImmediateIfSyncMissed; long VSyncCount;
    bool operator==(const CPresentationIntervalDesc& o) const { return PresentImmediate == o.PresentImmediate && VSyncCount == o.VSyncCount; }
};
struct CCreateDeviceParameters {
    void* WinHandle; long X, Y; long BackBufferBitDepth, ZBufferBitDepth, BackBufferCount, RefreshRate;
    bool ExclusiveMode; CPresentationIntervalDesc PresentationInterval; long MultisampleType;
};
class CDisplayManager { public:
    CCreateDeviceParameters LastDeviceCreationParameters;
    int resets; CCreateDeviceParameters seen;
    long ResetDevice(CCreateDeviceParameters* p) { ++resets; seen = *p; return 0; }
    void ChangePresentationInterval(const CPresentationIntervalDesc& interval)
    {
        CCreateDeviceParameters params(LastDeviceCreationParameters);
        if (!(params.PresentationInterval == interval)) {
            params.PresentationInterval = interval;
            ResetDevice(&params);
        }
    }
};
int main()
{
    CDisplayManager m; std::memset(&m, 0, sizeof(m));
    m.LastDeviceCreationParameters.PresentationInterval.PresentImmediate = true;
    m.LastDeviceCreationParameters.PresentationInterval.VSyncCount = 1;
    m.LastDeviceCreationParameters.RefreshRate = 60;
    CPresentationIntervalDesc same; same.PresentImmediate = true; same.PresentImmediateIfSyncMissed = true; same.VSyncCount = 1;
    CPresentationIntervalDesc other; other.PresentImmediate = true; other.PresentImmediateIfSyncMissed = false; other.VSyncCount = 2;
    int ok = 1;
    m.ChangePresentationInterval(same);
    if (m.resets != 0) ok = 0;
    m.ChangePresentationInterval(other);
    if (m.resets != 1 || m.seen.PresentationInterval.VSyncCount != 2 || m.seen.RefreshRate != 60) ok = 0;
    if (!ok) { std::printf("009bfe70_TEST FAIL\n"); return 1; }
    std::printf("009bfe70_TEST PASS\n");
    return 0;
}