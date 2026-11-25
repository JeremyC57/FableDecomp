// CEngineScreenEffectDisplacementRenderer::BeginDisplacementRendering() -- when the effect is active: fetch the displacement texture from the engine texture pool if missing, save the display's current render target and viewport, render into the displacement texture's top surface (cleared once to the neutral offset colour) and switch the cached render state at +0x2840 of the render core to 0xe; false when inactive
// Leaf: BeginDisplacementRendering
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B87680, length 496 bytes (body 0x1e6), plain `ret`.
// Callees: as the landed sibling BeginOutlineRendering 0x00B89A20 (texture pool get 0x00F36EE0, CRenderTarget ctor
//   0x00A10DC0, CSurface::operator=/Uninitialise 0x009F2E70/0x009F2E20, CBaseClass 0x0099A2D0/0x0099A2E0, top surface
//   0x009F9E00, AttachTarget 0x00A10E60, SetRenderTarget 0x009BF160, the core clear 0x009D8CF0 on the global @ 0x01436E18).
// Layout: engine/CEngineScreenEffectDisplacementRenderer.h (DisplacementTexture +0x10, its handle +0x14, HasContent
//   +0x1c, ClearX/ClearY +0xe0/+0xe4, OldRenderTarget +0xf0, OldViewport +0x114; the active byte is the retail-only +0x08
//   as in the landed FinishDisplacementRendering 0x00B869A0). Render core (retail body): capture stack +8, dirty-state list
//   +0x2008, their counts +0x2808/+0x280c, capture mask +0x2814, the cached state {index, value, captured mask, dirty
//   +0x10} at +0x2840. CSurface/CRenderTarget as the landed CRenderTarget::AttachTarget 0x00A10E60.
// Shape: as BeginOutlineRendering (scoped saved target); the clear colour is CRGBColour(0xff, ClearY, ClearX, 0); the
//   state switch is an inline core method (early-out on an equal value, capture push, dirty-list push).
// Name fix (NF98, round 8, q11274): 0x009F9E00 is CTexture::GetSurfaceLevel(long) const (Ego_r.pdb / FableWin.pdb
//   ?GetSurfaceLevel@CTexture@@QBE?AVCSurface@@J@Z; engine/CTexture.h), was a made-up non-const GetSurface: in-place respell of the
//   local CTexture declaration and its call sites, as the landed self row 0x009F9E00 (NF93).
#include "fable_compiler.h"
#include "engine/CEngineScreenEffectDisplacementRenderer.h"

#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0

struct IDirect3DSurface9;

class CSurface { public:                                  // vtable 0x0122F84C, 0x10 bytes
    IDirect3DSurface9* PD3DSurface;                // +0x04
    unsigned long AllocationSource;                // +0x08
    void* PAllocatedMemory;                        // +0x0c
    CSurface() : PD3DSurface(0), AllocationSource(0) {}
    virtual ~CSurface() { Uninitialise(); }
    void Uninitialise();                           // 0x009F2E20
    CSurface& operator=(const CSurface& other);    // 0x009F2E70
};

class CRenderTarget : public CBaseClass { public:                     // vtable 0x0129C800
    CSurface RenderToSurface;                      // +0x04
    CSurface ZBufferSurface;                       // +0x14
    CRenderTarget() {}
    CRenderTarget(CSurface& colour, CSurface& depth);   // 0x00A10DC0
    CRenderTarget& operator=(const CRenderTarget& other)
    {
        RenderToSurface = other.RenderToSurface;
        ZBufferSurface = other.ZBufferSurface;
        return *this;
    }
    void AttachTarget(const CSurface& target);     // 0x00A10E60
};

class C2DBoxI { public: long TLX, TLY, BRX, BRY; };

