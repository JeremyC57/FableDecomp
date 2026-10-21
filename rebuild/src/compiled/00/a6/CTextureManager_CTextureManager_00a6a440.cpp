// CTextureManager::~CTextureManager() -- uninitialise, then destroy the 16 preallocated texture pools and the CBaseClass base
// Leaf: ??1CTextureManager@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A6A440, 136 bytes. __fastcall; called by the deleting dtor 0x00A6C000 (vtable
//         0x0129DC5C slot 0). RENDER LAYER (texture pools).
// Callees: free 0x00BFEA14 (vector storage); CResourceList::~CResourceList() 0x009FC5C0 (vtable 0x0129C808, the pool base;
//          catalog "Stack<unsigned long,64>::~Stack" is wrong); CBaseClass::~CBaseClass() 0x0099A300 (tail jump); each
//          CManagedTextureData release through its vtable slot 1.
// Shape: vtable store, the inlined Uninitialise() (`if (TextureBuffer) TextureBuffer = 0; Initialised = false;`), then the
//        member array destroyed last-to-first (counter 16 in the frame, `sub edi,54h`): each pool's STLport vector of
//        CIVCountedPointer<CManagedTextureData> releases every element (null-tested release, pointer zeroed), frees its
//        storage, then the pool's CResourceList base dtor runs. The element release goes through an inline
//        `CManagedTextureData::DecRef()` on the POINTEE (`if (--RefCount == 0) Destroy();`): it keeps the object in ecx,
//        reuses the `dec [ecx+4]` flags (`jne`) and leaves `this` in ebx; the release spelled on the holder re-reads [esi].
// Layout: retail CTextureManager (PDB - 0x40, see landed Uninitialise 0x00A696D0): Initialised +0x04, PreallocPools +0x4C
//         (16 x 0x54), FailedAllocations +0x58C, BufferSize +0x5CC, TextureBuffer +0x5D0; CPreallocTexturePool: CResourceList
//         base 0x3C, Count +0x3C, MaxSize +0x40, PoolIndex +0x44, Textures +0x48 (12-byte STLport vector; PDB 16).
extern "C" void __cdecl free(void* p);           // 0x00BFEA14
class CBaseClass {                               // vtable 0x01231710
public:
    virtual void v000();
    CBaseClass();                                // 0x0099A2D0
    ~CBaseClass();                               // 0x0099A300
};
struct CManagedTextureData {
    virtual void v00();
    virtual void Destroy();                      // slot 1
    long RefCount;                               // +0x04
    __forceinline void DecRef() { if (--RefCount == 0) Destroy(); }
};
template <class T> struct CIVCountedPointer {
    T* Object;
    __forceinline ~CIVCountedPointer()
    {
        if (Object) {
            Object->DecRef();
            Object = 0;
        }
    }
};
template <class T> struct CRetailVector {        // STLport vector over a free()-based allocator
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    __forceinline ~CRetailVector()
    {
        T* last = _M_finish;
        for (T* p = _M_start; p != last; ++p)
            p->~T();
        if (_M_start)
            free(_M_start);
    }
};
struct CResourceList {                           // vtable 0x0129C808, 0x3C bytes
    virtual void v00();
    unsigned char _pad04[0x38];
    ~CResourceList();                            // 0x009FC5C0
};
struct __declspec(novtable) CPreallocTexturePool : CResourceList {
    unsigned long Count;                         // +0x3c
    unsigned long MaxSize;                       // +0x40
    long PoolIndex;                              // +0x44
    CRetailVector<CIVCountedPointer<CManagedTextureData> > Textures;   // +0x48
    __forceinline ~CPreallocTexturePool() {}
};
class CTextureManager : public CBaseClass {      // vtable 0x0129DC5C
public:
    bool                 Initialised;            // +0x04
    unsigned long        PoolCount;              // +0x08
    unsigned long        PreallocSizes[16];      // +0x0c
    CPreallocTexturePool PreallocPools[16];      // +0x4c
    unsigned long        FailedAllocations[16];  // +0x58c
    long                 BufferSize;             // +0x5cc
    void*                TextureBuffer;          // +0x5d0
    virtual ~CTextureManager();
    void Uninitialise()
    {
        if (TextureBuffer)
            TextureBuffer = 0;
        Initialised = false;
    }
};
CTextureManager::~CTextureManager()
{
    Uninitialise();
}