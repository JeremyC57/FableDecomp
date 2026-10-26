// CRenderManagerCore::Init(IDirect3DDevice9* device) -- initialise the render-state base, keep the device, start the system font, reset the render flags and counters, read the device caps and the back buffer's pixel format
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A0A940, length 130 bytes (`ret 4`).
// Callees: CRenderStateManager::Init(IDirect3DDevice9*) @ 0x00A07900 (the base at offset 0), CSystemFont::Initialise
// @ 0x00A6C170, IDirect3DDevice9::GetDeviceCaps (slot 7, +0x1c), CSurface::GetPixelFormat @ 0x009F2F60 (CPixelFormat
// returned through a hidden pointer).
// Layout: engine/CRenderManagerCore.h (PDB names, retail offsets agree): DisplayManager +0x3a3c, ScreenPixelFormat
// +0x3a48, SystemFont +0x3a4c, Rendering +0x3a54, TexturesEnabled +0x3a57, D3DDeviceCaps +0x3a58, TextureStagesInUse
// +0x3cb8, HighestMipmapToRender +0x3ccc, PD3DDevice +0x3cf0, PSelectedIndexBuffer +0x3cf4, XBoxShaderConstantsEnabled
// +0x3cfc, FixedFunctionPipelineEnabled +0x3cfd. CDisplayManager (PDB): BackBufferSurface +0x2c.
// Shape: statement order as below (one `mov al,1` for both true flags, one zero register `ebx` for the rest; the device
// is re-read after the font call); the pixel format temporary is homed in the dead device-argument slot.
// The method is attached through a derived shell because the generated header is a plain layout struct.
// Leaf: Init@CRenderManagerCore
#include "engine/CRenderManagerCore.h"

struct _D3DCAPS9;
struct IDirect3DDevice9 {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06();
    virtual long __stdcall GetDeviceCaps(_D3DCAPS9* caps);          // slot 7 (+0x1c)
};

class CPixelFormat { public:
    long FormatIndex;
};

class CSurface { public:
    void*         __vftable;
    void*         PD3DSurface;
    unsigned long AllocationSource;
    void*         PAllocatedMemory;
    CPixelFormat GetPixelFormat();                     // 0x009F2F60
};

class CDisplayManager { public:
    unsigned char _pad[0x2c];
    CSurface      BackBufferSurface;                   // +0x2c
};

struct CSystemFont {
    bool Initialise();                                 // 0x00A6C170
};

struct CRenderManagerCore_Init : CRenderManagerCore {
    void RenderStateManagerInit(IDirect3DDevice9* device);   // 0x00A07900 CRenderStateManager::Init (base at +0)
    void Init(IDirect3DDevice9* device);
};

void CRenderManagerCore_Init::Init(IDirect3DDevice9* device)
{
    RenderStateManagerInit(device);
    PD3DDevice = device;
    reinterpret_cast<CSystemFont*>(SystemFont)->Initialise();
    TexturesEnabled = true;
    FixedFunctionPipelineEnabled = true;
    HighestMipmapToRender = 0;
    PSelectedIndexBuffer = 0;
    PD3DDevice->GetDeviceCaps(reinterpret_cast<_D3DCAPS9*>(D3DDeviceCaps));
    TextureStagesInUse = 0;
    Rendering = false;
    ScreenPixelFormat_FormatIndex = DisplayManager->BackBufferSurface.GetPixelFormat().FormatIndex;
    XBoxShaderConstantsEnabled = false;
}