// CEnginePrimitiveManagerRepeatedZSprites::CEnginePrimitiveManagerRepeatedZSprites(CArray<CEngineScene*>* scenes) -- constructs the repeated z-sprite manager: looks up its vertex shader and its two pixel shaders by name, registers the EnableZSprites switch, builds the quad buffers, then allocates the dynamic texture page, the 256x256 billboard texture and z-buffer surface and sets the stipple / control-point defaults
// Leaf: ??0CEnginePrimitiveManagerRepeatedZSprites
// Naming wave 25: CPixelFormat::Initialise 0x009E3830 is ?Initialise@CPixelFormat@@QAEXW4_D3DFORMAT@@@Z (Ego_r.pdb; no constructor:
//   the body returns nothing), the format a _D3DFORMAT; a CPixelFormat built from a format is the inline constructor calling it.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BABB50, length 770 bytes (`ret 4`, no EH frame).
// Label correction: catalogued "CEnginePrimitiveManager2DSprites" (propagated); the vtable store 0x012A5314 is
//   CEnginePrimitiveManagerRepeatedZSprites (rtti-vtables.tsv: slot 1 CreateInternalPrimitive 0x00BAA820) and it calls BuildBuffers 0x00BAA6A0.
// Callees: CEnginePrimitiveManagerOcclusionBase::CEnginePrimitiveManagerOcclusionBase @ 0x00B90270, CVertexShader::CVertexShader
//   @ 0x00A62FB0, CPixelShader::CPixelShader @ 0x00A5EC00, CBaseClassNonCopyable::CBaseClassNonCopyable @ 0x0099A2F0 (vertex buffer root), CIndexBuffer::CIndexBuffer
//   @ 0x00AC1EC0, CCharString(const char*, long) @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0, CShaderResource::GetVertexShader @ 0x00A5D5F0 /
//   GetPixelShader @ 0x00A5D720, CEnginePrimitiveManager::RegisterEnable @ 0x00BACD80, BuildBuffers @ 0x00BAA6A0,
//   CGPUScratchBufferTextureManager AllocateMemoryPage @ 0x00F37660 / AllocateTexture @ 0x00F37440 / AllocateDepthBufferSurface @ 0x00F37390,
//   CPixelFormat::Initialise @ 0x009E3790 and CPixelFormat(D3DFORMAT) @ 0x009E3830.
// Layout: engine/CEnginePrimitiveManagerRepeatedZSprites.h names the members, but retail's PrimitiveSortList is a 12-byte
//   vector (PDB CArray 16), so every member after +0x80 sits 4 lower (BillboardTexture +0x8C, handles +0xA4/+0xA8/+0xAC,
//   NonStippleAlphaFactor +0xB0 ... CurrentRenderedMesh +0xC0): the class is restated locally (shader models as the landed 0x00BAD040).
// Shape: the shader lookups as the 2D sprite constructor (implicit CCharString temporaries); the pixel resource is read once into a
//   named local ahead of both pixel lookups; the vertex buffer's constructor is inline (CBase root, compression params in the
//   base, then the vtable store before the PVertexBuffer / Stride body stores of the derived constructor); the sort list is a real
//   STLport vector (_Vector_base + _STLP_alloc_proxy over an allocator temporary) so its three zero stores interleave with the first
//   CCharString pushes; the two 256x256 sizes are C2DCoordI TEMPORARIES passed by const reference (they share the shader-temporary
//   frame slot: sub esp,10h; named locals grow the frame to 0x18/0x20).
// NF46 (round 7): the 0x01436E98 global is NEngineInternal::GPResourceManager (Ego_r.pdb / FableWin.pdb
// ?GPResourceManager@NEngineInternal@@3PAVCEngineResourceManager@@A); the vertex shader resources are
// CEngineResourceManager::VShaderResources[bank] (retail +0x29C0 + 8*bank, FableWin.pdb inline GetVShaderResource) and the pixel one
// ShaderResourcePixel (+0x2A98, GetPShaderResource). Shader classes from the shared headers (CShaderResource::GetVertexShader /
// GetPixelShader const, Ego_r.pdb / FableWin.pdb); were a local CEngineData / g_pEngineData stand-in and local shader models.
// NF46: the +0x3190 scratch manager is GetGPUScratchBufferTextureManager() (engine/CGPUScratchBufferTextureManager.h, opt-in _TEXTURES and the
//   overlay opt-in _ALLOCATEDEPTHBUFFERSURFACE; Ego_r.pdb spellings AllocteMemoryPage (sic) / AllocateTexture / AllocateDepthBufferSurface taking
//   C2DExtentsI / CPixelFormat by reference, ESurfaceMultisampleType); the local CPixelFormat / C2DCoordI / _D3DFORMAT moved to
//   engine/CPixelFormat.h (FABLE_RETAIL_CPIXELFORMAT: the 8,8,8,8 format is the header's no -1 store constructor), C2DExtentsI.h, Direct3D9Types.h.
#define FABLE_RETAIL_CENGINERESOURCEMANAGER
#define FABLE_RETAIL_CPIXELFORMAT
#define FABLE_RETAIL_CGPUSCRATCHBUFFERTEXTUREMANAGER_TEXTURES
#define FABLE_RETAIL_CGPUSCRATCHBUFFERTEXTUREMANAGER_ALLOCATEDEPTHBUFFERSURFACE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CVERTEXSHADER
#include "fable_string.h"
#include "engine/CVertexShader.h"
#include "engine/CPixelShader.h"
#include "engine/CEngineResourceManager.h"
#include "engine/CShaderResource.h"
#include "engine/CPixelFormat.h"
#include "engine/C2DExtentsI.h"
#include "engine/CGPUScratchBufferTextureManager.h"
namespace NEngineInternal { extern CEngineResourceManager* GPResourceManager; }   // 0x01436E98

