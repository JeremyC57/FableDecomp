// CEngineOcclusionManager::~CEngineOcclusionManager() -- release the flare-test vertex shader's data, then destroy the members in reverse: the occlusion-box pixel/vertex shaders, the unit-box index and vertex buffers, the flare-test shader, the flare vertex-buffer map (range destroy + free), the four handle/flare arrays (free only), then the CEngineComponent base
// Leaf: ??1CEngineOcclusionManager
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B8DA80, length 352 bytes (with int3 padding; body 0x158).
// Label correction: catalogued "~CEnginePrimitiveManagerParticleLineGroup" (propagated); the vtable store 0x012A3CD0 is
//   CEngineOcclusionManager (rtti-vtables.tsv, slot 1 0x00B8DBE0) and the single caller is its deleting destructor @ 0x00B8F933.
// Callees: CIndexBuffer::Uninitialise @ 0x00AC1EE0, CVertexBufferWin32::Uninitialise @ 0x00A63200, ~CBaseClassNonCopyable
//   @ 0x0099A300, the `__fastcall` _Destroy_range of pair<unsigned long, CCountedPointer<CVertexBufferWin32>> @ 0x00B8F470
//   (first ecx, last edx, the allocator by const reference; manifest names another instantiation), free @ 0x00BFEA14,
//   CEngineComponent::~CEngineComponent @ 0x00B59730 (landed).
// Layout: Ego_r.pdb CEngineOcclusionManager with 12-byte retail CArray/vector members (PDB 16): FlareTests +0x24024,
//   PrevFrameFlareTests +0x24030, HandleMapper +0x2403c, PrevFrameHandleMapper +0x24048, FlareVertexBuffers +0x24054,
//   VertexShaderFlareTest +0x24060, UnitBoxVB +0x24068 (0x34), UnitBoxIB +0x2409c (0x14), OcclusionBoxVShader +0x240b0,
//   OcclusionBoxPixelShader +0x240b8. engine/CEngineOcclusionManager.h carries the PDB offsets, so the class is modelled locally.
// Shape: the user destructor body is an inline shader Release() (null-tested release, then `Data = 0`); member destructors
//   follow inline in reverse order; the map's destroy takes an empty allocator temporary in the top byte of the `push ecx`
//   slot; POD arrays only free their storage; the base destructor is called (the frame word blocks the tail jump).
extern "C" void __cdecl free(void* block);              // 0x00BFEA14

class CBaseClassNonCopyable {
public:
    virtual ~CBaseClassNonCopyable();                   // 0x0099A300
};

struct CPixelShaderData {
    virtual ~CPixelShaderData();                   // slot 0: the deleting destructor is called with 1
    void* m_pShader;                               // +0x04
    long  m_refCount;                              // +0x08
};
struct CVertexShaderData {
    virtual ~CVertexShaderData();                  // slot 0
    unsigned char _pad04[0xc];
    long  m_refCount;                              // +0x10
};

class CShaderBase { public:                               // vtable 0x0129A030
    virtual ~CShaderBase() { }
};
class CPixelShader : public CShaderBase { public:                // vtable 0x0129A054
    CPixelShaderData* Data;                        // +0x04
    __forceinline virtual ~CPixelShader()
    {
        CPixelShaderData* data = Data;
        if (data) {
            if (--data->m_refCount == 0)
                delete data;
        }
    }
};
class CVertexShader : public CShaderBase { public:               // vtable 0x0129B3D0
    CVertexShaderData* Data;                       // +0x04
    __forceinline void Release()
    {
        CVertexShaderData* data = Data;
        if (data) {
            if (--data->m_refCount == 0)
                delete data;
        }
        Data = 0;
    }
    __forceinline virtual ~CVertexShader()
    {
        CVertexShaderData* data = Data;
        if (data) {
            if (--data->m_refCount == 0)
                delete data;
        }
    }
};

class CVertexBufferWin32 : public CBaseClassNonCopyable { public:   // vtable 0x0129BA20, 0x34 bytes
    unsigned char _body[0x30];
    void Uninitialise();                           // 0x00A63200
    __forceinline virtual ~CVertexBufferWin32() { Uninitialise(); }
};

class CMovableResource { public:                          // vtable 0x0129B860
    virtual void v00();
    ~CMovableResource() { }                        // inline: the vtable store
};
struct IDirect3DIndexBuffer9;
class CIndexBuffer : public CMovableResource { public:           // vtable 0x0129EF2C, 0x14 bytes
    IDirect3DIndexBuffer9* PD3DIndexBuffer;        // +0x04
    unsigned long          ByteLength;             // +0x08
    long                   PrimitiveType;          // +0x0c
    void*                  PAllocatedBuffer;       // +0x10
    void Uninitialise();                           // 0x00AC1EE0
    __forceinline virtual ~CIndexBuffer() { Uninitialise(); }
};

template <class T> struct CPODArray {              // CArray<T> / vector<T> of a trivial T, 12 bytes
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    __forceinline ~CPODArray() { if (_M_start) free(_M_start); }
};

template <class T> struct allocator { allocator() {} };
template <class T> void __fastcall _Destroy_range(T* first, T* last, const allocator<T>& alloc);   // 0x00B8F470 for this T

template <class T> class CArray { public:                 // vector<T> of a non-trivial T, 12 bytes
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    __forceinline ~CArray()
    {
        _Destroy_range(_M_start, _M_finish, allocator<T>());
        if (_M_start)
            free(_M_start);
    }
};

template <class T> class CCountedPointer { public: T* Object; void* Info; };
template <class A, class B> struct pair { A first; B second; };

class CEngineComponent { public:
    virtual void v00();
    void** m_ppOwnerLink;                          // +0x04
    ~CEngineComponent();                           // 0x00B59730
};

struct COccluderFlareEntry { unsigned long Handle; unsigned long Result; };

class CEngineOcclusionManager : public CEngineComponent { public:   // vtable 0x012A3CD0
    unsigned char                    _pad_0x08[0x24024 - 0x08];   // occluder slots, entry buffers
    CPODArray<COccluderFlareEntry>   FlareTests;                  // +0x24024
    CPODArray<COccluderFlareEntry>   PrevFrameFlareTests;         // +0x24030
    CPODArray<pair<unsigned long, unsigned long> > HandleMapper;  // +0x2403c
    CPODArray<pair<unsigned long, unsigned long> > PrevFrameHandleMapper;   // +0x24048
    CArray<pair<unsigned long, CCountedPointer<CVertexBufferWin32> > > FlareVertexBuffers;   // +0x24054
    CVertexShader                    VertexShaderFlareTest;       // +0x24060
    CVertexBufferWin32               UnitBoxVB;                   // +0x24068
    CIndexBuffer                     UnitBoxIB;                   // +0x2409c
    CVertexShader                    OcclusionBoxVShader;         // +0x240b0
    CPixelShader                     OcclusionBoxPixelShader;     // +0x240b8
    virtual void v00();
    ~CEngineOcclusionManager();
};

CEngineOcclusionManager::~CEngineOcclusionManager()
{
    VertexShaderFlareTest.Release();
}