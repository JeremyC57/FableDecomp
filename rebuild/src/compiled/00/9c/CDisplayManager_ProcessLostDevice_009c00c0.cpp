// CDisplayManager::ProcessLostDevice() -- poll a lost Direct3D device: when it can be reset, retry the last creation parameters, then without multisampling, then at no more than 1024x768; true once a reset succeeds
// Leaf: ProcessLostDevice
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009C00C0, length 207 bytes, `ret`.
// Callees: IDirect3DDevice9::TestCooperativeLevel (COM slot 3, `call [ecx+0Ch]`, `this` pushed),
// CDisplayManager::ResetDevice(const CCreateDeviceParameters&) @ 0x009BFB30 (private; 1 = device lost, 2 = success),
// CDisplayManager::CCreateDeviceParameters copy constructor @ 0x009C11A0 (out of line here).
// Layout: PDB CDisplayManager: D3DDevice +0x58; LastDeviceCreationParameters at retail +0x268 (PDB +0x270). CCreateDeviceParameters
// (0x2c bytes): BackBufferDimensions +4, MultisampleType +0x28.
// Shape: D3DERR_DEVICELOST (0x88760868) is tested first (false), then S_OK (true); the D3DERR_DEVICENOTRESET
// (0x88760869) work is the body of an `if` with `return false` after it (shares the DEVICELOST tail); each ResetDevice result is tested `== 1` (false) then `== 2` (true); the fallback parameters
// are a local copy whose multisampling retry runs only when it was on, then the size is capped and the last result's
// `== 2` is returned as `? true : false` (bare `sete al`).
struct IDirect3DDevice9 {
    virtual long __stdcall QueryInterface(const void* iid, void** object);   // slot 0
    virtual unsigned long __stdcall AddRef();                                // slot 1
    virtual unsigned long __stdcall Release();                               // slot 2
    virtual long __stdcall TestCooperativeLevel();                           // slot 3 (+0xc)
};
class C2DExtentsI { public:
    long X;
    long Y;
};
struct CPresentationIntervalDesc {
    long Type;
    long Interval;
};
enum ESurfaceMultisampleType { MULTISAMPLE_NONE = 0 };
struct HWND__;

class CDisplayManager {
public:
    // NF140 (q4056): Ego_r.pdb / FableWin.pdb nest the result enum and the parameter block in CDisplayManager
    // (?ResetDevice@CDisplayManager@@AAE?AW4ECreateDeviceSuccess@1@ABUCCreateDeviceParameters@1@@Z); enumerators as Ego_d.pdb LF_ENUM.
    enum ECreateDeviceSuccess { CREATE_DEVICE_FAILED = 0, CREATE_DEVICE_DEVICE_LOST = 1, CREATE_DEVICE_SUCCESS = 2 };
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
        CCreateDeviceParameters(const CCreateDeviceParameters& other);   // 0x009C11A0
    };
    unsigned char _pad0[0x58];
    IDirect3DDevice9* D3DDevice;                   // +0x58
    unsigned char _pad1[0x268 - 0x5c];
    CCreateDeviceParameters LastDeviceCreationParameters;   // +0x268
private:
    ECreateDeviceSuccess ResetDevice(const CCreateDeviceParameters& parameters);   // 0x009BFB30 (private, AAE)
public:
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
        ECreateDeviceSuccess reset = ResetDevice(LastDeviceCreationParameters);
        if (reset == CREATE_DEVICE_DEVICE_LOST)
            return false;
        if (reset == CREATE_DEVICE_SUCCESS)
            return true;
        CCreateDeviceParameters parameters(LastDeviceCreationParameters);
        if (parameters.MultisampleType != MULTISAMPLE_NONE) {
            parameters.MultisampleType = MULTISAMPLE_NONE;
            reset = ResetDevice(parameters);
            if (reset == CREATE_DEVICE_DEVICE_LOST)
                return false;
            if (reset == CREATE_DEVICE_SUCCESS)
                return true;
        }
        if (parameters.BackBufferDimensions.X > 1024)
            parameters.BackBufferDimensions.X = 1024;
        if (parameters.BackBufferDimensions.Y > 768)
            parameters.BackBufferDimensions.Y = 768;
        reset = ResetDevice(parameters);
        if (reset == CREATE_DEVICE_DEVICE_LOST)
            return false;
        return reset == CREATE_DEVICE_SUCCESS ? true : false;
    }
    return false;
}