class CDisplayManager { public:
    unsigned char _pad000[0xc];
    CSurface      CurrentRenderSurface;            // +0x0c
    CSurface      CurrentZBufferSurface;           // +0x1c
    unsigned char _pad02c[0x1f4 - 0x2c];
    C2DBoxI       Viewport;                        // +0x1f4
    bool SetRenderTarget(CRenderTarget& target);   // 0x009BF160
};
namespace NEngineInternal { extern CDisplayManager* GPDisplayManager; }   // 0x01436E1C ?GPDisplayManager@NEngineInternal@@3PAVCDisplayManager@@A

class CTexture { public:
    CSurface GetSurfaceLevel(long level) const;    // 0x009F9E00
};

// NF44 (round 6): the 0x01436E98 global is NEngineInternal::GPResourceManager (Ego_r.pdb / FableWin.pdb
// ?GPResourceManager@NEngineInternal@@3PAVCEngineResourceManager@@A) and its +0x3190 member the CGPUScratchBufferTextureManager
// (engine/CEngineResourceManager.h GetGPUScratchBufferTextureManager(), engine/CGPUScratchBufferTextureManager.h, Ego_r.pdb member
// spellings); were the address-named CEngineGlobal_01436e98 / g_pEngine_01436e98 and local scratch-manager stand-ins.
#define FABLE_RETAIL_CENGINERESOURCEMANAGER
#define FABLE_RETAIL_CGPUSCRATCHBUFFERTEXTUREMANAGER_FREEMEMORYPAGE
#define FABLE_RETAIL_CGPUSCRATCHBUFFERTEXTUREMANAGER_ENDTEXTUREUSE
#define FABLE_RETAIL_CGPUSCRATCHBUFFERTEXTUREMANAGER_STARTTEXTUREUSE
#include "engine/CEngineResourceManager.h"
#include "engine/CGPUScratchBufferTextureManager.h"
namespace NEngineInternal { extern CEngineResourceManager* GPResourceManager; }   // 0x01436E98

class CRGBColour { public:
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
    FABLE_FORCEINLINE void SetState(CRenderStateValue& state, unsigned long value)
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

// Naming (NF443 gen, 2026-11-25): callee-only rebind of this row's calls to their PDB spellings (T1 tracker; the target
//   self rows' Leaf / PDB name sets), through a bodiless declaration or the row's own model (no layout; rulings r2/r3, NF438 q1 friend):
//   0x009D8CF0 ?ClearRenderTarget@CRenderManager2D@@QAEXABVCRGBColour@@K@Z (was ?Clear@CRenderManagerCore@@QAEXABVCRGBColour@@K@Z)
class CRenderManager2D { public: void ClearRenderTarget(CRGBColour const&, unsigned long); };
bool CEngineScreenEffectDisplacementRenderer_Begin::BeginDisplacementRendering()
{
    if (!IsActive())
        return false;
    if (!DisplacementTexture)
        DisplacementTexture = NEngineInternal::GPResourceManager->GetGPUScratchBufferTextureManager().StartTextureUse(DisplacementTextureHandle);
    {
        CRenderTarget current(NEngineInternal::GPDisplayManager->CurrentRenderSurface,
                              NEngineInternal::GPDisplayManager->CurrentZBufferSurface);
        *(CRenderTarget*)OldRenderTarget = current;
    }
    *(C2DBoxI*)&OldViewport_TLX = NEngineInternal::GPDisplayManager->Viewport;
    CRenderTarget target;
    target.AttachTarget(DisplacementTexture->GetSurfaceLevel(0));
    NEngineInternal::GPDisplayManager->SetRenderTarget(target);
    if (!HasContent) {
        reinterpret_cast<CRenderManager2D*>(g_pRenderManagerCore)->ClearRenderTarget(CRGBColour(0xff, (unsigned char)ClearY, (unsigned char)ClearX, 0), 4);
        HasContent = true;
    }
    g_pRenderManagerCore->SetState(g_pRenderManagerCore->DisplacementState, 0xe);
    return true;
}