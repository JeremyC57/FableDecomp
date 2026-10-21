// CDisplayManager::SwapScreens() -- present the back buffer; flag a lost device, otherwise tick the vsync timer and advance the screen index
// Leaf: ?SwapScreens@CDisplayManager@@QAE_NXZ
// Retail: Fable.exe @ 0x009BEEB0, 97 bytes, plain `ret`; Ego_r.pdb ?SwapScreens@CDisplayManager@@QAE_NXZ.
// Body: ++FrameCounter; hr = D3DDevice->Present(0, 0, 0, 0) (IDirect3DDevice9 slot 17, +0x44); D3DERR_DEVICELOST
// (0x88760868) -> DeviceLostFlag = true, false; any other non-D3D_OK -> false; else VsyncCallbackTimer->0x009F73C0(),
// CurrentScreenIndex = (CurrentScreenIndex + 1) % (NoBackBuffers + 1), true.
// Layout: Ego_r.pdb CDisplayManager -- D3DDevice +0x58, NoBackBuffers +0x19c, CurrentScreenIndex +0x218, DeviceLostFlag
// +0x255, FrameCounter +0x25c (retail agrees), VsyncCallbackTimer +0x294 (Ego_r.pdb +0x29c: retail's STLport list is 8
// bytes smaller). No engine/ header. The timer method name is not confirmed.
// Shape: /O2 (16-aligned entry). The result test sits in an inline helper (name not recovered; Ego_r.pdb lists no such
// CDisplayManager method, so it was fully inlined): written in SwapScreens itself, every if/else/switch spelling lets the
// compiler cross-jump the two `xor al,al / pop esi / ret` returns (93 bytes); retail keeps both.
typedef long HRESULT;
struct IDirect3DDevice9 {
    virtual void __stdcall _v00(); virtual void __stdcall _v01(); virtual void __stdcall _v02(); virtual void __stdcall _v03();
    virtual void __stdcall _v04(); virtual void __stdcall _v05(); virtual void __stdcall _v06(); virtual void __stdcall _v07();
    virtual void __stdcall _v08(); virtual void __stdcall _v09(); virtual void __stdcall _v10(); virtual void __stdcall _v11();
    virtual void __stdcall _v12(); virtual void __stdcall _v13(); virtual void __stdcall _v14(); virtual void __stdcall _v15();
    virtual void __stdcall _v16();
    virtual HRESULT __stdcall Present(const void* source_rect, const void* dest_rect, void* dest_window, const void* dirty_region);   // slot 17
};
const HRESULT D3D_OK = 0;
const HRESULT D3DERR_DEVICELOST = (HRESULT)0x88760868;
class CVsyncCallbackTimer {
public:
    void OnPresent();                                    // 0x009F73C0 (name not confirmed)
};
class CDisplayManager {
public:
    unsigned char        _pad000[0x58];
    IDirect3DDevice9*    D3DDevice;                      // +0x058
    unsigned char        _pad05c[0x19c - 0x5c];
    long                 NoBackBuffers;                  // +0x19c
    unsigned char        _pad1a0[0x218 - 0x1a0];
    long                 CurrentScreenIndex;             // +0x218
    unsigned char        _pad21c[0x255 - 0x21c];
    bool                 DeviceLostFlag;                 // +0x255
    unsigned char        _pad256[0x25c - 0x256];
    long                 FrameCounter;                   // +0x25c
    unsigned char        _pad260[0x294 - 0x260];
    CVsyncCallbackTimer* VsyncCallbackTimer;             // +0x294 (scoped_ptr)
    bool SwapScreens();
    bool CheckPresentResult(HRESULT hr)
    {
        if (hr == D3DERR_DEVICELOST)
        {
            DeviceLostFlag = true;
            return false;
        }
        if (hr != D3D_OK)
            return false;
        return true;
    }
};
bool CDisplayManager::SwapScreens()
{
    ++FrameCounter;
    if (!CheckPresentResult(D3DDevice->Present(0, 0, 0, 0)))
        return false;
    VsyncCallbackTimer->OnPresent();
    CurrentScreenIndex = (CurrentScreenIndex + 1) % (NoBackBuffers + 1);
    return true;
}