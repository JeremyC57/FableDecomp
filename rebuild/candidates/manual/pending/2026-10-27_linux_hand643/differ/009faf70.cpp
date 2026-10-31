// CTexture::GenerateMipmapsWithGPU() -- inside a render-state capture block, render each mip level 1..n-1 of the D3D texture by attaching that level's surface to a render target and blitting with the GPU, then restore the display's previous target and viewport
// Grade: DIFFER(407v407) -- register colouring only (unchanged by the PDB respell): retail keeps the display manager in edi, the level in ebx and does the capture-block size/mask read-modify-write in edx/ebp; ours display ebx, level edi, size/mask ebp/ebx. Respelled this session to the PDB vocabulary (CSystemManager::GetDisplayManager over CCountedPointer<CDisplayManager>::Get, CDisplayManager::GetRenderTarget by value / GetViewport / SetRenderTarget(CRenderTarget&) / SetViewport(const C2DBoxI&), CTexture::GetNoLevels / GetSurfaceLevel / GetD3DTexture, CRenderStateCaptureBlock::Capture/Restore over CRenderStateManager::StartCaptureBlock(void*), CRenderTarget(CSurface&, CSurface&)); earlier tries: ++size, size = size + 1, member StateStackSize++, unsigned level, scoped target. Park: colouring tie.
// Leaf: ?GenerateMipmapsWithGPU@CTexture@@QAEXXZ
// Callees: GFGetSystemManager 0x009A4EC0, CRenderTarget::CRenderTarget(CSurface&, CSurface&) 0x00A10DC0, CBase::CBase 0x0099A2D0,
//   CBase::~CBase 0x0099A2E0, CRenderTarget::DetachTarget 0x00A10E10, IDirect3DTexture9 GetLevelCount (slot 13) / GetSurfaceLevel
//   (slot 18), CSurface::InitialiseFromTextureMipmap 0x009F2F10, CSurface copy ctor 0x009F2D60, CSurface::Uninitialise 0x009F2E20,
//   CRenderTarget::AttachTarget 0x00A10E60, CDisplayManager::SetRenderTarget 0x009BF160, CDisplayManager::SetViewport 0x009BEF80,
//   CTexture::BltToRenderTargetWithGPU 0x009FA780, CRenderStateManager::RestoreCaptureBlock() 0x00A05840.
// Layout: Ego_r.pdb CDisplayManager (RenderTargetSurface +0x0c, RenderTargetDepthBuffer +0x1c, ViewportBox +0x1f4),
//   CSystemManager PDisplayManager +0x60, CRenderStateManager (StateStackSize +0x2808, CurrentBookmarkMask +0x2814,
//   BookmarkDummyState +0x3a24). engine/CTexture.h is a packed byte view of the bitfields, so CTexture is local here.
struct CStateInfo { unsigned char _body[0x14]; };
struct CStateStackElement { CStateInfo* State; unsigned long Value; };
class CRenderStateManager { public:
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
    void StartCaptureBlock(void* block)
    {
        StateStack[StateStackSize].State = &BookmarkDummyState;
        StateStackSize++;
        CurrentBookmarkMask <<= 1;
    }
    void RestoreCaptureBlock();              // 0x00A05840
};
extern CRenderStateManager* g_pRenderStateManager;   // 0x013CB508
class CRenderStateCaptureBlock { public:
    CRenderStateCaptureBlock() { Capture(); }
    ~CRenderStateCaptureBlock() { Restore(); }
    void Capture() { g_pRenderStateManager->StartCaptureBlock(this); }
    void Restore() { g_pRenderStateManager->RestoreCaptureBlock(); }
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
class CBase { public:
    CBase();                                                // 0x0099A2D0
    virtual ~CBase();                                       // 0x0099A2E0
};
class CSurface { public:                                    // vtable 0x0122F84C
    IDirect3DSurface9* PD3DSurface;                         // +0x04
    unsigned long AllocationSource;                         // +0x08
    void* PAllocatedMemory;                                 // +0x0c
    CSurface() : PD3DSurface(0), AllocationSource(0) {}
    CSurface(const CSurface& other);                        // 0x009F2D60
    virtual ~CSurface() { Uninitialise(); }
    void InitialiseFromTextureMipmap(IDirect3DSurface9* surface);   // 0x009F2F10
    void Uninitialise();                                    // 0x009F2E20
};
class CRenderTarget : public CBase { public:                // vtable 0x0129C800
    CSurface RenderToSurface;                               // +0x04
    CSurface ZBufferSurface;                                // +0x14
    CRenderTarget() {}
    CRenderTarget(CSurface& renderTo, CSurface& zBuffer);   // 0x00A10DC0
    void DetachTarget();                                    // 0x00A10E10
    void AttachTarget(const CSurface& surface);             // 0x00A10E60
};
class C2DBoxI { public: long TLX, TLY, BRX, BRY; };
class CDisplayManager { public:
    void*         __vftable;                                // +0x00
    void*         SystemManager;                            // +0x04
    void*         PRenderManager;                           // +0x08
    CSurface      RenderTargetSurface;                      // +0x0c
    CSurface      RenderTargetDepthBuffer;                  // +0x1c
    unsigned char _pad2c[0x1f4 - 0x2c];
    C2DBoxI       ViewportBox;                              // +0x1f4
    CRenderTarget GetRenderTarget() { return CRenderTarget(RenderTargetSurface, RenderTargetDepthBuffer); }
    const C2DBoxI& GetViewport() const { return ViewportBox; }
    bool SetRenderTarget(CRenderTarget& target);            // 0x009BF160
    void SetViewport(const C2DBoxI& viewport);              // 0x009BEF80
};
template <class T> class CCountedPointer { public:
    T* PObject;
    T* Get() const { return PObject; }
};
class CSystemManager { public:
    unsigned char _pad00[0x60];
    CCountedPointer<CDisplayManager> PDisplayManager;       // +0x60
    CDisplayManager* GetDisplayManager() { return PDisplayManager.Get(); }
};
CSystemManager* __fastcall GFGetSystemManager();            // 0x009A4EC0
class CTexture { public:
    IDirect3DTexture9* PD3DTexture;                         // +0x00
    unsigned long      ByteLength : 28;                     // +0x04
    unsigned long      AllocationSource : 4;
    IDirect3DTexture9* GetD3DTexture() const { return PD3DTexture; }
    long GetNoLevels() const { return GetD3DTexture()->GetLevelCount(); }
    CSurface GetSurfaceLevel(long level) const
    {
        IDirect3DSurface9* surface;
        GetD3DTexture()->GetSurfaceLevel(level, &surface);
        CSurface result;
        result.InitialiseFromTextureMipmap(surface);
        return result;
    }
    void BltToRenderTargetWithGPU(bool a, bool b, bool c) const;  // 0x009FA780
    void GenerateMipmapsWithGPU();
};

void CTexture::GenerateMipmapsWithGPU()
{
    CRenderStateCaptureBlock captureBlock;
    CDisplayManager* display = GFGetSystemManager()->GetDisplayManager();
    CRenderTarget previous = display->GetRenderTarget();
    CRenderTarget target;
    for (long level = 1; level < GetNoLevels(); ++level) {
        target.DetachTarget();
        target.AttachTarget(GetSurfaceLevel(level));
        display->SetRenderTarget(target);
        BltToRenderTargetWithGPU(true, true, false);
    }
    display->SetRenderTarget(previous);
    display->SetViewport(display->GetViewport());
}
