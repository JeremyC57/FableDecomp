// Behaviour test for CDisplayManager::SetDisplayMode (0x009BFF40): an unchanged mode returns 2 without a reset; any
// change resets with the last parameters plus the new size (clamped to >= 32), depth, refresh rate and multisampling.
#include <cstdio>
struct C2DExtentsI {
    long X;                                        // +0x00
    long Y;                                        // +0x04
};
struct CPixelFormat {
    long FormatIndex;
    unsigned long GetColourDepth() const { return FormatIndex == 1 ? 32 : 16; }
};
struct CPresentationIntervalDesc {
    long Type;                                     // +0x00
    long Interval;                                 // +0x04
};
enum ESurfaceMultisampleType { MULTISAMPLE_NONE = 0 };
enum ECreateDeviceSuccess { CREATE_DEVICE_FAILED = 1, CREATE_DEVICE_SUCCEEDED = 2 };
struct HWND__ { int unused; };
struct CCreateDeviceParameters {
    HWND__* WinHandle;                             // +0x00
    C2DExtentsI BackBufferDimensions;              // +0x04
    long BackBufferBitDepth;                       // +0x0c
    long ZBufferBitDepth;                          // +0x10
    long BackBufferCount;                          // +0x14
    long RefreshRate;                              // +0x18
    bool ExclusiveMode;                            // +0x1c
    CPresentationIntervalDesc PresentationInterval;   // +0x20
    ESurfaceMultisampleType MultisampleType;       // +0x28
    CCreateDeviceParameters() { }
    CCreateDeviceParameters(const CCreateDeviceParameters& o)
        : WinHandle(o.WinHandle), BackBufferDimensions(o.BackBufferDimensions), BackBufferBitDepth(o.BackBufferBitDepth),
          ZBufferBitDepth(o.ZBufferBitDepth), BackBufferCount(o.BackBufferCount), RefreshRate(o.RefreshRate),
          ExclusiveMode(o.ExclusiveMode), PresentationInterval(o.PresentationInterval), MultisampleType(o.MultisampleType) { }
};

class CDisplayManager {
public:
    C2DExtentsI BackBufferDimensions;              // +0x18c
    ESurfaceMultisampleType MultisampleType;       // +0x1a4
    CPixelFormat BackBufferFormat;                 // +0x1a8
    long RefreshRate;                              // +0x1c8
    CCreateDeviceParameters LastDeviceCreationParameters;   // +0x268
    CCreateDeviceParameters reset; int resets;
    ECreateDeviceSuccess ResetDevice(CCreateDeviceParameters* parameters) { reset = *parameters; ++resets; return CREATE_DEVICE_FAILED; }
    ECreateDeviceSuccess SetDisplayMode(const C2DExtentsI& size, unsigned long bitDepth, ESurfaceMultisampleType multisample, long refreshRate);
};

ECreateDeviceSuccess CDisplayManager::SetDisplayMode(const C2DExtentsI& size, unsigned long bitDepth, ESurfaceMultisampleType multisample, long refreshRate)
{
    if (size.X == BackBufferDimensions.X && size.Y == BackBufferDimensions.Y && bitDepth == BackBufferFormat.GetColourDepth()
        && refreshRate == RefreshRate && multisample == MultisampleType)
        return CREATE_DEVICE_SUCCEEDED;
    CCreateDeviceParameters parameters = LastDeviceCreationParameters;
    parameters.BackBufferDimensions = size;
    if (parameters.BackBufferDimensions.X < 32)
        parameters.BackBufferDimensions.X = 32;
    if (parameters.BackBufferDimensions.Y < 32)
        parameters.BackBufferDimensions.Y = 32;
    parameters.BackBufferBitDepth = bitDepth;
    parameters.RefreshRate = refreshRate;
    parameters.MultisampleType = multisample;
    return ResetDevice(&parameters);
}

int main()
{
    HWND__ window;
    CDisplayManager d; d.resets = 0;
    d.BackBufferDimensions.X = 640; d.BackBufferDimensions.Y = 480; d.BackBufferFormat.FormatIndex = 1; d.RefreshRate = 60;
    d.MultisampleType = MULTISAMPLE_NONE;
    d.LastDeviceCreationParameters.WinHandle = &window; d.LastDeviceCreationParameters.ZBufferBitDepth = 24;
    d.LastDeviceCreationParameters.BackBufferCount = 2; d.LastDeviceCreationParameters.ExclusiveMode = true;
    d.LastDeviceCreationParameters.PresentationInterval.Type = 3; d.LastDeviceCreationParameters.PresentationInterval.Interval = 7;
    d.LastDeviceCreationParameters.RefreshRate = 75; d.LastDeviceCreationParameters.BackBufferBitDepth = 16;
    int bad = 0;
    C2DExtentsI same = { 640, 480 };
    if (d.SetDisplayMode(same, 32, MULTISAMPLE_NONE, 60) != CREATE_DEVICE_SUCCEEDED || d.resets) bad = 1;
    C2DExtentsI tiny = { 8, 600 };
    if (d.SetDisplayMode(tiny, 16, (ESurfaceMultisampleType)2, 85) != CREATE_DEVICE_FAILED || d.resets != 1) bad = 2;
    const CCreateDeviceParameters& p = d.reset;
    if (p.BackBufferDimensions.X != 32 || p.BackBufferDimensions.Y != 600 || p.BackBufferBitDepth != 16 || p.RefreshRate != 85
        || p.MultisampleType != 2) bad = 3;
    if (p.WinHandle != &window || p.ZBufferBitDepth != 24 || p.BackBufferCount != 2 || !p.ExclusiveMode
        || p.PresentationInterval.Type != 3 || p.PresentationInterval.Interval != 7) bad = 4;
    if (bad) { std::printf("009bff40_TEST FAIL\n"); return 1; }
    std::printf("009bff40_TEST PASS\n");
    return 0;
}
