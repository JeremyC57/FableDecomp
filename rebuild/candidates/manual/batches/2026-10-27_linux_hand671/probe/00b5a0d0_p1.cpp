// CEngineScreenEffectColourFilter::~CEngineScreenEffectColourFilter() -- colour-filter screen effect destructor: release the colour-filter pixel shader and the screen-copy vertex shader, uninitialise the saved render target's two surfaces, then ~CEngineComponent
// Leaf: ??1CEngineScreenEffectColourFilter@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B5A0D0, 139 bytes, thiscall, tail jump to CEngineComponent::~CEngineComponent 0x00B59730.
// Identity (proven): the first store is vtable 0x012A25CC (rtti-vtables.tsv: CEngineScreenEffectColourFilter, RTTI
//   .?AVCEngineScreenEffectColourFilter@@ : CEngineComponent); the sole caller 0x00B5BB03 is the scalar deleting destructor
//   0x00B5BB00, slot 0 of that vtable. Ego_r.pdb / FableWin.pdb ??1CEngineScreenEffectColourFilter@@UAE@XZ; the catalog
//   label is right. The members walked are Ego_r's (retail offsets equal the PDB here).
// Callees: each shader data block's virtual deleting destructor (slot 0, flag 1) on its last release, CSurface::Uninitialise
//   0x009F2E20 (x2, inline user ~CSurface), CBaseClass::~CBaseClass 0x0099A2E0, ??1CEngineComponent@@UAE@XZ 0x00B59730 (tail).
// Vtables: CPixelShader 0x0129A054, CVertexShader 0x0129B3D0, CShaderBase 0x0129A030, CSurface 0x0122F84C (rtti-vtables.tsv).
// Layout: Ego_r.pdb CEngineScreenEffectColourFilter (0xEC): ForceColourFilter +0x0C, Settings +0x10, GrayRGBWeights +0x30,
//   GrayLevel +0x40, EffectSupported +0x44, TextureToBackBufferVerts +0x48 (CTVertexColTex1[4]), OldRenderTarget +0xA8
//   (CRenderTarget 0x24: CBaseClass, RenderToSurface +0x04, ZBufferSurface +0x14), OldViewport +0xCC, ScreenCopyVertexShader
//   +0xDC, ColourFilterPixelShader +0xE4. CPixelShaderData::ReferenceCount +0x08, CVertexShaderData::ReferenceCount +0x10.
//   engine/CEngineScreenEffectColourFilter.h and CEngineComponent.h are flattened (members as byte arrays), so the class is a
//   local model with the PDB member classes the destructor chain needs.
// Shape: empty user body (the own vtable store); the members' destructors in reverse order: each shader's inline user dtor
//   (own vtable, null-tested `dec ; jne ; call [vt]` release through a local copy of Data), then the protected empty
//   ~CShaderBase (its vtable store); CRenderTarget's implicit dtor (no store): ZBufferSurface, RenderToSurface, ~CBaseClass.
#include "engine/CBaseClass.h"                              // ~CBaseClass 0x0099A2E0

struct IDirect3DPixelShader9;
struct IDirect3DVertexShader9;
struct IDirect3DVertexDeclaration9;
struct _D3DVERTEXELEMENT9;
struct IDirect3DSurface9;

class CPixelShaderData {
    friend class CPixelShader;
    virtual ~CPixelShaderData();                            // private virtual (??1CPixelShaderData@@EAE@XZ)
public:
    IDirect3DPixelShader9* PixelShader;                     // +0x04
    unsigned long          ReferenceCount;                  // +0x08
    unsigned long          ConstCount;                      // +0x0C
    long*                  ConstIndices;                    // +0x10
};
class CVertexShaderData {
    friend class CVertexShader;
    virtual ~CVertexShaderData();                           // private virtual (??1CVertexShaderData@@EAE@XZ)
public:
    IDirect3DVertexDeclaration9* VertexDeclaration;         // +0x04
    IDirect3DVertexShader9*      VertexShader;              // +0x08
    _D3DVERTEXELEMENT9*          FormatDecl;                // +0x0C
    unsigned long                ReferenceCount;            // +0x10
};
class CShaderBase { public:                                 // vtable 0x0129A030
    CShaderBase() { }
protected:
    virtual ~CShaderBase() { }
};
class CPixelShader : public CShaderBase { public:           // vtable 0x0129A054
    CPixelShaderData* Data;                                 // +0x04
    CPixelShader() : Data(0) { }
    virtual ~CPixelShader()
    {
        CPixelShaderData* data = Data;
        if (data) {
            if (--data->ReferenceCount == 0)
                delete data;
        }
    }
};
class CVertexShader : public CShaderBase { public:          // vtable 0x0129B3D0
    CVertexShaderData* Data;                                // +0x04
    CVertexShader() : Data(0) { }
    virtual ~CVertexShader()
    {
        CVertexShaderData* data = Data;
        if (data) {
            if (--data->ReferenceCount == 0)
                delete data;
        }
    }
};
class CSurface { public:                                    // vtable 0x0122F84C, 0x10 bytes
    virtual ~CSurface() { Uninitialise(); }
    void Uninitialise();                                    // 0x009F2E20
    IDirect3DSurface9* PD3DSurface;                         // +0x04
    unsigned long      AllocationSource;                    // +0x08
    void*              PAllocatedMemory;                    // +0x0C
};
class CRenderTarget : public CBaseClass { public:           // 0x24 bytes, implicit destructor
    CSurface RenderToSurface;                               // +0x04
    CSurface ZBufferSurface;                                // +0x14
};

class CEngineComponent {                                    // engine/CEngineComponent.h offsets, 0x0C bytes
public:
    virtual ~CEngineComponent();                            // 0x00B59730
    void**        GlobalPointerAddress;                     // +0x04
    bool          RenderEnabled;                            // +0x08
    unsigned char KeepAlive;                                // +0x09
};

class CRGBFloatColour { public: float R, G, B, A; };
class CEngineColourFilterSettings { public: unsigned char _body[0x20]; };
class CTVertexColTex1 { public: float X, Y, Z; unsigned long Colour; float U, V; };
class C2DBoxI { public: long TLX, TLY, BRX, BRY; };
class CEngineScreenEffectColourFilter : public CEngineComponent {   // vtable 0x012A25CC
public:
    virtual ~CEngineScreenEffectColourFilter();
    bool                        ForceColourFilter;          // +0x0C
    CEngineColourFilterSettings Settings;                   // +0x10
    CRGBFloatColour             GrayRGBWeights;             // +0x30
    float                       GrayLevel;                  // +0x40
    bool                        EffectSupported;            // +0x44
    CTVertexColTex1             TextureToBackBufferVerts[4];// +0x48
    CRenderTarget               OldRenderTarget;            // +0xA8
    C2DBoxI                     OldViewport;                // +0xCC
    CVertexShader               ScreenCopyVertexShader;     // +0xDC
    CPixelShader                ColourFilterPixelShader;    // +0xE4
};

CEngineScreenEffectColourFilter::~CEngineScreenEffectColourFilter()
{
}
