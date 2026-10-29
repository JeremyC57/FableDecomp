// CShaderResource::~CShaderResource() -- store this class's vtable, close the shader bank and drop the render manager, destroy the embedded CVertexShaderInput, free the shader array, then the CBase destructor
// Leaf: ??1CShaderResource
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A5D800, length 108 bytes (+ int3 padding).
// Label correction: catalogued as CPatchTesselationEdgeStrip::~CPatchTesselationEdgeStrip (propagated); the only caller
// is the scalar deleting destructor @ 0x00A5EBE0, slot 0 of CShaderResource's vtable 0x0129D580 (the stored vtable).
// Callees: CShaderResource::CloseShaderBank @ 0x00A5D750, free @ 0x00BFEA14 (x3), the CStreamInfo range destroy
// @ 0x00A5E900 (`__fastcall` first/last, the iterator tag by reference at `lea [esp+0Bh]`), CBaseClass::~CBaseClass
// @ 0x0099A2E0 (the embedded input's base), CBaseClassNonCopyable::~CBaseClassNonCopyable @ 0x0099A300.
// Layout: Ego_r.pdb CShaderResource with the retail 12-byte CArray (PDB 16): ShaderBank +0x04, Shaders +0x08,
// ShaderRenderManager +0x14, VertexShaderInput +0x18 (0x1c bytes: vptr, DataStreams +0x1c, D3DDeclaration +0x28),
// VertexShaderDeclaration +0x34, VertexFormat +0x38.
// Shape: the body is the inline Close() (an out-of-line copy sits at 0x00A5D7A0: CloseShaderBank, ShaderRenderManager = 0);
// the embedded CVertexShaderInput's destructor is inlined here (the novtable input as the landed 0x00A5EB70: declaration
// array freed, stream array destroyed and freed, then its base destructor), then the Shaders array freed and the base
// destructor called; `ret`.
extern "C" void __cdecl free(void* block);         // 0x00BFEA14

class CBaseClass { public:                                // vtable 0x01231710
    virtual void v00();
    virtual ~CBaseClass();                                 // 0x0099A2E0
};
class CBaseClassNonCopyable {
protected:
    virtual ~CBaseClassNonCopyable();                                      // 0x0099A300
};

struct SNonscalarPtrIteratorTag { SNonscalarPtrIteratorTag() { } };

struct CStreamInfo;
void __fastcall DestroyRange(CStreamInfo* first, CStreamInfo* last, const SNonscalarPtrIteratorTag& tag);   // 0x00A5E900

struct CStreamInfoArray {
    CStreamInfo* m_pFirst;
    CStreamInfo* m_pLast;
    CStreamInfo* m_pEnd;
    ~CStreamInfoArray()
    {
        DestroyRange(m_pFirst, m_pLast, SNonscalarPtrIteratorTag());
        if (m_pFirst != 0)
            free(m_pFirst);
    }
};
template <class T>
struct CPodArray {
    T* m_pFirst;
    T* m_pLast;
    T* m_pEnd;
    ~CPodArray() { if (m_pFirst) free(m_pFirst); }
};

struct D3DVERTEXELEMENT9;
struct __declspec(novtable) CVertexShaderInput : CBaseClass {   // vtable 0x0129EA08
    CStreamInfoArray              DataStreams;     // +0x04
    CPodArray<D3DVERTEXELEMENT9>  D3DDeclaration;  // +0x10
    virtual void v00();
};

class CShaderDataBank;
class CShaderBase;
class CShaderRenderManager;
class CShaderResource : public CBaseClassNonCopyable { public:                   // vtable 0x0129D580, retail 0x3c bytes
    CShaderDataBank*          ShaderBank;          // +0x04
    CPodArray<CShaderBase*>   Shaders;             // +0x08
    CShaderRenderManager*     ShaderRenderManager; // +0x14
    CVertexShaderInput        VertexShaderInput;   // +0x18
    D3DVERTEXELEMENT9*        VertexShaderDeclaration;   // +0x34
    long                      VertexFormat;        // +0x38
    void CloseShaderBank();                        // 0x00A5D750
    void Close()                                   // out-of-line copy @ 0x00A5D7A0
    {
        CloseShaderBank();
        ShaderRenderManager = 0;
    }
    ~CShaderResource();
};

CShaderResource::~CShaderResource()
{
    Close();
}