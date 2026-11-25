// CEngineScreenEffectOutlineGlow::BeginOutlineRendering() -- when the effect is active: open the outline render-state capture block, fetch the three effect textures from the engine texture pool if missing, save the display's current render target and viewport, render into the outline texture's top surface and clear it to transparent black
// Leaf: BeginOutlineRendering
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B89A20, length 450 bytes, plain `ret`.
// Callees: texture pool get @ 0x00F36EE0 (engine global @ 0x01436E98 + 0x3190), CRenderTarget(const CSurface&, const
//   CSurface&) @ 0x00A10DC0, CSurface::operator= @ 0x009F2E70, CSurface::Uninitialise @ 0x009F2E20, CBase ctor/dtor
//   @ 0x0099A2D0/0x0099A2E0, CTexture top-surface getter @ 0x009F9E00 (CSurface by value), CRenderTarget::AttachTarget
//   @ 0x00A10E60, CDisplayManager::SetRenderTarget @ 0x009BF160 (global @ 0x01436E1C), the render core clear @ 0x009D8CF0
//   (global @ 0x01436E18). The render-state manager @ 0x013CB508 capture push is inlined.
// Layout: engine/CEngineScreenEffectOutlineGlow.h (textures +0xc.., handles +0x18.., OldRenderTarget +0x14c, OldViewport
//   +0x170, OutlineRenderingCaptureBlock_Open +0x1e0; the active byte is the retail-only +0x08, as in the landed
//   FinishOutlineRendering 0x00B89140). CSurface/CRenderTarget as the landed CRenderTarget::AttachTarget 0x00A10E60.
//   Display manager: current colour surface +0xc, depth surface +0x1c, viewport +0x1f4. Render-state manager: capture
//   stack of 8-byte entries at +8, its count +0x2808, capture mask +0x2814, the capture block +0x3a24 (retail body).
// Shape: the saved target is a scoped named CRenderTarget (built from the display surfaces, assigned member-wise into
//   OldRenderTarget, destroyed before the viewport copy); CSurface has a user virtual dtor, CRenderTarget an implicit one.
// Name fix (NF98, round 8, q11274): 0x009F9E00 is CTexture::GetSurfaceLevel(long) const (Ego_r.pdb / FableWin.pdb
//   ?GetSurfaceLevel@CTexture@@QBE?AVCSurface@@J@Z; engine/CTexture.h), was a made-up non-const GetSurface: in-place respell of the
//   local CTexture declaration and its call sites, as the landed self row 0x009F9E00 (NF93).
#include "engine/CEngineScreenEffectOutlineGlow.h"

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

struct CRenderManagerCore {
};
extern CRenderManagerCore* g_pRenderManagerCore;  // 0x01436E18

struct SCaptureEntry { void* Block; unsigned long State; };

class CRenderStateManager { public:
    unsigned char _pad000[8];
    SCaptureEntry CaptureStack[0x500];             // +0x008
    unsigned long CaptureStackCount;               // +0x2808
    unsigned char _pad280c[8];
    unsigned long CaptureMask;                     // +0x2814
    unsigned char _pad2818[0x3a24 - 0x2818];
    unsigned char CaptureBlock[4];                 // +0x3a24
    void OpenCaptureBlock()
    {
        CaptureStack[CaptureStackCount++].Block = CaptureBlock;
        CaptureMask <<= 1;
    }
};
extern CRenderStateManager* g_pRenderStateManager;   // 0x013CB508

struct CEngineScreenEffectOutlineGlow_Begin : CEngineScreenEffectOutlineGlow {
    unsigned char IsActive() const { return _pad_0x08[0]; }
    void BeginOutlineRendering();
};

// Naming (NF443 gen, 2026-11-25): callee-only rebind of this row's calls to their PDB spellings (T1 tracker; the target
//   self rows' Leaf / PDB name sets), through a bodiless declaration or the row's own model (no layout; rulings r2/r3, NF438 q1 friend):
//   0x009D8CF0 ?ClearRenderTarget@CRenderManager2D@@QAEXABVCRGBColour@@K@Z (was ?Clear@CRenderManagerCore@@QAEXABVCRGBColour@@K@Z)
class CRenderManager2D { public: void ClearRenderTarget(CRGBColour const&, unsigned long); };
void CEngineScreenEffectOutlineGlow_Begin::BeginOutlineRendering()
{
    if (IsActive()) {
        OutlineRenderingCaptureBlock_Open = true;
        g_pRenderStateManager->OpenCaptureBlock();
        if (!OutlineTexture)
            OutlineTexture = NEngineInternal::GPResourceManager->GetGPUScratchBufferTextureManager().StartTextureUse(OutlineTextureHandle);
        if (!Blur1Texture)
            Blur1Texture = NEngineInternal::GPResourceManager->GetGPUScratchBufferTextureManager().StartTextureUse(Blur1TextureHandle);
        if (!Blur2Texture)
            Blur2Texture = NEngineInternal::GPResourceManager->GetGPUScratchBufferTextureManager().StartTextureUse(Blur2TextureHandle);
        {
            CRenderTarget current(NEngineInternal::GPDisplayManager->CurrentRenderSurface,
                                  NEngineInternal::GPDisplayManager->CurrentZBufferSurface);
            *(CRenderTarget*)OldRenderTarget = current;
        }
        *(C2DBoxI*)&OldViewport_TLX = NEngineInternal::GPDisplayManager->Viewport;
        CRenderTarget target;
        target.AttachTarget(OutlineTexture->GetSurfaceLevel(0));
        NEngineInternal::GPDisplayManager->SetRenderTarget(target);
        reinterpret_cast<CRenderManager2D*>(g_pRenderManagerCore)->ClearRenderTarget(CRGBColour(0, 0, 0, 0), 4);
    }
}