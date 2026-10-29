// CSurfaceCompositor::CSurfaceCompositor(CSurface* destSurface) -- CBase ctor, clear the capture block, take the destination surface, create the compositor's own CRenderTarget into its counted pointer, zero the blend type and layer count
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F38990, length 117 bytes (+11 int3 padding). `ret 4`.
// Callees: CBaseClass::CBaseClass @ 0x0099A2D0 (twice: own base, then the new CRenderTarget's base), operator new @ 0x00BFEA1A (0x24 bytes),
// CCountedPointer<CRenderTarget>::Reset(CRenderTarget*) @ 0x00F39CA0 (catalog label is a byte-twin instantiation for
// CIPhysicsFacesSegmentBase). Vtables: CSurfaceCompositor 0x01301CF0, CRenderTarget 0x0129C800, CSurface 0x0122F84C.
// Shape: init list = StateCaptureBlock (its inline ctor clears the byte at +8), DestSurface, RenderTarget (two zero words);
// body = Reset(new CRenderTarget), then CurrentBlend = 0 and LayerCount = 0 (stored last). The CRenderTarget default ctor is
// inline: CBase, vtable, two embedded CSurface (vtable shared through eax, PD3DSurface/AllocationSource zeroed).
// Model: PDB CSurfaceCompositor (0x30 bytes): +4 CurrentBlend, +8 StateCaptureBlock, +9 WasRendering, +0xC DestSurface,
// +0x10 CCountedPointer<CRenderTarget> RenderTarget, +0x18 C2DBoxF OldWindow, +0x28 LayerCount, +0x2C AlphaTarget.
class CBaseClass {
public:
    CBaseClass();                                    // 0x0099A2D0
    virtual ~CBaseClass();
};
void* __cdecl operator new(unsigned int size);  // 0x00BFEA1A

struct IDirect3DSurface9;
class CSurface { public:                               // vtable 0x0122F84C, 0x10 bytes
    virtual ~CSurface();
    IDirect3DSurface9* PD3DSurface;             // +0x04
    unsigned long AllocationSource;             // +0x08
    void* PAllocatedMemory;                     // +0x0c
    CSurface() : PD3DSurface(0), AllocationSource(0) {}
};
class CRenderTarget : public CBaseClass { public:                  // vtable 0x0129C800, 0x24 bytes
    CSurface ColourSurface;                     // +0x04
    CSurface DepthSurface;                      // +0x14
    CRenderTarget() {}
    virtual ~CRenderTarget();
};
template <class T> class CCountedPointer { public:
    T* Data;                                    // +0x00
    void* Info;                                 // +0x04
    CCountedPointer() : Data(0), Info(0) {}
    void Reset(T* p);                           // 0x00F39CA0
};
struct CRenderStateCaptureBlock {
    bool Captured;
    CRenderStateCaptureBlock() : Captured(false) {}
};
class C2DBoxF { public: float left, top, right, bottom; };
struct CSurfaceCompositor : CBaseClass {             // vtable 0x01301CF0, 0x30 bytes
    long CurrentBlend;                          // +0x04 ECompositeBlendType
    CRenderStateCaptureBlock StateCaptureBlock; // +0x08
    bool WasRendering;                          // +0x09
    CSurface* DestSurface;                      // +0x0c
    CCountedPointer<CRenderTarget> RenderTarget;    // +0x10
    C2DBoxF OldWindow;                          // +0x18
    long LayerCount;                            // +0x28
    bool AlphaTarget;                           // +0x2c
    CSurfaceCompositor(CSurface* destSurface);
    virtual ~CSurfaceCompositor();
};

CSurfaceCompositor::CSurfaceCompositor(CSurface* destSurface)
    : DestSurface(destSurface)
{
    RenderTarget.Reset(new CRenderTarget);
    CurrentBlend = 0;
    LayerCount = 0;
}