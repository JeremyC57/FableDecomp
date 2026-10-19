// CDisplayManager::SetDisplayMode(const C2DExtentsI& size, unsigned long bitDepth, ESurfaceMultisampleType multisample, long refreshRate) -- return 2 (succeeded, nothing to do) when the requested mode is the current one, else reset the device with the last creation parameters changed to the new size (at least 32x32), depth, refresh rate and multisampling
// Leaf: SetDisplayMode
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009BFF40, length 222 bytes, `ret 10h`.
// Callees: CPixelFormat::GetColourDepth() @ 0x009E3820, CDisplayManager::ResetDevice(CCreateDeviceParameters*)
// @ 0x009BFB30.
// Layout: PDB CDisplayManager: BackBufferDimensions +0x18c, MultisampleType +0x1a4, BackBufferFormat +0x1a8, RefreshRate
// +0x1c8; LastDeviceCreationParameters at retail +0x268 (PDB +0x270: the DeviceResetCallbacks list before it is a 4-byte
// STLport list). CCreateDeviceParameters (PDB, 0x2c bytes): WinHandle +0, BackBufferDimensions +4, BackBufferBitDepth
// +0xc, ZBufferBitDepth +0x10, BackBufferCount +0x14, RefreshRate +0x18, ExclusiveMode +0x1c, PresentationInterval
// +0x20 (8 bytes), MultisampleType +0x28.
// Shape: the unchanged test is one `&&` chain returning 2; the parameter block is a copy of the last parameters through
// an inline memberwise copy constructor (VC drops the stores the following assignments overwrite and schedules the
// kept members first; the implicit copy is a `rep movsd` block), then the size (clamped up to 32 per axis), depth,
// refresh rate and multisampling are assigned and the block is handed to ResetDevice.
struct C2DExtentsI {
    long X;                                        // +0x00
    long Y;                                        // +0x04
};
struct CPixelFormat {
    long FormatIndex;
    unsigned long GetColourDepth() const;          // 0x009E3820
};
struct CPresentationIntervalDesc {
    long Type;                                     // +0x00
    long Interval;                                 // +0x04
};
enum ESurfaceMultisampleType { MULTISAMPLE_NONE = 0 };
enum ECreateDeviceSuccess { CREATE_DEVICE_FAILED = 1, CREATE_DEVICE_SUCCEEDED = 2 };
struct HWND__;
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
    unsigned char _pad0[0x18c];
    C2DExtentsI BackBufferDimensions;              // +0x18c
    unsigned char _pad1[0x10];
    ESurfaceMultisampleType MultisampleType;       // +0x1a4
    CPixelFormat BackBufferFormat;                 // +0x1a8
    unsigned char _pad2[0x1c];
    long RefreshRate;                              // +0x1c8
    unsigned char _pad3[0x268 - 0x1cc];
    CCreateDeviceParameters LastDeviceCreationParameters;   // +0x268
    ECreateDeviceSuccess ResetDevice(CCreateDeviceParameters* parameters);   // 0x009BFB30
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
