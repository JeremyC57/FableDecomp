// CEngineWeatherRenderer::~CEngineWeatherRenderer() -- weather renderer destructor (empty user body): destroys the weather settings, the eight rain / snow / mist shaders, the mist and rain index buffers and the three vertex buffers in reverse declaration order, then ~CEngineComponent
// Leaf: ??1CEngineWeatherRenderer
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B513B0, 443 bytes, thiscall, tail jump to CEngineComponent::~CEngineComponent 0x00B59730.
// Identity (proven): stores vtable 0x012A2434 first (RTTI .?AVCEngineWeatherRenderer@@ : CEngineComponent); its sole caller
//   0x00B56693 is the scalar deleting destructor 0x00B56690 (slot 0 of that vtable); Ego_r.pdb ??1CEngineWeatherRenderer@@UAE@XZ.
//   The landed ctor 0x00B52250 lays the members out at the same offsets (engine/CEngineWeatherRenderer.h).
// Callees: ~CBaseClassNonCopyable 0x0099A300 (from the inline ~CEngineWeatherSettings, vtable 0x012A2424, and after each
//   CVertexBufferWin32::Uninitialise 0x00A63200 through the implicit ~CVertexBufferBase), CIndexBuffer::Uninitialise 0x00AC1EE0,
//   the shader data objects' virtual deleting destructors (CPixelShaderData refcount +0x08, CVertexShaderData refcount +0x10),
//   CEngineComponent::~CEngineComponent 0x00B59730.
// Shape: a user `~CEngineWeatherRenderer() { }` (own vtable store, then the members): the settings object is a CBaseClassNonCopyable
//   derivative with an inline empty virtual destructor (its vtable store + the out-of-line protected base destructor); the shader
//   wrappers release their counted data inline (`dec [data+N] ; jne ; call [vt]` with the deleting flag) and reset to the CShaderBase
//   vtable; CIndexBuffer / CVertexBufferWin32 call Uninitialise() and fall to CMovableResource / CBaseClassNonCopyable.
// Layout: engine/CEngineWeatherRenderer.h holds the offsets but flattens every member to bytes and the base to a vptr; the class is
//   restated over CEngineComponent (Ego_r.pdb 0xc: GlobalPointerAddress, RenderEnabled, KeepAlive) with the real member classes;
//   engine/CEngineComponent.h is the flattened form too (Header debt in NOTES.md). CEngineWeatherSettings (0x558, Ego_r.pdb) and
//   the trailing settings members after MistVolumeTexture take no part in the destructor and stay opaque.
#include <stddef.h>
#include "engine/CBaseClassNonCopyable.h"            // CBaseClass() 0x0099A2D0 / ~CBaseClass 0x0099A2E0, ~CBaseClassNonCopyable 0x0099A300
// Layout: engine/CVertexShader.h, engine/CIndexBuffer.h, engine/CVertexBufferWin32.h and engine/CMovableResource.h flatten the base
//   chains (no CShaderBase / CMovableResource / CVertexBufferBase bases, no inline destructors), so the shader and buffer members are
//   restated here with their RTTI chains and the inline destructors retail expands (the landed 0x00BBA480 / 0x00B52250 models).
class CVertexShaderData { public:                          // Ego_r.pdb, 0x14: ReferenceCount +0x10
    virtual ~CVertexShaderData();                          // slot 0 (private in Ego_r: ??1CVertexShaderData@@EAE@XZ)
    void*       VertexDeclaration;                         // IDirect3DVertexDeclaration9*
    void*       VertexShader;                              // IDirect3DVertexShader9*
    const void* FormatDecl;                                // _D3DVERTEXELEMENT9*
    unsigned long ReferenceCount;                          // +0x10
    unsigned long RemoveReference() { return --ReferenceCount; }
};
class CPixelShaderData { public:                           // Ego_r.pdb, 0x14: ReferenceCount +0x08
    virtual ~CPixelShaderData();                           // slot 0 (private in Ego_r: ??1CPixelShaderData@@EAE@XZ)
    void*         PixelShader;                             // IDirect3DPixelShader9*
    unsigned long ReferenceCount;                          // +0x08
    unsigned long ConstCount;
    long*         ConstIndices;
    unsigned long RemoveReference() { return --ReferenceCount; }
};
class CShaderBase { public:                                // vtable 0x0129A030
    virtual ~CShaderBase() { }
};
class CVertexShader : public CShaderBase { public:         // vtable 0x0129B3D0, 8 bytes
    CVertexShader();                                       // 0x00A62FB0
    virtual ~CVertexShader() { if (Data) { CVertexShaderData* current = Data; if (!current->RemoveReference()) delete current; } }
    CVertexShaderData* Data;                               // +0x04
};
class CPixelShader : public CShaderBase { public:          // vtable 0x0129A054, 8 bytes
    CPixelShader();                                        // 0x00A5EC00
    virtual ~CPixelShader() { if (Data) { CPixelShaderData* current = Data; if (!current->RemoveReference()) delete current; } }
    CPixelShaderData* Data;                                // +0x04
};
struct IDirect3DVertexBuffer9;
struct IDirect3DIndexBuffer9;
class CVertexCompressionParams { public:
    float PosScaleX, PosScaleY, PosScaleZ, PosScaleW;
    float PosBiasX, PosBiasY, PosBiasZ, PosBiasW;
};
class CVertexBufferBase : public CBaseClassNonCopyable { public:   // 0x34 bytes, implicit destructor (no vtable store)
    IDirect3DVertexBuffer9*  PVertexBuffer;                // +0x04
    unsigned long            Length;                       // +0x08
    long                     Type;                         // +0x0c EVertexType
    unsigned char            Stride;                       // +0x10
    CVertexCompressionParams CompressionParams;            // +0x14
};
class CVertexBufferWin32 : public CVertexBufferBase { public:      // vtable 0x0129BA20
    CVertexBufferWin32();
    virtual ~CVertexBufferWin32() { Uninitialise(); }
    void Uninitialise();                                   // 0x00A63200 (?Uninitialise@CVertexBufferWin32@@QAEXXZ)
};
class CMovableResource { public:                           // vtable 0x0129B860 (only FableWin has an out-of-line ??1)
    virtual ~CMovableResource() { }
};
class CIndexBuffer : public CMovableResource { public:     // vtable 0x0129EF2C, 0x14 bytes
    CIndexBuffer();                                        // 0x00AC1EC0
    virtual ~CIndexBuffer() { Uninitialise(); }
    void Uninitialise();                                   // 0x00AC1EE0 (?Uninitialise@CIndexBuffer@@QAEXXZ)
    IDirect3DIndexBuffer9* PD3DIndexBuffer;                // +0x04
    unsigned long          ByteLength;                     // +0x08
    long                   PrimitiveType;                  // +0x0c EPrimitiveType
    void*                  PAllocatedBuffer;               // +0x10
};
class CEngineComponentParams;
class CEngineComponent { public:                           // Ego_r.pdb 0xc; no RTTI base
    CEngineComponent();                                    // 0x00B59710
    virtual ~CEngineComponent();                           // slot 0, 0x00B59730 (??1CEngineComponent@@UAE@XZ)
    void** GlobalPointerAddress;                           // +0x04
    bool   RenderEnabled;                                  // +0x08
    unsigned char KeepAlive;                               // +0x09
};
class CEngineWeatherSettings : public CBaseClassNonCopyable { public:   // vtable 0x012A2424, 0x558 bytes (Ego_r.pdb)
    CEngineWeatherSettings();                              // 0x00B50FB0
    virtual ~CEngineWeatherSettings() { }                  // Ego_r ??1CEngineWeatherSettings@@UAE@XZ; expanded inline by retail
    unsigned char Body[0x554];                             // Enable .. ParticleBoxPos[81] (not touched here)
};
class CVolumeTexture;
class CEngineWeatherRenderer : public CEngineComponent { public:   // vtable 0x012A2434, 0x1328 bytes
    CEngineWeatherRenderer();                              // 0x00B52250 (landed)
    virtual ~CEngineWeatherRenderer();
    virtual void FrameStart(const CEngineComponentParams&);          // slot 1 0x00B53F60
    CVertexBufferWin32     RainVertexBuffer;                         // +0x0c
    CIndexBuffer           RainIndexBuffer;                          // +0x40
    CVertexBufferWin32     SnowVertexBuffer;                         // +0x54
    CVertexBufferWin32     MistVertexBuffer;                         // +0x88
    CIndexBuffer           MistIndexBuffer;                          // +0xbc
    CVertexShader          RainParticleVertexShader;                 // +0xd0
    CVertexShader          RainParticleClippingVertexShader;         // +0xd8
    CPixelShader           RainParticlePixelShader;                  // +0xe0
    CVertexShader          SnowParticleVertexShader;                 // +0xe8
    CVertexShader          SnowParticleClippingVertexShader;         // +0xf0
    CPixelShader           SnowParticlePixelShader;                  // +0xf8
    CVertexShader          MistVertexShader;                         // +0x100
    CPixelShader           MistPixelShader;                          // +0x108
    CVolumeTexture*        MistVolumeTexture;                        // +0x110
    CEngineWeatherSettings Settings;                                 // +0x114
    unsigned char          Rest[0x1328 - 0x66c];                     // +0x66c WeatherMaskList .. DebugBoxColour4
};
FABLE_STATIC_ASSERT(sizeof(CEngineWeatherRenderer) == 0x1328);
FABLE_STATIC_ASSERT(offsetof(CEngineWeatherRenderer, Settings) == 0x114);

CEngineWeatherRenderer::~CEngineWeatherRenderer()
{
}