struct CEngineSceneArray;
class CBaseClassNonCopyable {
protected:
    virtual ~CBaseClassNonCopyable();   // 0x0099A300
    CBaseClassNonCopyable();                                       // 0x0099A2F0
};
struct IDirect3DVertexBuffer9;
class CVertexCompressionParams { public:
    float PosScaleX, PosScaleY, PosScaleZ, PosScaleW;
    float PosBiasX, PosBiasY, PosBiasZ, PosBiasW;
    CVertexCompressionParams()
        : PosScaleX(1.0f), PosScaleY(1.0f), PosScaleZ(1.0f), PosScaleW(1.0f),
          PosBiasX(0.0f), PosBiasY(0.0f), PosBiasZ(0.0f), PosBiasW(0.0f) { }
};
struct CVertexBufferBase : CBaseClassNonCopyable {                 // 0x34 bytes
    IDirect3DVertexBuffer9*  PVertexBuffer;        // +0x04
    unsigned long            Length;               // +0x08
    long                     Type;                 // +0x0c
    unsigned char            Stride;               // +0x10
    CVertexCompressionParams CompressionParams;    // +0x14
    CVertexBufferBase() : Length(0), Type(0), CompressionParams() { }
};
class CVertexBufferWin32 : public CVertexBufferBase { public:    // vtable 0x0129BA20
    virtual void VSlot1();
    CVertexBufferWin32()
    {
        PVertexBuffer = 0;
        Stride = 0;
    }
};
class CIndexBuffer { public:                              // 0x14 bytes
    void* _vtable;
    unsigned char _body[0x10];
    CIndexBuffer();                                // 0x00AC1EC0
};
template <class T> struct allocator { allocator() { } };
template <class V, class A> struct _STLP_alloc_proxy : A {
    V _M_data;
    _STLP_alloc_proxy(const A& a, V v) : A(a), _M_data(v) { }
};
template <class T> struct _Vector_base {
    T* _M_start;
    T* _M_finish;
    _STLP_alloc_proxy<T*, allocator<T> > _M_end_of_storage;
    _Vector_base(const allocator<T>& a) : _M_start(0), _M_finish(0), _M_end_of_storage(a, (T*)0) { }
};
template <class T> struct CPODArray : _Vector_base<T> {   // retail 12-byte STLport vector
    CPODArray(const allocator<T>& a = allocator<T>()) : _Vector_base<T>(a) { }
};
class CEngineBillboard { public:                          // 0x18 bytes
    void* Texture;
    void* Surface;
    unsigned char _rest[0x10];
};
class CEngineInternalPrimitiveRepeatedZSprite;
class C3DMeshInfo;

