// CCreateDeviceParameters::CCreateDeviceParameters(const CCreateDeviceParameters&) -- memberwise copy
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009C11A0, length 73 bytes. No callees.
// PDB layout (44 bytes): HWND WinHandle +0x00, C2DExtentsI BackBufferDimensions +0x04 (two longs),
// long BackBufferBitDepth +0x0c, ZBufferBitDepth +0x10, BackBufferCount +0x14, RefreshRate +0x18,
// bool ExclusiveMode +0x1c (copied as one byte, its padding is not), CPresentationIntervalDesc
// PresentationInterval +0x20 (two dwords), ESurfaceMultisampleType MultisampleType +0x28.
// `this` is moved to eax first and every member is copied through edx: the compiler-shaped
// memberwise copy constructor, written out so the translation unit emits it.
struct HWND__;

class C2DExtentsI { public:
    long Width;                                // +0x00
    long Height;                               // +0x04
};

struct CPresentationIntervalDesc {
    long Interval;                             // +0x00
    long Flags;                                // +0x04
};

enum ESurfaceMultisampleType { SURFACE_MULTISAMPLE_NONE = 0 };

struct CCreateDeviceParameters {
    HWND__*                   WinHandle;              // +0x00
    C2DExtentsI               BackBufferDimensions;   // +0x04
    long                      BackBufferBitDepth;     // +0x0c
    long                      ZBufferBitDepth;        // +0x10
    long                      BackBufferCount;        // +0x14
    long                      RefreshRate;            // +0x18
    bool                      ExclusiveMode;          // +0x1c
    CPresentationIntervalDesc PresentationInterval;   // +0x20
    ESurfaceMultisampleType   MultisampleType;        // +0x28
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