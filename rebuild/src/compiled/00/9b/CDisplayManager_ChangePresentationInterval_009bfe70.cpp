// CDisplayManager::ChangePresentationInterval(const CPresentationIntervalDesc&) -- re-create the device when the requested present interval differs from the last one
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009BFE70, length 160 bytes.
// Callees: CDisplayManager::ResetDevice(const CCreateDeviceParameters&) @ 0x009BFB30 (private).
// Layout: as SetExclusiveMode (0x009C0020): LastDeviceCreationParameters at +0x268 (PDB +624, the shipped
// DeviceResetCallbacks list is 8 bytes narrower), copied memberwise into a 0x2c-byte frame local.
// Shape: the local copy is taken first (its VSyncCount word stays in esi, never stored, because the reset path
// overwrites it); the interval equality compares PresentImmediate and VSyncCount only (an inline operator==,
// PresentImmediateIfSyncMissed is ignored). On a change the new interval is copied in as two dwords and the device
// is reset; the reset result is discarded (void method).
struct HWND__;

class C2DExtentsI { public:
    long X;                                        // +0x00
    long Y;                                        // +0x04
};

struct CPresentationIntervalDesc {
    bool PresentImmediate;                         // +0x00
    bool PresentImmediateIfSyncMissed;             // +0x01
    long VSyncCount;                               // +0x04
    bool operator==(const CPresentationIntervalDesc& other) const
    {
        return PresentImmediate == other.PresentImmediate && VSyncCount == other.VSyncCount;
    }
};

enum ESurfaceMultisampleType { SURFACE_MULTISAMPLE_NONE = 0 };


class CDisplayManager { public:
    // NF140 (q4056): Ego_r.pdb / FableWin.pdb nest the result enum and the parameter block in CDisplayManager
    // (?ResetDevice@CDisplayManager@@AAE?AW4ECreateDeviceSuccess@1@ABUCCreateDeviceParameters@1@@Z); enumerators as Ego_d.pdb LF_ENUM.
    enum ECreateDeviceSuccess { CREATE_DEVICE_FAILED = 0, CREATE_DEVICE_DEVICE_LOST = 1, CREATE_DEVICE_SUCCESS = 2 };
    struct CCreateDeviceParameters {                   // PDB 44 bytes
        HWND__*                   WinHandle;           // +0x00
        C2DExtentsI               BackBufferDimensions;// +0x04
        long                      BackBufferBitDepth;  // +0x0c
        long                      ZBufferBitDepth;     // +0x10
        long                      BackBufferCount;     // +0x14
        long                      RefreshRate;         // +0x18
        bool                      ExclusiveMode;       // +0x1c
        CPresentationIntervalDesc PresentationInterval;// +0x20
        ESurfaceMultisampleType   MultisampleType;     // +0x28
        CCreateDeviceParameters(const CCreateDeviceParameters& other)   // memberwise (the out-of-line copy is 0x009C11A0)
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
    };
    unsigned char           _pad0[0x1a0];          // vptr slot, managers, surfaces, D3D objects, caps, extents
    bool                    ExclusiveMode;         // +0x1a0
    unsigned char           _pad1[0x268 - 0x1a1];  // multisample type .. device-reset callback list (+0x264)
    CCreateDeviceParameters LastDeviceCreationParameters; // +0x268
private:
    ECreateDeviceSuccess ResetDevice(const CCreateDeviceParameters& params);   // 0x009BFB30 (private, AAE)
public:
    void ChangePresentationInterval(const CPresentationIntervalDesc& interval);
};

void CDisplayManager::ChangePresentationInterval(const CPresentationIntervalDesc& interval)
{
    CCreateDeviceParameters params(LastDeviceCreationParameters);
    if (!(params.PresentationInterval == interval)) {
        params.PresentationInterval = interval;
        ResetDevice(params);
    }
}