// CRenderManagerCore::CRenderManagerCore(CDisplayManager& displayManager, unsigned long initFlags) -- construct the render-state base, bind the display manager, and clear the per-stage texture bindings
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A0AFE0, length 116 bytes (+12 int3 padding).
// PDB: ??0CRenderManagerCore@@QAE@AAVCDisplayManager@@K@Z.
// Shape: the second argument goes to the out-of-line CRenderStateManager(unsigned long) base ctor (0x00A05970);
// DisplayManager (+0x3A3C) is set from an argument, so it stores before the vtable store (0x0129C9A4); then the
// in-place members in declaration order: the CPixelFormat at +0x3A48 (inline ctor, FormatIndex = -1), the
// CSystemFont at +0x3A4C (out-of-line CInitBaseClass base ctor 0x0099A310, then its own vtable 0x0129C998), and the
// body clears CurrentlySelectedTextures[8] (+0x3D00) with an element loop VC unrolls into eight `mov [..],eax`.
// Model: local polymorphic model (engine/CRenderManagerCore.h flattens the CRenderStateManager base into pads, and
// the vptr is a plain word there); offsets follow the engine header.
class CDisplayManager;
struct IDirect3DBaseTexture9;

class CRenderStateManager { public:                         // sizeof 0x3A3C (engine/CRenderStateManager.h)
    CRenderStateManager(unsigned long initFlags);    // 0x00A05970
    virtual ~CRenderStateManager();
    unsigned char _body[0x3A38];                     // +0x04 PD3DDevice .. InitFlags
};

class CInitBaseClass { public:                              // vtable 0x0129A7CC
    CInitBaseClass();                                // 0x0099A310
    virtual ~CInitBaseClass();
    bool Initialised;                                // +0x04
};

struct CSystemFont : CInitBaseClass {                // vtable 0x0129C998, 8 bytes
    CSystemFont() {}
    virtual ~CSystemFont();
};

class CPixelFormat { public:
    CPixelFormat() : FormatIndex(-1) {}
    long FormatIndex;
};

struct CRenderManagerCore : CRenderStateManager {   // vtable 0x0129C9A4
    CRenderManagerCore(CDisplayManager& displayManager, unsigned long initFlags);
    virtual ~CRenderManagerCore();

    CDisplayManager*       DisplayManager;           // +0x3A3C
    float                  LastRenderEndTime;        // +0x3A40
    long                   NoTrisPerSecond;          // +0x3A44
    CPixelFormat           ScreenPixelFormat;        // +0x3A48
    CSystemFont            SystemFont;               // +0x3A4C
    unsigned char          _flagsAndCaps[0x3D00 - 0x3A54];   // +0x3A54 Rendering .. FixedFunctionPipelineEnabled
    IDirect3DBaseTexture9* CurrentlySelectedTextures[8];     // +0x3D00
};

CRenderManagerCore::CRenderManagerCore(CDisplayManager& displayManager, unsigned long initFlags)
    : CRenderStateManager(initFlags), DisplayManager(&displayManager)
{
    for (long i = 0; i < 8; ++i)
        CurrentlySelectedTextures[i] = 0;
}