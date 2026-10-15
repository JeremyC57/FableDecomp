// CHandleBasedMemoryPool::`scalar deleting destructor'(unsigned int) -- stores the CHandleBasedMemoryPool vtable, uninitialises the pool, destroys the name string (+0x448) and frees the handle array's block (+0x438), runs CBase::~CBase and frees on bit 0
// Leaf: ??_GCHandleBasedMemoryPool
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A03AA0, length 73 bytes (vtable 0x0129C964 slot 0).
// Callees: CHandleBasedMemoryPool::Uninitialise @ 0x00A02190, CCharString::~CCharString @ 0x0099EAE0
// (manifest "_Cons_val<...>", propagated), free @ 0x00BFEA14, CBase::~CBase @ 0x0099A300,
// operator delete @ 0x00BFE9BC.
// The catalog label "CThingTrackNode::vector_deleting_destructor" is propagated: the only reference is
// slot 0 of the CHandleBasedMemoryPool vtable 0x0129C964.
// Layout: engine/CHandleBasedMemoryPool.h gives Handles (CArray<CHandleInfo>) 0x10 bytes at +0x438 and the
// name at +0x44c; the retail CArray is 0x0c bytes (wave 23 rule), so FirstUnusedHandle sits at +0x444 and
// the name at +0x448 (the destructor's `lea ecx,[esi+448h]`). Modelled locally (the header is a flat
// struct) as a polymorphic CBase-derived class.
// Shape: the destructor body calls Uninitialise, then the members die in reverse order: the string, then
// the CArray's inline destructor (`if (data) free(data)`), then the base.
extern "C" void __cdecl free(void* block);         // 0x00BFEA14

struct CCharString {
    void* m_pData;
    CCharString();                                 // 0x0099E4B0
    ~CCharString();                                // 0x0099EAE0
};

struct CBase {                                     // root polymorphic base
    CBase();
    virtual ~CBase();                              // 0x0099A300
};

struct CHandleInfoArray {                          // CArray<CHandleBasedMemoryPool::CHandleInfo>, 0x0c bytes
    void* m_pData;
    long  m_size;
    long  m_capacity;
    CHandleInfoArray() : m_pData(0), m_size(0), m_capacity(0) { }
    ~CHandleInfoArray() { if (m_pData) free(m_pData); }
};

struct CHandleBasedMemoryPool : CBase {            // vtable 0x0129C964
    unsigned char    _pad_0x04[0x434];             // +0x04 pool state and FreeLists[256] (+0x38)
    CHandleInfoArray Handles;                      // +0x438
    long             FirstUnusedHandle;            // +0x444
    CCharString      Name;                         // +0x448
    unsigned char    CriticalSection[0x18];        // +0x44c
    void Uninitialise();                           // 0x00A02190
    CHandleBasedMemoryPool() { }
    __forceinline virtual ~CHandleBasedMemoryPool() { Uninitialise(); }
};

CHandleBasedMemoryPool* Create_CHandleBasedMemoryPool()
{
    return new CHandleBasedMemoryPool;
}
