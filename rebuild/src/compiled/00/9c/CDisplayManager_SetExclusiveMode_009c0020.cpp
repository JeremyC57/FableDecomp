// CDisplayManager::SetExclusiveMode(bool) -- re-create the device with the last creation parameters but a new exclusive-mode flag
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009C0020, length 144 bytes.
// Leaf: ?SetExclusiveMode@CDisplayManager@@QAE?AW4ECreateDeviceSuccess@1@_N@Z
// Callees: CDisplayManager::ResetDevice(const CCreateDeviceParameters&) @ 0x009BFB30 (private).
// Layout: PDB CDisplayManager (748 bytes): ExclusiveMode +0x1a0 (416), LastDeviceCreationParameters at PDB +624; the
// shipped build stores the DeviceResetCallbacks list as one sentinel pointer (PDB 12 bytes), so every member after it
// sits 8 lower and the parameters live at +0x268. CCreateDeviceParameters (44 bytes, PDB) is copied memberwise
// (the bool at +0x1c as one byte, the CPresentationIntervalDesc at +0x20 as two dwords) into a 0x2c-byte frame local.
// Shape: when the flag already matches the method returns 2 (CREATE_DEVICE_SUCCESS: nothing to change); otherwise the copy of
// ExclusiveMode is dead (overwritten by the argument) so only the other members are copied, and ResetDevice's
// result is returned.
struct HWND__;

class C2DExtentsI { public:
    long X;                                        // +0x00
    long Y;                                        // +0x04
};

struct CPresentationIntervalDesc {
    bool PresentImmediate;                         // +0x00
    bool PresentImmediateIfSyncMissed;             // +0x01
    long VSyncCount;                               // +0x04
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
    ECreateDeviceSuccess SetExclusiveMode(bool exclusive);
};

CDisplayManager::ECreateDeviceSuccess CDisplayManager::SetExclusiveMode(bool exclusive)
{
    if (ExclusiveMode != exclusive) {
        CCreateDeviceParameters params(LastDeviceCreationParameters);
        params.ExclusiveMode = exclusive;
        return ResetDevice(params);
    }
    return CREATE_DEVICE_SUCCESS;
}