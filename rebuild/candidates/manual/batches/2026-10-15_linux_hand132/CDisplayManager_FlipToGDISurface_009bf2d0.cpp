// CDisplayManager::FlipToGDISurface() -- in exclusive mode keep presenting until the front buffer is the GDI surface again (screen index 0), flagging a lost device
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009BF2D0, length 128 bytes.
// Callees: IDirect3DDevice9::Present (COM slot 17, `call [ecx+44h]`, `this` pushed), CVsyncCallbackTimer::OnPostSwapScreens()
// @ 0x009F73C0.
// Layout: PDB CDisplayManager: D3DDevice +0x58, NoBackBuffers +0x19c, ExclusiveMode +0x1a0, CurrentScreenIndex +0x218, DeviceLostFlag
// +0x255, FrameCounter +0x25c; VsyncCallbackTimer (scoped_ptr) +0x294 (PDB +668: the shipped DeviceResetCallbacks list is 8 bytes
// narrower).
// Shape: `while` loop inside the exclusive-mode test (speed TU: the loop head is 16-byte aligned); each pass bumps the frame
// counter, presents, flags D3DERR_DEVICELOST (0x88760868) or, on success, notifies the vsync timer and advances the screen
// index modulo the buffer count (`cdq ; idiv`); the index is re-read for the loop test.
struct IDirect3DDevice9 {                          // COM: IUnknown + IDirect3DDevice9 slots up to Present
    virtual long __stdcall QueryInterface(const void* iid, void** object);   // slot 0
    virtual unsigned long __stdcall AddRef();                                // slot 1
    virtual unsigned long __stdcall Release();                               // slot 2
    virtual long __stdcall TestCooperativeLevel();                           // slot 3
    virtual unsigned int __stdcall GetAvailableTextureMem();                 // slot 4
    virtual long __stdcall EvictManagedResources();                          // slot 5
    virtual long __stdcall GetDirect3D(void** d3d);                          // slot 6
    virtual long __stdcall GetDeviceCaps(void* caps);                        // slot 7
    virtual long __stdcall GetDisplayMode(unsigned int swapChain, void* mode);   // slot 8
    virtual long __stdcall GetCreationParameters(void* parameters);          // slot 9
    virtual long __stdcall SetCursorProperties(unsigned int x, unsigned int y, void* bitmap);   // slot 10
    virtual void __stdcall SetCursorPosition(int x, int y, unsigned long flags);                 // slot 11
    virtual int __stdcall ShowCursor(int show);                              // slot 12
    virtual long __stdcall CreateAdditionalSwapChain(void* parameters, void** swapChain);   // slot 13
    virtual long __stdcall GetSwapChain(unsigned int index, void** swapChain);               // slot 14
    virtual unsigned int __stdcall GetNumberOfSwapChains();                  // slot 15
    virtual long __stdcall Reset(void* parameters);                          // slot 16
    virtual long __stdcall Present(const void* sourceRect, const void* destRect, void* destWindow,
                                   const void* dirtyRegion);                 // slot 17 (+0x44)
};

const long D3DERR_DEVICELOST = (long)0x88760868;

struct CVsyncCallbackTimer {
    void OnPostSwapScreens();                      // 0x009F73C0
};

template <class T> struct scoped_ptr {
    T* ptr;
    T* operator->() const { return ptr; }
};

struct CDisplayManager {
    unsigned char     _pad0[0x58];
    IDirect3DDevice9* D3DDevice;                   // +0x58
    unsigned char     _pad5c[0x19c - 0x5c];
    long              NoBackBuffers;               // +0x19c
    bool              ExclusiveMode;               // +0x1a0
    unsigned char     _pad1a1[0x218 - 0x1a1];
    long              CurrentScreenIndex;          // +0x218
    unsigned char     _pad21c[0x255 - 0x21c];
    bool              DeviceLostFlag;              // +0x255
    unsigned char     _pad256[0x25c - 0x256];
    long              FrameCounter;                // +0x25c
    unsigned char     _pad260[0x294 - 0x260];
    scoped_ptr<CVsyncCallbackTimer> VsyncCallbackTimer;   // +0x294
    void FlipToGDISurface();
};

void CDisplayManager::FlipToGDISurface()
{
    if (ExclusiveMode) {
        while (CurrentScreenIndex != 0) {
            ++FrameCounter;
            long result = D3DDevice->Present(0, 0, 0, 0);
            if (result == D3DERR_DEVICELOST)
                DeviceLostFlag = true;
            else if (result == 0) {
                VsyncCallbackTimer->OnPostSwapScreens();
                CurrentScreenIndex = (CurrentScreenIndex + 1) % (NoBackBuffers + 1);
            }
        }
    }
}
