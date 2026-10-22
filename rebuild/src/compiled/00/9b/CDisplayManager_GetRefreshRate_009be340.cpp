// CDisplayManager::GetRefreshRate() -- the default adapter's current display-mode refresh rate (IDirect3D9::GetAdapterDisplayMode), throwing when the query fails
// Leaf: GetRefreshRate
// Retail: Fable.exe (TLC, VC7.1) @ 0x009BE340, 54 bytes, `ret`. Unreferenced out-of-line copy.
// Callees: IDirect3D9 slot 8 (+0x20) GetAdapterDisplayMode(adapter 0, D3DDISPLAYMODE*); _CxxThrowException 0x00BFEB84
//   with ThrowInfo 0x013692F8 (the CCreateDeviceFailed type the landed CDisplayManager::Init 0x009C0E50 throws).
// Shape: the 16-byte D3DDISPLAYMODE is the frame (RefreshRate at +8 is returned); on a negative HRESULT the empty
//   exception (user ctor, so no zero store) is copied byte-wise from its dead frame byte into the throw slot.
//   Layout (retail, as in 0x009C0E50): IDirect3D9* PD3D +0x54.
struct D3DDISPLAYMODE {
    unsigned int Width;                            // +0x00
    unsigned int Height;                           // +0x04
    unsigned int RefreshRate;                      // +0x08
    long         Format;                           // +0x0C
};
struct IDirect3D9 {
    virtual long __stdcall v00(); virtual long __stdcall v01(); virtual long __stdcall v02(); virtual long __stdcall v03();
    virtual long __stdcall v04(); virtual long __stdcall v05(); virtual long __stdcall v06(); virtual long __stdcall v07();
    virtual long __stdcall GetAdapterDisplayMode(unsigned int adapter, D3DDISPLAYMODE* mode);   // slot 8 (+0x20)
};
struct CCreateDeviceFailed { CCreateDeviceFailed() {} };

class CDisplayManager {
public:
    unsigned char _pad000[0x54];
    IDirect3D9*   PD3D;                            // +0x054
    unsigned int GetRefreshRate();
};

unsigned int CDisplayManager::GetRefreshRate()
{
    D3DDISPLAYMODE mode;
    if (PD3D->GetAdapterDisplayMode(0, &mode) < 0)
        throw CCreateDeviceFailed();
    return mode.RefreshRate;
}