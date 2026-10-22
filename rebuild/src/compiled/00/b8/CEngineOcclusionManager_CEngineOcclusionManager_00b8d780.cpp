// CEngineOcclusionManager::CEngineOcclusionManager() -- constructs the occlusion manager: every occluder slot marked free (-1), both entry buffers zeroed and the frame's entry pointers aimed at them, then the flare-test vertex shader looked up, the occlusion buffers initialised and the occlusion-box vertex / pixel shaders looked up
// Leaf: ??0CEngineOcclusionManager
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B8D780, length 767 bytes (plain `ret`, no EH frame).
// Callees: CEngineComponent::CEngineComponent @ 0x00B59710 (manifest "CNavigationLayer", propagated), CVertexShader::CVertexShader
//   @ 0x00A62FB0, CBase::CBase @ 0x0099A2F0 (unit-box vertex buffer root), CIndexBuffer::CIndexBuffer @ 0x00AC1EC0,
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
struct CCharString {
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                // 0x0099EAE0
    void* m_pData;
};

struct CVertexShaderData {
    virtual ~CVertexShaderData();                  // slot 0
    void*         VertexDeclaration;
    void*         VertexShader;
    const void*   FormatDecl;
    long          ReferenceCount;                  // +0x10
    long RemoveReference() { return --ReferenceCount; }
    void ReleaseReference() { if (--ReferenceCount == 0) delete this; }
};
struct CPixelShaderData {
    virtual ~CPixelShaderData();                   // slot 0
    void*         PixelShader;
    long          ReferenceCount;                  // +0x08
    long RemoveReference() { return --ReferenceCount; }
    void ReleaseReference() { if (--ReferenceCount == 0) delete this; }
};

struct CShaderBase {                               // vtable 0x0129A030
    virtual ~CShaderBase() { }
};
struct CVertexShader : CShaderBase {               // vtable 0x0129B3D0, 8 bytes
    CVertexShaderData* Data;
    CVertexShader();                               // 0x00A62FB0
    CVertexShader(const CVertexShader& other) : Data(other.Data) { if (Data) ++Data->ReferenceCount; }
    CVertexShader& operator=(const CVertexShader& other)
    {
        if (Data) Data->ReleaseReference();
        Data = other.Data;
        ++Data->ReferenceCount;
        return *this;
    }
    virtual ~CVertexShader() { if (Data) { CVertexShaderData* current = Data; if (!current->RemoveReference()) delete current; } }
};
struct CPixelShader : CShaderBase {                // vtable 0x0129A054, 8 bytes
    CPixelShaderData* Data;
    CPixelShader();                                // 0x00A5EC00
    CPixelShader(const CPixelShader& other) : Data(other.Data) { if (Data) ++Data->ReferenceCount; }
    CPixelShader& operator=(const CPixelShader& other)
    {
        if (Data) { Data->ReleaseReference(); Data = 0; }
        Data = other.Data;
        ++Data->ReferenceCount;
        return *this;
    }
    virtual ~CPixelShader() { if (Data) { CPixelShaderData* current = Data; if (!current->RemoveReference()) delete current; } }
};

struct CShaderResource {
    CVertexShader GetVertexShader(const CCharString& name);   // 0x00A5D5F0
    CPixelShader  GetPixelShader(const CCharString& name);    // 0x00A5D720
};
struct CEngineData {
    unsigned char    _pad0000[0x29c8];
    CShaderResource* StaticUnlitShaderResource;    // +0x29C8
    unsigned char    _pad29cc[0x2a60 - 0x29cc];
    CShaderResource* OcclusionBoxShaderResource;   // +0x2A60
    unsigned char    _pad2a64[0x2a98 - 0x2a64];
    CShaderResource* PixelShaderResource;          // +0x2A98
    CShaderResource* GetStaticUnlitShaderResource() { return StaticUnlitShaderResource; }
    CShaderResource* GetOcclusionBoxShaderResource() { return OcclusionBoxShaderResource; }
    CShaderResource* GetPixelShaderResource() { return PixelShaderResource; }
};
extern CEngineData* g_pEngineData;                 // 0x01436E98

struct CBase {
    virtual void VSlot0();
    CBase();                                       // 0x0099A2F0
};
struct IDirect3DVertexBuffer9;
struct CVertexCompressionParams {
    float PosScaleX, PosScaleY, PosScaleZ, PosScaleW;
    float PosBiasX, PosBiasY, PosBiasZ, PosBiasW;
    CVertexCompressionParams()
        : PosScaleX(1.0f), PosScaleY(1.0f), PosScaleZ(1.0f), PosScaleW(1.0f),
          PosBiasX(0.0f), PosBiasY(0.0f), PosBiasZ(0.0f), PosBiasW(0.0f) { }
};
struct CVertexBufferBase : CBase {                 // 0x34 bytes
    IDirect3DVertexBuffer9*  PVertexBuffer;        // +0x04
    unsigned long            Length;               // +0x08
    long                     Type;                 // +0x0c
    unsigned char            Stride;               // +0x10
    CVertexCompressionParams CompressionParams;    // +0x14
    CVertexBufferBase() : Length(0), Type(0), CompressionParams() { }
};
struct CVertexBufferWin32 : CVertexBufferBase {    // vtable 0x0129BA20
    virtual void VSlot1();
    CVertexBufferWin32()
    {
        PVertexBuffer = 0;
        Stride = 0;
    }
};
struct CIndexBuffer {                              // 0x14 bytes
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
template <class T> struct CCountedPointer { T* Object; void* Info; };

struct CEngineComponent {                          // 0x0C bytes
    virtual ~CEngineComponent();
    void** m_ppOwnerLink;                          // +0x04
    void*  m_pOwner;                               // +0x08
    CEngineComponent();                            // 0x00B59710
};

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

struct CEngineOcclusionManager : CEngineComponent {   // vtable 0x012A3CD0
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
    void InitializeOcclusionBuffers();             // 0x00B8D0D0
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
    VertexShaderFlareTest = g_pEngineData->GetStaticUnlitShaderResource()->GetVertexShader("VSHADER_STATIC_UNLIT");
    InitializeOcclusionBuffers();
    OcclusionBoxVShader = g_pEngineData->GetOcclusionBoxShaderResource()->GetVertexShader("VSHADER_OCCLUSION_BOX");
    OcclusionBoxPixelShader = g_pEngineData->GetPixelShaderResource()->GetPixelShader("PSHADER_DIFFUSE_ONLY");
}