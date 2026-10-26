#include <cstdio>
struct HWND__;
class C2DExtentsI { public: long Width; long Height; };
struct CPresentationIntervalDesc { long Interval; long Flags; };
enum ESurfaceMultisampleType { SURFACE_MULTISAMPLE_NONE = 0, SURFACE_MULTISAMPLE_4 = 4 };
struct CCreateDeviceParameters {
    HWND__*                   WinHandle;
    C2DExtentsI               BackBufferDimensions;
    long                      BackBufferBitDepth;
    long                      ZBufferBitDepth;
    long                      BackBufferCount;
    long                      RefreshRate;
    bool                      ExclusiveMode;
    CPresentationIntervalDesc PresentationInterval;
    ESurfaceMultisampleType   MultisampleType;
    CCreateDeviceParameters() { }
    CCreateDeviceParameters(const CCreateDeviceParameters& other);
};
CCreateDeviceParameters::CCreateDeviceParameters(const CCreateDeviceParameters& other)
    : WinHandle(other.WinHandle)
    , BackBufferDimensions(other.BackBufferDimensions)
    , BackBufferBitDepth(other.BackBufferBitDepth)
    , ZBufferBitDepth(other.ZBufferBitDepth)
    , BackBufferCount(other.BackBufferCount)
    , RefreshRate(other.RefreshRate)
    , ExclusiveMode(other.ExclusiveMode)
    , PresentationInterval(other.PresentationInterval)
    , MultisampleType(other.MultisampleType)
{
}
int main()
{
    CCreateDeviceParameters a;
    a.WinHandle = reinterpret_cast<HWND__*>(0x1234);
    a.BackBufferDimensions.Width = 1024; a.BackBufferDimensions.Height = 768;
    a.BackBufferBitDepth = 32; a.ZBufferBitDepth = 24; a.BackBufferCount = 2; a.RefreshRate = 60;
    a.ExclusiveMode = true; a.PresentationInterval.Interval = 1; a.PresentationInterval.Flags = 5;
    a.MultisampleType = SURFACE_MULTISAMPLE_4;
    CCreateDeviceParameters b(a);
    if (b.WinHandle != a.WinHandle || b.BackBufferDimensions.Width != 1024 || b.BackBufferDimensions.Height != 768
        || b.BackBufferBitDepth != 32 || b.ZBufferBitDepth != 24 || b.BackBufferCount != 2 || b.RefreshRate != 60
        || !b.ExclusiveMode || b.PresentationInterval.Interval != 1 || b.PresentationInterval.Flags != 5
        || b.MultisampleType != SURFACE_MULTISAMPLE_4 || sizeof(CCreateDeviceParameters) != 44) { std::printf("009c11a0_TEST FAIL\n"); return 1; }
    std::printf("009c11a0_TEST PASS\n");
    return 0;
}