struct CEnginePrimitiveManagerOcclusionBase {      // 0x20 bytes
    virtual ~CEnginePrimitiveManagerOcclusionBase();
    unsigned char _body[0x1c];
    CEnginePrimitiveManagerOcclusionBase(CEngineSceneArray* scenes);   // 0x00B90270
    void RegisterEnable(const CCharString& name);  // 0x00BACD80
};

class CEnginePrimitiveManagerRepeatedZSprites : public CEnginePrimitiveManagerOcclusionBase { public:   // vtable 0x012A5314
    CVertexShader      VertexShader;               // +0x20
    CPixelShader       PixelShader;                // +0x28
    CPixelShader       PixelShaderNoStipple;       // +0x30
    CVertexBufferWin32 VertexBuffer;               // +0x38
    CIndexBuffer       IndexBuffer;                // +0x6c
    CPODArray<CEngineInternalPrimitiveRepeatedZSprite*> PrimitiveSortList;   // +0x80
    CEngineBillboard   BillboardTexture;           // +0x8c
    long               BillboardTextureHandle;     // +0xa4
    long               ZBufferTextureHandle;       // +0xa8
    long               DynamicTexturePageHandle;   // +0xac
    float              NonStippleAlphaFactor;      // +0xb0
    bool               EnableZSpriteAlpha;         // +0xb4
    float              VerticalControlPointWeightingFactor;   // +0xb8
    float              ControlPointWeightingPower; // +0xbc
    C3DMeshInfo*       CurrentRenderedMesh;        // +0xc0
    CEnginePrimitiveManagerRepeatedZSprites(CEngineSceneArray* scenes);
    virtual ~CEnginePrimitiveManagerRepeatedZSprites();
    void BuildBuffers();                           // 0x00BAA6A0
};

CEnginePrimitiveManagerRepeatedZSprites::CEnginePrimitiveManagerRepeatedZSprites(CEngineSceneArray* scenes)
    : CEnginePrimitiveManagerOcclusionBase(scenes)
{
    VertexShader = NEngineInternal::GPResourceManager->GetVShaderResource((EShaderBankNo)17)->GetVertexShader("VSHADER_ZSPRITE");
    const CShaderResource* pixelResource = NEngineInternal::GPResourceManager->GetPShaderResource();
    PixelShader = pixelResource->GetPixelShader("PSHADER_ZSPRITE");
    PixelShaderNoStipple = pixelResource->GetPixelShader("PSHADER_ZSPRITE_NO_STIPPLE");
    RegisterEnable("EnableZSprites");
    BuildBuffers();
    CGPUScratchBufferTextureManager* textures = &NEngineInternal::GPResourceManager->GetGPUScratchBufferTextureManager();
    DynamicTexturePageHandle = textures->AllocteMemoryPage(0, true);
    CPixelFormat format(8, 8, 8, 8);
    BillboardTextureHandle = textures->AllocateTexture(DynamicTexturePageHandle, C2DExtentsI(256, 256), 1, format, 1, false);
    BillboardTexture.Texture = 0;
    BillboardTexture.Surface = 0;
    CPixelFormat depthFormat(D3DFMT_D24S8);
    ZBufferTextureHandle = textures->AllocateDepthBufferSurface(DynamicTexturePageHandle, C2DExtentsI(256, 256), depthFormat, SURFACE_MULTISAMPLE_NONE);
    CurrentRenderedMesh = 0;
    NonStippleAlphaFactor = 1.2f;
    EnableZSpriteAlpha = true;
    VerticalControlPointWeightingFactor = 5.0f;
    ControlPointWeightingPower = 1.0f;
}