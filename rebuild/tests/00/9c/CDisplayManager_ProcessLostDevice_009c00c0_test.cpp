// Behaviour test for CDisplayManager::ProcessLostDevice (0x009C00C0): cooperative-level dispatch and the reset ladder
// (last parameters, then without multisampling, then capped to 1024x768).
#include <cstdio>
struct IDirect3DDevice9 { long level; virtual long TestCooperativeLevel() { return level; } };
class C2DExtentsI { public: long X, Y; };
struct CPresentationIntervalDesc { long Type, Interval; };
enum ESurfaceMultisampleType { MULTISAMPLE_NONE = 0 };
enum ECreateDeviceSuccess { CREATE_DEVICE_FAILED = 1, CREATE_DEVICE_SUCCEEDED = 2 };
struct CCreateDeviceParameters {
    void* WinHandle; C2DExtentsI BackBufferDimensions; long BackBufferBitDepth, ZBufferBitDepth, BackBufferCount, RefreshRate;
    bool ExclusiveMode; CPresentationIntervalDesc PresentationInterval; ESurfaceMultisampleType MultisampleType;
};
class CDisplayManager {
public:
    IDirect3DDevice9* D3DDevice; CCreateDeviceParameters LastDeviceCreationParameters;
    int calls; int results[4]; CCreateDeviceParameters seen[4];
    ECreateDeviceSuccess ResetDevice(CCreateDeviceParameters* p) { seen[calls] = *p; return (ECreateDeviceSuccess)results[calls++]; }
    bool ProcessLostDevice();
};
bool CDisplayManager::ProcessLostDevice()
{
    long result = D3DDevice->TestCooperativeLevel();
    if (result == (long)0x88760868)
        return false;
    if (result == 0)
        return true;
    if (result == (long)0x88760869) {
        ECreateDeviceSuccess reset = ResetDevice(&LastDeviceCreationParameters);
        if (reset == CREATE_DEVICE_FAILED)
            return false;
        if (reset == CREATE_DEVICE_SUCCEEDED)
            return true;
        CCreateDeviceParameters parameters(LastDeviceCreationParameters);
        if (parameters.MultisampleType != MULTISAMPLE_NONE) {
            parameters.MultisampleType = MULTISAMPLE_NONE;
            reset = ResetDevice(&parameters);
            if (reset == CREATE_DEVICE_FAILED)
                return false;
            if (reset == CREATE_DEVICE_SUCCEEDED)
                return true;
        }
        if (parameters.BackBufferDimensions.X > 1024)
            parameters.BackBufferDimensions.X = 1024;
        if (parameters.BackBufferDimensions.Y > 768)
            parameters.BackBufferDimensions.Y = 768;
        reset = ResetDevice(&parameters);
        if (reset == CREATE_DEVICE_FAILED)
            return false;
        return reset == CREATE_DEVICE_SUCCEEDED ? true : false;
    }
    return false;
}

static void Setup(CDisplayManager& d, IDirect3DDevice9& dev, long level, int r0, int r1, int r2, long ms)
{
    dev.level = level; d.D3DDevice = &dev; d.calls = 0;
    d.results[0] = r0; d.results[1] = r1; d.results[2] = r2; d.results[3] = 0;
    d.LastDeviceCreationParameters.BackBufferDimensions.X = 1600; d.LastDeviceCreationParameters.BackBufferDimensions.Y = 1200;
    d.LastDeviceCreationParameters.MultisampleType = (ESurfaceMultisampleType)ms;
}
int main()
{
    CDisplayManager d; IDirect3DDevice9 dev; int bad = 0;
    Setup(d, dev, (long)0x88760868, 0, 0, 0, 0); if (d.ProcessLostDevice() || d.calls) bad = 1;
    Setup(d, dev, 0, 0, 0, 0, 0); if (!d.ProcessLostDevice() || d.calls) bad = 2;
    Setup(d, dev, 5, 0, 0, 0, 0); if (d.ProcessLostDevice() || d.calls) bad = 3;
    Setup(d, dev, (long)0x88760869, 2, 0, 0, 0); if (!d.ProcessLostDevice() || d.calls != 1) bad = 4;
    Setup(d, dev, (long)0x88760869, 1, 0, 0, 0); if (d.ProcessLostDevice() || d.calls != 1) bad = 5;
    Setup(d, dev, (long)0x88760869, 0, 2, 0, 4);
    if (!d.ProcessLostDevice() || d.calls != 2 || d.seen[1].MultisampleType != 0 || d.seen[1].BackBufferDimensions.X != 1600) bad = 6;
    Setup(d, dev, (long)0x88760869, 0, 0, 2, 4);
    if (!d.ProcessLostDevice() || d.calls != 3 || d.seen[2].BackBufferDimensions.X != 1024 || d.seen[2].BackBufferDimensions.Y != 768) bad = 7;
    Setup(d, dev, (long)0x88760869, 0, 0, 0, 0);
    if (d.ProcessLostDevice() || d.calls != 2) bad = 8;   // no multisampling: straight to the capped retry, which returns 0
    if (bad) { std::printf("009c00c0_TEST FAIL\n"); return 1; }
    std::printf("009c00c0_TEST PASS\n");
    return 0;
}