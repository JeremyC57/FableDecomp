// CTextureManager::CTextureManager() -- the texture manager starts uninitialised with no pools: the sixteen prealloc pools are built in place (resource list base, empty texture arrays, pool index -1), the failure counters, buffer size and buffer pointer cleared
// Leaf: ??0CTextureManager
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A6A360, 219 bytes (+5 int3). __fastcall, `ret`. CTextureManager vtable 0x0129DC5C,
//         CPreallocTexturePool vtable 0x0129DC50.
// Callees: CBaseClassNonCopyable::CBaseClassNonCopyable() 0x0099A2F0, CResourceList::CResourceList() 0x009FC570 (the pool's base).
// Shape: Initialised and PoolCount stored through the shared zero register (ebx) right after the vtable; the member array
//        is built by the inline counted loop (a frame word holds the count of 16, the element pointers step by 0x54): each
//        pool runs the out-of-line base constructor, stores its vtable, then Count, MaxSize, PoolIndex = -1 and the STLport
//        vector (start, finish, end-of-storage through its allocator proxy -- the proxy word is written first and again, which
//        is why the compiler bases the stores on +0x50); BufferSize and TextureBuffer take the zero register, the sixteen
//        FailedAllocations are a body loop the compiler unrolled through a fresh zero (eax).
// Layout: engine/CTextureManager.h with the retail 0x54-byte pool (PDB 0x58: the CArray member is 12 bytes in retail):
//         Initialised +0x04, PoolCount +0x08, PreallocSizes +0x0c, PreallocPools +0x4c, FailedAllocations +0x58c, BufferSize
//         +0x5cc, TextureBuffer +0x5d0. CPreallocTexturePool: CResourceList 0x3c, Count +0x3c, MaxSize +0x40, PoolIndex +0x44,
//         Textures +0x48.
struct CBaseClassNonCopyable {
    virtual void v00();
    CBaseClassNonCopyable();                   // 0x0099A2F0
};
struct CResourceList {                         // vtable 0x0129C808, 0x3c bytes
    virtual void v00();
    unsigned char _body[0x3c - 4];
    CResourceList();                           // 0x009FC570
};
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E (folded away: the count is the constant 0)
template <class T> struct CAllocator {         // STLport allocator<T>: empty
    CAllocator() { }
    T* allocate(unsigned long count) { return count != 0 ? static_cast<T*>(malloc(count * sizeof(T))) : 0; }
};
template <class T> struct CAllocProxy : CAllocator<T> {    // STLport _STLP_alloc_proxy<T*, T, allocator<T> >
    T* m_data;
    CAllocProxy(const CAllocator<T>& alloc, T* p) : CAllocator<T>(alloc), m_data(p) { }
};
template <class T> struct CVectorBase {        // STLport _Vector_base<T, allocator<T> >
    T*             _M_start;
    T*             _M_finish;
    CAllocProxy<T> _M_end_of_storage;
    CVectorBase(unsigned long count, const CAllocator<T>& alloc)
        : _M_start(0), _M_finish(0), _M_end_of_storage(alloc, 0)
    {
        _M_start = _M_end_of_storage.allocate(count);
        _M_finish = _M_start;
        _M_end_of_storage.m_data = _M_start + count;
    }
};
template <class T> struct CArray : CVectorBase<T> {   // retail CArray<T> = STLport vector<T> (12 bytes): vector(n) with n = 0
    explicit CArray(unsigned long count) : CVectorBase<T>(count, CAllocator<T>()) { this->_M_finish = this->_M_start; }
};
struct CManagedTextureData;
struct CIVCountedPointer { CManagedTextureData* Object; };
struct CPreallocTexturePool : CResourceList {  // vtable 0x0129DC50, retail 0x54 bytes
    unsigned long                 Count;        // +0x3c
    unsigned long                 MaxSize;      // +0x40
    long                          PoolIndex;    // +0x44
    CArray<CIVCountedPointer>     Textures;     // +0x48
    virtual void v00();
    CPreallocTexturePool() : CResourceList(), Count(0), MaxSize(0), PoolIndex(-1), Textures(0) { }
};
struct CTextureManager : CBaseClassNonCopyable {   // vtable 0x0129DC5C
    bool                 Initialised;           // +0x04
    unsigned long        PoolCount;             // +0x08
    unsigned long        PreallocSizes[16];     // +0x0c
    CPreallocTexturePool PreallocPools[16];     // +0x4c
    unsigned long        FailedAllocations[16]; // +0x58c
    long                 BufferSize;            // +0x5cc
    void*                TextureBuffer;         // +0x5d0
    virtual void v00();
    CTextureManager();
};
CTextureManager::CTextureManager()
    : CBaseClassNonCopyable(), Initialised(false), PoolCount(0), BufferSize(0), TextureBuffer(0)
{
    for (long i = 0; i < 16; ++i)
        FailedAllocations[i] = 0;
}
