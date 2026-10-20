#include <new>
// Behaviour test for CTextureManager::CTextureManager (0x00A6A360): the manager starts uninitialised with no
// pools; each of the sixteen prealloc pools gets its resource-list base, zero count and max size, pool index
// -1 and an empty texture vector; failure counters, buffer size and buffer pointer are zero.
#include <cstdio>
#include <cstdlib>
static int g_baseCtors = 0, g_listCtors = 0;
struct CBaseClassNonCopyable { virtual void v00() {} CBaseClassNonCopyable() { ++g_baseCtors; } };
struct CResourceList { virtual void v00() {} unsigned char _body[0x3c - 4]; CResourceList() { ++g_listCtors; _body[0] = 0x5a; } };
void* malloc_spy(unsigned int size) { std::printf("unexpected allocation of %u\n", size); std::abort(); return 0; }
template <class T> struct CAllocator { CAllocator() { } T* allocate(unsigned long count) { return count != 0 ? static_cast<T*>(malloc_spy(count * sizeof(T))) : 0; } };
template <class T> struct CAllocProxy : CAllocator<T> { T* m_data; CAllocProxy(const CAllocator<T>& alloc, T* p) : CAllocator<T>(alloc), m_data(p) { } };
template <class T> struct CVectorBase {
    T* _M_start; T* _M_finish; CAllocProxy<T> _M_end_of_storage;
    CVectorBase(unsigned long count, const CAllocator<T>& alloc) : _M_start(0), _M_finish(0), _M_end_of_storage(alloc, 0)
    { _M_start = _M_end_of_storage.allocate(count); _M_finish = _M_start; _M_end_of_storage.m_data = _M_start + count; }
};
template <class T> struct CArray : CVectorBase<T> { explicit CArray(unsigned long count) : CVectorBase<T>(count, CAllocator<T>()) { this->_M_finish = this->_M_start; } };
struct CManagedTextureData;
struct CIVCountedPointer { CManagedTextureData* Object; };
struct CPreallocTexturePool : CResourceList {
    unsigned long Count; unsigned long MaxSize; long PoolIndex; CArray<CIVCountedPointer> Textures;
    virtual void v00() {}
    CPreallocTexturePool() : CResourceList(), Count(0), MaxSize(0), PoolIndex(-1), Textures(0) { }
};
struct CTextureManager : CBaseClassNonCopyable {
    bool Initialised; unsigned long PoolCount; unsigned long PreallocSizes[16]; CPreallocTexturePool PreallocPools[16];
    unsigned long FailedAllocations[16]; long BufferSize; void* TextureBuffer;
    virtual void v00() {}
    CTextureManager();
};
CTextureManager::CTextureManager() : CBaseClassNonCopyable(), Initialised(false), PoolCount(0), BufferSize(0), TextureBuffer(0)
{
    for (long i = 0; i < 16; ++i)
        FailedAllocations[i] = 0;
}
static int fail(const char* why) { std::printf("00a6a360_TEST FAIL: %s\n", why); return 1; }
int main()
{
    static unsigned char raw[sizeof(CTextureManager)];
    for (unsigned i = 0; i < sizeof(raw); ++i) raw[i] = 0xee;            // dirty storage: everything observed must be written by the ctor
    CTextureManager* m = new (raw) CTextureManager;
    if (g_baseCtors != 1 || g_listCtors != 16) return fail("base constructors");
    if (m->Initialised || m->PoolCount != 0 || m->BufferSize != 0 || m->TextureBuffer != 0) return fail("manager words");
    for (int i = 0; i < 16; ++i) {
        const CPreallocTexturePool& p = m->PreallocPools[i];
        if (p.Count != 0 || p.MaxSize != 0 || p.PoolIndex != -1 || p._body[0] != 0x5a) return fail("pool words");
        if (p.Textures._M_start != 0 || p.Textures._M_finish != 0 || p.Textures._M_end_of_storage.m_data != 0) return fail("pool vector");
        if (m->FailedAllocations[i] != 0) return fail("failure counters");
    }
    std::printf("00a6a360_TEST PASS\n");
    return 0;
}