// CSurfaceCompositor::~CSurfaceCompositor() -- release the compositor's counted render target, close a still-open render-state capture block, then the CBase root
// Leaf: ??1CSurfaceCompositor
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B2E6C0, length 82 bytes, `jmp` tail. Caller: the scalar deleting destructor
// 0x00F39D10 (vtable 0x01301CF0 slot 0 = CSurfaceCompositor, rtti-vtables.tsv). The manifest label
// (_Dest_val<allocator<pair<CCharString const,CCountedPointer<C3DSkeletalMorphTarget>>>>) is propagated and wrong.
// Layout: Ego_r.pdb CSurfaceCompositor (0x30) -- the landed ctor 0x00F38990's model: CurrentBlend +4, StateCaptureBlock +8
// (CRenderStateCaptureBlock {bool Open}), WasRendering +9, DestSurface +0xc, CCountedPointer<CRenderTarget> RenderTarget +0x10,
// C2DBoxF OldWindow +0x18, LayerCount +0x28, AlphaTarget +0x2c.
// Callees: CCPPointerInfo::DeleteFunc through the block, operator delete @ 0x00BFE9BC (the block),
// CRenderStateManager::RestoreCaptureBlock @ 0x00A05840 on g_pRenderStateManager @ 0x013CB508, CBaseClass::~CBaseClass @ 0x0099A2E0.
// Shape: the class declares no destructor (no own vtable store); members go in reverse order -- RenderTarget, then the
// capture block's inline destructor (`if (Open) { Open = false; restore; }`) -- and the out-of-line root dtor is the tail.
void __cdecl operator delete(void* p);                       // 0x00BFE9BC

#include "engine/CCountedPointer.h"

class CRenderStateManager { public:
    void RestoreCaptureBlock();                              // 0x00A05840
};
extern CRenderStateManager* g_pRenderStateManager;           // 0x013CB508

struct CRenderStateCaptureBlock {
    bool Open;
    ~CRenderStateCaptureBlock()
    {
        if (Open) {
            Open = false;
            g_pRenderStateManager->RestoreCaptureBlock();
        }
    }
};

#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0

class CSurface;
class CRenderTarget;
class C2DBoxF { public: float TLX, TLY, BRX, BRY; };

class CSurfaceCompositor : public CBaseClass {                    // vtable 0x01301CF0, 0x30 bytes
public:
    long                           CurrentBlend;             // +0x04 ECompositeBlendType
    CRenderStateCaptureBlock       StateCaptureBlock;        // +0x08
    bool                           WasRendering;             // +0x09
    CSurface*                      DestSurface;              // +0x0c
    CCountedPointer<CRenderTarget> RenderTarget;             // +0x10
    C2DBoxF                        OldWindow;                // +0x18
    long                           LayerCount;               // +0x28
    bool                           AlphaTarget;              // +0x2c
};

void DestroySurfaceCompositor(CSurfaceCompositor* compositor)   // emits the implicit destructor; not part of the retail body
{
    compositor->CSurfaceCompositor::~CSurfaceCompositor();
}