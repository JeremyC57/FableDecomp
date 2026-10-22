// CTexture::GenerateMipmapsWithGPU() -- inside a render-state capture block, render each mip level 1..n-1 of the D3D texture by attaching that level's surface to a render target and blitting with the GPU, then restore the display's previous target and viewport
// Grade: DIFFER(407v407) -- register assignment only: retail keeps the display manager in edi, the level in ebx and does the capture-block size/mask read-modify-write in edx/ebp; ours swaps edi/ebx and uses ebp/ebx for the capture block (tried: ++size, size = size + 1, member StateStackSize++, unsigned level, scoped target)
// Leaf: GenerateMipmapsWithGPU
// Callees: GFGetSystemManager 0x9A4EC0 (+0x60 display manager), CRenderTarget(CSurface*, CSurface*) 0xA10DC0, CBase() 0x99A2D0, ~CBase 0x99A2E0, DetachTarget 0xA10E10,
//   IDirect3DTexture9 GetLevelCount (slot 13) / GetSurfaceLevel (slot 18), CSurface::InitialiseFromTextureMipmap 0x9F2F10, CSurface copy ctor 0x9F2D60, Uninitialise 0x9F2E20,
//   AttachTarget 0xA10E60, CDisplayManager::SetRenderTarget 0x9BF160, SetViewport 0x9BEF80, CTexture::BltToRenderTargetWithGPU 0x9FA780, RestoreCaptureBlock 0xA05840.
struct CStateInfo { unsigned char _body[0x14]; };
struct CStateStackElement { CStateInfo* State; unsigned long Value; };
struct CRenderStateManager {
    void*              __vftable;            // +0x00
    void*              PD3DDevice;           // +0x04
    CStateStackElement StateStack[1024];     // +0x08
    CStateInfo*        StateUpdateList[256]; // +0x2008
    CStateInfo*        AllStatesList[256];   // +0x2408
    long               StateStackSize;       // +0x2808
    long               StateUpdateListSize;  // +0x280c
    long               AllStatesListSize;    // +0x2810
    unsigned long      CurrentBookmarkMask;  // +0x2814
    unsigned char      DeviceState[0x3a24 - 0x2818];
    CStateInfo         BookmarkDummyState;   // +0x3a24
    void CreateCaptureBlock()
    {
        long& size = StateStackSize;
        StateStack[size].State = &BookmarkDummyState;
        StateStackSize++;
        CurrentBookmarkMask <<= 1;
    }
    void RestoreCaptureBlock();              // 0x00A05840
};
extern CRenderStateManager* g_pRenderStateManager;   // 0x013CB508
struct CRenderStateCaptureBlock {
    CRenderStateCaptureBlock() { g_pRenderStateManager->CreateCaptureBlock(); }
    ~CRenderStateCaptureBlock() { g_pRenderStateManager->RestoreCaptureBlock(); }
};
struct IDirect3DSurface9;
struct IDirect3DTexture9 {
    virtual void __stdcall v00(); virtual void __stdcall v01(); virtual void __stdcall v02(); virtual void __stdcall v03();
    virtual void __stdcall v04(); virtual void __stdcall v05(); virtual void __stdcall v06(); virtual void __stdcall v07();
    virtual void __stdcall v08(); virtual void __stdcall v09(); virtual void __stdcall v10(); virtual void __stdcall v11();
    virtual void __stdcall v12();
    virtual unsigned long __stdcall GetLevelCount();                                   // slot 13
    virtual void __stdcall v14(); virtual void __stdcall v15(); virtual void __stdcall v16(); virtual void __stdcall v17();
    virtual long __stdcall GetSurfaceLevel(unsigned int level, IDirect3DSurface9** surface);   // slot 18
};
struct CBase {
    CBase();                                                // 0x0099A2D0
    virtual ~CBase();                                       // 0x0099A2E0
};
struct CSurface {                                           // vtable 0x0122F84C
    IDirect3DSurface9* PSurface;                            // +0x04
    long Width;                                             // +0x08
    long Height;                                            // +0x0c
    CSurface() : PSurface(0), Width(0) {}
    CSurface(const CSurface& other);                        // 0x009F2D60
    virtual ~CSurface() { Uninitialise(); }
    void InitialiseFromTextureMipmap(IDirect3DSurface9* surface);   // 0x009F2F10
    void Uninitialise();                                    // 0x009F2E20
};
struct CRenderTarget : CBase {                              // vtable 0x0129C800
    CSurface Colour;                                        // +0x04
    CSurface Depth;                                         // +0x14
    CRenderTarget() {}
    CRenderTarget(CSurface* colour, CSurface* depth);       // 0x00A10DC0
    void DetachTarget();                                    // 0x00A10E10
    void AttachTarget(const CSurface& surface);             // 0x00A10E60
};
struct C2DBoxI { long TLX, TLY, BRX, BRY; };
struct CDisplayManager {
    unsigned char _pad00[0x0c];
    CSurface BackBuffer;                                    // +0x0c
    CSurface DepthBuffer;                                   // +0x1c
    unsigned char _pad2c[0x1f4 - 0x2c];
    C2DBoxI Viewport;                                       // +0x1f4
    bool SetRenderTarget(CRenderTarget* target);            // 0x009BF160
    void SetViewport(const C2DBoxI* viewport);              // 0x009BEF80
};
struct CSystemManager { unsigned char _pad00[0x60]; CDisplayManager* PDisplayManager; };
CSystemManager* __fastcall GFGetSystemManager();            // 0x009A4EC0
struct CTexture {
    IDirect3DTexture9* PD3DTexture;                         // +0x00
    void BltToRenderTargetWithGPU(bool a, bool b, bool c);  // 0x009FA780
    CSurface GetMipmapSurface(long level)
    {
        IDirect3DSurface9* surface;
        PD3DTexture->GetSurfaceLevel(level, &surface);
        CSurface result;
        result.InitialiseFromTextureMipmap(surface);
        return result;
    }
    void GenerateMipmapsWithGPU();
};

void CTexture::GenerateMipmapsWithGPU()
{
    CRenderStateCaptureBlock captureBlock;
    CDisplayManager* display = GFGetSystemManager()->PDisplayManager;
    CRenderTarget previous(&display->BackBuffer, &display->DepthBuffer);
    CRenderTarget target;
    for (long level = 1; level < (long)PD3DTexture->GetLevelCount(); ++level) {
        target.DetachTarget();
        target.AttachTarget(GetMipmapSurface(level));
        display->SetRenderTarget(&target);
        BltToRenderTargetWithGPU(true, true, false);
    }
    display->SetRenderTarget(&previous);
    display->SetViewport(&display->Viewport);
}
