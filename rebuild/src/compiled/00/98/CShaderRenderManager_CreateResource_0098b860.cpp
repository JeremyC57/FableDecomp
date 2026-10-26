// CShaderRenderManager::CreateResource() -- new CShaderResource, wrap it in a counted pointer, initialise it against this manager, register it in ResourceManagers and return it
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0098B860, length 208 bytes (`ret 4`: hidden result pointer).
// GRAPHICS LAYER (replacement work later): true class CShaderRenderManager (vtable 0x0129A080, per the landed
// Uninitialise 0x0098A7A0); Ego_r.pdb ?CreateResource@CShaderRenderManager@@QAE?AV?$CCountedPointer@VCShaderResource@@@@XZ.
// The catalog label "C3DAnimFile::C3DAnimFile" is wrong (no vtable store; a by-value counted-pointer return).
// Callers: 0x009D9B80, 0x00B3B60A, 0x00B3B70A, 0x00B3D698 (ecx = the singleton 0x013BC470).
// Callees: operator new @ 0x00BFEA1A (0x3c-byte CShaderResource, 0x0C-byte count block), CShaderResource::CShaderResource
// @ 0x00A5D870, CShaderResource::Initialise(CShaderRenderManager&) @ 0x00A5D4D0 (catalog "CObservable::AddExclusiveObserver"),
// the counted pointer's __fastcall DeleteData hook @ 0x0098BA10, ResourceManagers' _M_insert_overflow @ 0x0098DC30,
// operator delete @ 0x00BFE9BC.
// Retail offsets: ResourceManagers (CArray<CCountedPointer<CShaderResource> >, 12 bytes, 8-byte elements) +0x3a8
// (PDB +0x3b8: PDB containers are 4 bytes wider).
// Shape: `CCountedPointer<T> p(new T); ... return p;` (no NRVO: merged inc/dec copy-out), the inline STLport
// push_back (placement copy + `add [v+4],8`, else _M_insert_overflow(pos, x, __false_type(), 1, true)).
// Leaf: CreateResource@CShaderRenderManager
struct SCountBlock {
    long m_references;                              // +0
    void (__fastcall* m_pfnDestroy)(void*);         // +4
    void* m_pObject;                                // +8
    SCountBlock(void (__fastcall* destroy)(void*), void* object) : m_references(1), m_pfnDestroy(destroy), m_pObject(object) {}
};

template<class T> class CCountedPointer { public:
    T* Object;                                      // +0
    SCountBlock* Count;                             // +4
    static void __fastcall DeleteData(void* object);   // 0x0098BA10
    CCountedPointer(T* object) : Object(object), Count(object ? new SCountBlock(DeleteData, object) : 0) {}
    CCountedPointer(const CCountedPointer& other) : Object(other.Object), Count(other.Count) { if (Count) ++Count->m_references; }
    ~CCountedPointer()
    {
        if (Count) {
            if (--Count->m_references == 0) {
                Count->m_pfnDestroy(Count->m_pObject);
                delete Count;
            }
        }
    }
    T* operator->() const { return Object; }
};

class CShaderRenderManager;
class CShaderResource { public:                            // 0x3c bytes
    CShaderResource();                              // 0x00A5D870
    void Initialise(CShaderRenderManager& manager); // 0x00A5D4D0
    unsigned char _body[0x3c];
};
void* FindRef_CShaderResource_Initialise_1();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it only writes a word through the reference parameter inline, so
// VC7.1 knows the caller's address-taken local does not escape the call.
#pragma auto_inline(off)
__declspec(noinline) void CShaderResource::Initialise(CShaderRenderManager& manager)
{
    void* p = FindRef_CShaderResource_Initialise_1();
    if (!p)
        return;
    *(void**)&manager = p;
    
}
#pragma auto_inline(on)

struct __false_type { __false_type() {} };          // empty tag with a user constructor (no byte store)
inline void* operator new(unsigned int, void* where) { return where; }

struct CShaderResourceArray {                       // retail CArray<CCountedPointer<CShaderResource> > (12 bytes)
    CCountedPointer<CShaderResource>* m_start;
    CCountedPointer<CShaderResource>* m_finish;
    CCountedPointer<CShaderResource>* m_end_of_storage;
    void _M_insert_overflow(CCountedPointer<CShaderResource>* position, const CCountedPointer<CShaderResource>& x,
                            const __false_type&, unsigned long fill_len, bool atend);   // 0x0098DC30
    void push_back(const CCountedPointer<CShaderResource>& x)
    {
        if (m_finish != m_end_of_storage) {
            new (m_finish) CCountedPointer<CShaderResource>(x);
            ++m_finish;
        }
        else
            _M_insert_overflow(m_finish, x, __false_type(), 1, true);
    }
};
void* FindRef_CShaderResourceArray__M_insert_overflow_0();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it only writes a word through the reference parameter inline, so
// VC7.1 knows the caller's address-taken local does not escape the call.
#pragma auto_inline(off)
__declspec(noinline) void CShaderResourceArray::_M_insert_overflow(CCountedPointer<CShaderResource>* position, const CCountedPointer<CShaderResource>& x, const __false_type& p2, unsigned long fill_len, bool atend)
{
    void* p = FindRef_CShaderResourceArray__M_insert_overflow_0();
    if (!p)
        return;
    *(void**)position = p;
    if (*(void**)&x != p)
        return;
}
#pragma auto_inline(on)

class CShaderRenderManager { public:
    void* __vftable;                                // +0x000
    unsigned char _pad004[0x3a4];
    CShaderResourceArray ResourceManagers;          // +0x3a8
    CCountedPointer<CShaderResource> CreateResource();
};

CCountedPointer<CShaderResource> CShaderRenderManager::CreateResource()
{
    CCountedPointer<CShaderResource> resource(new CShaderResource);
    resource->Initialise(*this);
    ResourceManagers.push_back(resource);
    return resource;
}