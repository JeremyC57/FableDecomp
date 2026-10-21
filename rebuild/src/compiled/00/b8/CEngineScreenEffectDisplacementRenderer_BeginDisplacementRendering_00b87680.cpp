// CEngineScreenEffectDisplacementRenderer::BeginDisplacementRendering() -- when the effect is active: fetch the displacement texture from the engine texture pool if missing, save the display's current render target and viewport, render into the displacement texture's top surface (cleared once to the neutral offset colour) and switch the cached render state at +0x2840 of the render core to 0xe; false when inactive
// Leaf: BeginDisplacementRendering
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B87680, length 496 bytes (body 0x1e6), plain `ret`.
// Callees: as the landed sibling BeginOutlineRendering 0x00B89A20 (texture pool get 0x00F36EE0, CRenderTarget ctor
//   0x00A10DC0, CSurface::operator=/Uninitialise 0x009F2E70/0x009F2E20, CBase 0x0099A2D0/0x0099A2E0, top surface
//   0x009F9E00, AttachTarget 0x00A10E60, SetRenderTarget 0x009BF160, the core clear 0x009D8CF0 on the global @ 0x01436E18).
// Layout: engine/CEngineScreenEffectDisplacementRenderer.h (DisplacementTexture +0x10, its handle +0x14, HasContent
//   +0x1c, ClearX/ClearY +0xe0/+0xe4, OldRenderTarget +0xf0, OldViewport +0x114; the active byte is the retail-only +0x08
//   as in the landed FinishDisplacementRendering 0x00B869A0). Render core (retail body): capture stack +8, dirty-state list
//   +0x2008, their counts +0x2808/+0x280c, capture mask +0x2814, the cached state {index, value, captured mask, dirty
//   +0x10} at +0x2840. CSurface/CRenderTarget as the landed CRenderTarget::AttachTarget 0x00A10E60.
// Shape: as BeginOutlineRendering (scoped saved target); the clear colour is CRGBColour(0xff, ClearY, ClearX, 0); the
//   state switch is an inline core method (early-out on an equal value, capture push, dirty-list push).
#include "engine/CEngineScreenEffectDisplacementRenderer.h"

struct CBase {
    CBase();                                       // 0x0099A2D0
    virtual ~CBase();                              // 0x0099A2E0
};

struct IDirect3DSurface9;

struct CSurface {                                  // vtable 0x0122F84C, 0x10 bytes
    IDirect3DSurface9* PD3DSurface;                // +0x04
    unsigned long AllocationSource;                // +0x08
    void* PAllocatedMemory;                        // +0x0c
    CSurface() : PD3DSurface(0), AllocationSource(0) {}
    virtual ~CSurface() { Uninitialise(); }
    void Uninitialise();                           // 0x009F2E20
    CSurface& operator=(const CSurface& other);    // 0x009F2E70
};

struct CRenderTarget : CBase {                     // vtable 0x0129C800
    CSurface RenderToSurface;                      // +0x04
    CSurface ZBufferSurface;                       // +0x14
    CRenderTarget() {}
    CRenderTarget(const CSurface& colour, const CSurface& depth);   // 0x00A10DC0
    CRenderTarget& operator=(const CRenderTarget& other)
    {
        RenderToSurface = other.RenderToSurface;
        ZBufferSurface = other.ZBufferSurface;
        return *this;
    }
    void AttachTarget(const CSurface& target);     // 0x00A10E60
};

struct C2DBoxI { long TLX, TLY, BRX, BRY; };

struct CDisplayManager {
    unsigned char _pad000[0xc];
    CSurface      CurrentRenderSurface;            // +0x0c
    CSurface      CurrentZBufferSurface;           // +0x1c
    unsigned char _pad02c[0x1f4 - 0x2c];
    C2DBoxI       Viewport;                        // +0x1f4
    void SetRenderTarget(CRenderTarget* target);   // 0x009BF160
};
extern CDisplayManager* g_pDisplayManager_01436e1c;   // 0x01436E1C

struct CTexture {
    CSurface GetSurface(long level);               // 0x009F9E00
};

struct CTexturePool_00f36ee0 {
    CTexture* Get(long handle);                    // 0x00F36EE0
};
struct CEngineGlobal_01436e98 {
    unsigned char         _pad[0x3190];
    CTexturePool_00f36ee0 m_texturePool;           // +0x3190
};
extern CEngineGlobal_01436e98* g_pEngine_01436e98;   // 0x01436E98

struct CRGBColour {
    unsigned char B, G, R, A;
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};

struct SCaptureEntry { void* Block; unsigned long State; };

struct CRenderStateValue {                         // one cached render state of the core
    unsigned long Index;                           // +0x00
    unsigned long Value;                           // +0x04
    unsigned long CapturedMask;                    // +0x08
    unsigned long _pad0c;
    bool          Dirty;                           // +0x10
};

struct CRenderManagerCore {
    unsigned char      _pad000[8];
    SCaptureEntry      CaptureStack[0x400];        // +0x0008
    CRenderStateValue* DirtyStates[0x200];         // +0x2008
    unsigned long      CaptureStackCount;          // +0x2808
    unsigned long      DirtyStateCount;            // +0x280c
    unsigned char      _pad2810[4];
    unsigned long      CaptureMask;                // +0x2814
    unsigned char      _pad2818[0x2840 - 0x2818];
    CRenderStateValue  DisplacementState;          // +0x2840
    void Clear(const CRGBColour& colour, unsigned long flags);   // 0x009D8CF0
    __forceinline void SetState(CRenderStateValue& state, unsigned long value)
    {
        if (state.Value == value)
            return;
        if (!(state.CapturedMask & CaptureMask)) {
            state.CapturedMask |= CaptureMask;
            CaptureStack[CaptureStackCount].Block = &state;
            CaptureStack[CaptureStackCount].State = state.Value;
            ++CaptureStackCount;
        }
        state.Value = value;
        if (!state.Dirty) {
            state.Dirty = true;
            DirtyStates[DirtyStateCount++] = &state;
        }
    }
};
extern CRenderManagerCore* g_pRenderManagerCore;  // 0x01436E18

struct CEngineScreenEffectDisplacementRenderer_Begin : CEngineScreenEffectDisplacementRenderer {
    unsigned char IsActive() const { return _pad_0x08[0]; }
    bool BeginDisplacementRendering();
};

bool CEngineScreenEffectDisplacementRenderer_Begin::BeginDisplacementRendering()
{
    if (!IsActive())
        return false;
    if (!DisplacementTexture)
        DisplacementTexture = g_pEngine_01436e98->m_texturePool.Get(DisplacementTextureHandle);
    {
        CRenderTarget current(g_pDisplayManager_01436e1c->CurrentRenderSurface,
                              g_pDisplayManager_01436e1c->CurrentZBufferSurface);
        *(CRenderTarget*)OldRenderTarget = current;
    }
    *(C2DBoxI*)&OldViewport_TLX = g_pDisplayManager_01436e1c->Viewport;
    CRenderTarget target;
    target.AttachTarget(DisplacementTexture->GetSurface(0));
    g_pDisplayManager_01436e1c->SetRenderTarget(&target);
    if (!HasContent) {
        g_pRenderManagerCore->Clear(CRGBColour(0xff, (unsigned char)ClearY, (unsigned char)ClearX, 0), 4);
        HasContent = true;
    }
    g_pRenderManagerCore->SetState(g_pRenderManagerCore->DisplacementState, 0xe);
    return true;
}