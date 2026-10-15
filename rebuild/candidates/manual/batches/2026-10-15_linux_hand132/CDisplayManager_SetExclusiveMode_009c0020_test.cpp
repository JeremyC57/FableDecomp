#include <cstdio>
#include <cstring>
// Behaviour test for CDisplayManager::SetExclusiveMode (0x009C0020): same flag -> 2 and no reset;
// different flag -> ResetDevice gets a copy of the last parameters with the new flag, and its result is returned.
struct CPresentationIntervalDesc { bool PresentImmediate; bool PresentImmediateIfSyncMissed; long VSyncCount; };
struct CCreateDeviceParameters {
    void* WinHandle; long X, Y; long BackBufferBitDepth, ZBufferBitDepth, BackBufferCount, RefreshRate;
    bool ExclusiveMode; CPresentationIntervalDesc PresentationInterval; long MultisampleType;
};
struct CDisplayManager {
    bool ExclusiveMode;
    CCreateDeviceParameters LastDeviceCreationParameters;
    int resets; CCreateDeviceParameters seen;
    long ResetDevice(CCreateDeviceParameters* p) { ++resets; seen = *p; return 7; }
    long SetExclusiveMode(bool exclusive)
    {
        if (ExclusiveMode != exclusive) {
            CCreateDeviceParameters params(LastDeviceCreationParameters);
            params.ExclusiveMode = exclusive;
            return ResetDevice(&params);
        }
        return 2;
    }
};
int main()
{
    CDisplayManager m; std::memset(&m, 0, sizeof(m));
    m.ExclusiveMode = true;
    m.LastDeviceCreationParameters.X = 640; m.LastDeviceCreationParameters.Y = 480;
    m.LastDeviceCreationParameters.ExclusiveMode = true; m.LastDeviceCreationParameters.MultisampleType = 3;
    int ok = 1;
    if (m.SetExclusiveMode(true) != 2 || m.resets != 0) ok = 0;
    if (m.SetExclusiveMode(false) != 7 || m.resets != 1) ok = 0;
    if (m.seen.ExclusiveMode != false || m.seen.X != 640 || m.seen.Y != 480 || m.seen.MultisampleType != 3) ok = 0;
    if (m.LastDeviceCreationParameters.ExclusiveMode != true) ok = 0;
    if (!ok) { std::printf("009c0020_TEST FAIL\n"); return 1; }
    std::printf("009c0020_TEST PASS\n");
    return 0;
}
