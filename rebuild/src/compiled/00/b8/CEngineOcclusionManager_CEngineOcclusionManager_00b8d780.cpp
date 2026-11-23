// CEngineOcclusionManager::CEngineOcclusionManager() -- constructs the occlusion manager: every occluder slot marked free (-1), both entry buffers zeroed and the frame's entry pointers aimed at them, then the flare-test vertex shader looked up, the occlusion buffers initialised and the occlusion-box vertex / pixel shaders looked up
// Leaf: ??0CEngineOcclusionManager
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8). Also: the local CCountedPointer members Object->Data are the header CCountedPointer's (same offsets).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B8D780, length 767 bytes (plain `ret`, no EH frame).
// Callees: CEngineComponent::CEngineComponent @ 0x00B59710 (manifest "CNavigationLayer", propagated), CVertexShader::CVertexShader
//   @ 0x00A62FB0, CBaseClassNonCopyable::CBaseClassNonCopyable @ 0x0099A2F0 (unit-box vertex buffer root), CIndexBuffer::CIndexBuffer @ 0x00AC1EC0,
//   CPixelShader::CPixelShader @ 0x00A5EC00, CCharString(const char*, long) @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0,
//   CShaderResource::GetVertexShader @ 0x00A5D5F0 / GetPixelShader @ 0x00A5D720, InitializeOcclusionBuffers @ 0x00B8D0D0.
// Globals: the engine data @ 0x01436E98; shader resources at +0x29C8 (static unlit), +0x2A60 (occlusion box), +0x2A98 (pixel).
// Layout: engine/CEngineOcclusionManager.h names the members, but retail's vectors are 12 bytes (PDB 16), so from FlareTests on
//   the members sit lower (FlareTests +0x24024 ... FlareVertexBuffers +0x24054, VertexShaderFlareTest +0x24060, UnitBoxVB +0x24068,
//   UnitBoxIB +0x2409C, OcclusionBoxVShader +0x240B0, OcclusionBoxPixelShader +0x240B8), as the landed destructor 0x00B8DA80 models.
// Shape: the slot / entry arrays are arrays of classes with inline constructors (`Handle = -1`; both words zeroed), so each gets its
//   own counted loop; the body then memsets the slot array to 0xFF (`or eax,-1 ; rep stosd` through the kept edi); the shader
//   lookups follow the landed 2D sprite constructor 0x00BAD040 (implicit CCharString temporaries, inline shader destructors) and
//   every resource is read through an INLINE accessor (a direct member read loads the global early / into another register).
#include <string.h>
// NF46 (round 7): the 0x01436E98 global is NEngineInternal::GPResourceManager (Ego_r.pdb / FableWin.pdb
// ?GPResourceManager@NEngineInternal@@3PAVCEngineResourceManager@@A); the vertex shader resources are
// CEngineResourceManager::VShaderResources[bank] (retail +0x29C0 + 8*bank, FableWin.pdb inline GetVShaderResource) and the pixel one
// ShaderResourcePixel (+0x2A98, GetPShaderResource). Shader classes from the shared headers (CShaderResource::GetVertexShader /
// GetPixelShader const, Ego_r.pdb / FableWin.pdb); were a local CEngineData / g_pEngineData stand-in and local shader models.
#define FABLE_RETAIL_CENGINECOMPONENT_PROTECTED
#define FABLE_RETAIL_CENGINERESOURCEMANAGER
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CVERTEXSHADER
#include "fable_string.h"
#include "engine/CVertexShader.h"
#include "engine/CPixelShader.h"
#include "engine/CEngineResourceManager.h"
#include "engine/CShaderResource.h"
namespace NEngineInternal { extern CEngineResourceManager* GPResourceManager; }   // 0x01436E98

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
template <class T> struct vector : _Vector_base<T> {   // retail 12-byte STLport vector
    vector(const allocator<T>& a = allocator<T>()) : _Vector_base<T>(a) { }
};
template <class A, class B> struct pair { A first; B second; };
#include "engine/CCountedPointer.h"


struct COccluderSlot {                             // 8 bytes
    long Handle;
    long RetrievalID;
    COccluderSlot() : Handle(-1) { }
};
struct COccluderEntry {                            // 8 bytes
    unsigned long Handle;
    unsigned long Result;
    COccluderEntry() : Handle(0), Result(0) { }
};
struct COccluderFlareEntry { unsigned long Handle; unsigned long Result; };

class CEngineOcclusionManager : public CEngineComponent { public:   // vtable 0x012A3CD0
    COccluderSlot      OccluderSlots[2048];        // +0x0c
    unsigned long      NextSlot;                   // +0x400c
    unsigned long      CurrentFrame;               // +0x4010
    COccluderEntry*    Entries;                    // +0x4014
    COccluderEntry*    PrevFrameEntries;           // +0x4018
    unsigned long      EntriesSize;                // +0x401c
    unsigned long      PrevFrameEntriesSize;       // +0x4020
    COccluderEntry     EntriesBuffer1[8192];       // +0x4024
    COccluderEntry     EntriesBuffer2[8192];       // +0x14024
    vector<COccluderFlareEntry> FlareTests;        // +0x24024
    vector<COccluderFlareEntry> PrevFrameFlareTests;   // +0x24030
    vector<pair<unsigned long, unsigned long> > HandleMapper;            // +0x2403c
    vector<pair<unsigned long, unsigned long> > PrevFrameHandleMapper;   // +0x24048
    vector<pair<unsigned long, CCountedPointer<CVertexBufferWin32> > > FlareVertexBuffers;   // +0x24054
    CVertexShader      VertexShaderFlareTest;      // +0x24060
    CVertexBufferWin32 UnitBoxVB;                  // +0x24068
    CIndexBuffer       UnitBoxIB;                  // +0x2409c
    CVertexShader      OcclusionBoxVShader;        // +0x240b0
    CPixelShader       OcclusionBoxPixelShader;    // +0x240b8
    CEngineOcclusionManager();
    virtual ~CEngineOcclusionManager();
    private: void BuildUnitBox(); public:             // 0x00B8D0D0; NF296: PDB name, was InitializeOcclusionBuffers
};

CEngineOcclusionManager::CEngineOcclusionManager()
{
    memset(OccluderSlots, 0xff, sizeof(OccluderSlots));
    NextSlot = 0;
    CurrentFrame = 0;
    Entries = EntriesBuffer1;
    PrevFrameEntries = EntriesBuffer2;
    EntriesSize = 0;
    PrevFrameEntriesSize = 0;
    VertexShaderFlareTest = NEngineInternal::GPResourceManager->GetVShaderResource((EShaderBankNo)1)->GetVertexShader("VSHADER_STATIC_UNLIT");
    BuildUnitBox();
    OcclusionBoxVShader = NEngineInternal::GPResourceManager->GetVShaderResource((EShaderBankNo)20)->GetVertexShader("VSHADER_OCCLUSION_BOX");
    OcclusionBoxPixelShader = NEngineInternal::GPResourceManager->GetPShaderResource()->GetPixelShader("PSHADER_DIFFUSE_ONLY");
}